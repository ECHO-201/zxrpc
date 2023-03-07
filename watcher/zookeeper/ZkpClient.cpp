#include "ZkpClient.h"
#include <semaphore.h>
#include <iostream>
#include "Logger.h"
#include "config_file.h"

using namespace watcher::tools;
using namespace watcher::zookeeper;

void global_watcher(zhandle_t *zh, int type,
                   int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  
	{
		if (state == ZOO_CONNECTED_STATE)  
		{
			sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
		}
	}
}

ZkpClient::ZkpClient() : m_zkpdle(nullptr)
{
}

ZkpClient::~ZkpClient()
{
    if (m_zkpdle != nullptr)
    {
        zookeeper_close(m_zkpdle); 
    }
}

void ZkpClient::Start()
{
    std::string host = config_file::instance()->GetStringInfo("zookeeper", "zookeeperip", "0.0.0.0");
    std::string port = config_file::instance()->GetStringInfo("zookeeper", "zookeeperport", "8000");
    std::string connstr = host + ":" + port;
	
    m_zkpdle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zkpdle) 
    {
        LOG_DEBUG("zookeeper_init error!");
        exit(EXIT_FAILURE);
    }
	
    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zkpdle, &sem);
	
    sem_wait(&sem);
    LOG_DEBUG("zookeeper_init success!");
}

void ZkpClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;

	flag = zoo_exists(m_zkpdle, path, 0, nullptr);
	if (ZNONODE == flag) 
	{
		flag = zoo_create(m_zkpdle, path, data, datalen,
			&ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
		if (flag == ZOK)
		{
			LOG_DEBUG("znode create success... path: %s", path);
		}
		else
		{
			LOG_DEBUG("flag: %d", flag);
			LOG_DEBUG("znode create error... path: %s", path);
			exit(EXIT_FAILURE);
		}
	}
}

std::string ZkpClient::GetData(const char *path)
{
    char buffer[64];
	int bufferlen = sizeof(buffer);
	int flag = zoo_get(m_zkpdle, path, 0, buffer, &bufferlen, nullptr);
	if (flag != ZOK)
	{
		LOG_DEBUG("get znode error... path: %s", path);
		return "";
	}
	else
	{
		return buffer;
	}
}

bool ZkpClient::GetChildren(const char *path, bool watch_cast, String_vector &s_v)
{
	int flag = zoo_get_children(m_zkpdle, path, watch_cast, &s_v);
	if (flag != ZOK)
	{
		LOG_DEBUG("get children error... path: %s", path);
		return false;
	}
	return true;
}

bool ZkpClient::DelNode(const char *path){
	int flag = zoo_delete(m_zkpdle, path, 0);
	if(flag != ZOK){
		LOG_DEBUG("get children error... path: %s", path);
		return false;
	}
	return true;
}