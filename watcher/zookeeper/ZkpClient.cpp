#include "ZkpClient.h"
#include <semaphore.h>
#include <iostream>
#include "Logger.h"
#include "config_file.h"

using namespace watcher::tools;
using namespace watcher::zookeeper;

// 全局的watcher观察器   zkserver给ZkpClient的通知
void global_watcher(zhandle_t *zh, int type,
                   int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  // 回调的消息类型是和会话相关的消息类型
	{
		if (state == ZOO_CONNECTED_STATE)  // ZkpClient和zkserver连接成功
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
        zookeeper_close(m_zkpdle); // 关闭句柄，释放资源  MySQL_Conn
    }
}

// 连接zkserver
void ZkpClient::Start()
{
    std::string host = config_file::instance()->GetStringInfo("zookeeper", "zookeeperip", "0.0.0.0");
    std::string port = config_file::instance()->GetStringInfo("zookeeper", "zookeeperport", "8000");
    std::string connstr = host + ":" + port;
	/*
	zookeeper_mt：多线程版本
	zookeeper的API客户端程序提供了三个线程
	1.API调用线程 
	2.网络I/O线程  pthread_create和poll
	3.watcher回调线程 pthread_create
	*/
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
	// 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
	flag = zoo_exists(m_zkpdle, path, 0, nullptr);
	if (ZNONODE == flag) // 表示path的znode节点不存在
	{
		// 创建指定path的znode节点了
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