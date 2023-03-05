#include "WorkTask.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Types.h"

#include "Logger.h"

#include "Rpcprovider.h"
using namespace zxrpc::rpc;

#include "Singleton.h"

using namespace zxrpc::tools;

#include "SocketHandler.h"
using namespace zxrpc::socket;

#include "Workflow.h"
using namespace zxrpc::work;
using namespace zxrpc::task;

#include "redis.h"
using namespace zxrpc::redis;

#include "DataStream.h"
using namespace zxrpc::serialize;

#include <sstream>
using std::ostringstream;

#include "MyDB.h"
using namespace zxrpc::mysql;

WorkTask::WorkTask(Socket* socket) : Task(socket)
{
}

WorkTask::~WorkTask()
{
}

void WorkTask::run()
{
    LOG_DEBUG("work task run");
    SocketHandler *handler = Singleton<SocketHandler>::instance();

    Socket *socket = static_cast<Socket *>(m_data);

    MsgHead msg_head;
    memset(&msg_head, 0, sizeof(msg_head));
    int len = socket->recv((char *)(&msg_head), sizeof(msg_head));
    LOG_DEBUG("%d", len);
    if (len == 0)
    {
        LOG_WARN("socket closed by peer");
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EAGAIN)
    {
        LOG_WARN("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len == -1 && errno == EWOULDBLOCK)
    {
        LOG_WARN("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len == -1 && errno == EINTR)
    {
        LOG_WARN("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len != sizeof(msg_head))
    {
        LOG_ERROR("recv msg head error length: %d, errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    LOG_INFO("recv msg head len: %d, flag: %s, cmd: %d, body len: %d", len, msg_head.flag, msg_head.cmd, msg_head.len);
    if (strncmp(msg_head.flag, "work", 4) != 0)
    {
        LOG_ERROR("msg head flag error");
        handler->remove(socket);
        return;
    }

    if (msg_head.len >= uint32_t(recv_buff_size))
    {
        LOG_ERROR("recv msg body len: %d, large than recv_buff_size: %d", msg_head.len, recv_buff_size);
        handler->remove(socket);
        return;
    }

    char buf[recv_buff_size];
    memset(buf, 0, recv_buff_size);
    len = socket->recv(buf, msg_head.len);
    if (len == -1 && errno == EAGAIN)
    {
        LOG_WARN("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EWOULDBLOCK)
    {
        LOG_WARN("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EINTR)
    {
        LOG_WARN("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    if (len != (int)(msg_head.len))
    {
        LOG_ERROR("recv msg body error length: %d, body: %s, errno: %d", len, buf, errno);
        handler->remove(socket);
        return;
    }    

    LOG_INFO("recv msg content len: %d, msg data: %s", len, buf);

    std::string workflow_name = "./config/workflow.xml";
    struct stat stat_buf;
    if(stat(workflow_name.c_str(), &stat_buf) != 0){
        LOG_DEBUG("workflow_name is no exist!");
        return;
    }
    
    Redis rds;
	if(!rds.connect())
	{
		LOG_DEBUG("connect error!");
		return;
	}

    std::string workflow_time = rds.get(workflow_name);
    std::string workflow_mtime = ctime(&stat_buf.st_mtime);
    if(workflow_time.empty() || workflow_mtime != workflow_time){
        LOG_DEBUG("change workflow");
        Workflow *workflow = Singleton<Workflow>::instance();
        workflow->clear();
        workflow->load(workflow_name);
        rds.set(workflow_name, workflow_mtime);
    }

    Workflow *workflow = Singleton<Workflow>::instance();

    ostringstream os;
    os << (int)(msg_head.cmd);
    const string work = os.str();
    const string input = buf;
    string output;

    workflow->run(work, input, output);

    socket->send(output.c_str(), output.length());
    handler->attach(socket);
}

void WorkTask::destroy()
{
    LOG_DEBUG("work job destory");
    delete this;
}
