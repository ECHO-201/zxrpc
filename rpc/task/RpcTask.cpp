#include "RpcTask.h"
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

#include <sstream>
using std::ostringstream;

#include "MyDB.h"
using namespace zxrpc::mysql;

RpcTask::RpcTask(Socket* socket) : Task(socket)
{
}

RpcTask::~RpcTask()
{
}

void RpcTask::run()
{
    LOG_DEBUG("work task run");
    SocketHandler *handler = Singleton<SocketHandler>::instance();

    Socket *socket = static_cast<Socket *>(m_data);

    MsgHead msg_head;
    memset(&msg_head, 0, sizeof(msg_head));
    int len = socket->recv((char *)(&msg_head), sizeof(msg_head));

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

    DataStream in;
    in.write(buf, len);

    string func;
    in >> func;

    DataStream out;
    RpcProvider *server = Singleton<RpcProvider>::instance();
    switch (msg_head.cmd)
    {
    case CALL:
        server->call(func, in, out);
        socket->send(out.data(), out.size());
        break;
    case SEND:
        server->call(func, in, out);
        break;
    default:
        break;
    }
    handler->attach(socket);

    call_method(func);
}

void RpcTask::destroy()
{
    LOG_DEBUG("work job destory");
    delete this;
}

void RpcTask::call_method(std::string func){
    MyDB* mydb = new MyDB();
    if(!mydb->connect()){
        LOG_DEBUG("work job destory");
    }

    mydb->exeSQL("update rpc_serv set cnt = cnt + 1 where name = '" + func +"';");
}
