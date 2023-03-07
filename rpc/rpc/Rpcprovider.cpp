#include "Rpcprovider.h"
#include "ZkpClient.h"
using namespace zxrpc::rpc;

#include "Logger.h"
#include "System.h"
#include "config_file.h"
#include "Singleton.h"
using namespace zxrpc::tools;

#include "TaskDispatcher.h"
using namespace zxrpc::thread;

#include "SocketHandler.h"
using namespace zxrpc::socket;

RpcProvider::RpcProvider()
{
    //System * sys = Singleton<System>::instance();
    //sys->init();
    config_file::setPath("./config/server.ini");
    m_ip = config_file::instance()->GetStringInfo("server", "ip", "0.0.0.0");
    m_port = config_file::instance()->GetNumberInfo("server", "port", 8888);
    m_threads = config_file::instance()->GetNumberInfo("server", "threads", 32);
    m_connects = config_file::instance()->GetNumberInfo("server", "max_conn", 10000);
    m_wait_time = config_file::instance()->GetNumberInfo("server", "wait_time", 5);

    // Workflow * workflow = Singleton<Workflow>::instance();
    // workflow->load("./config/workflow.xml");
}

RpcProvider::~RpcProvider()
{
}

void RpcProvider::listen(const std::string & ip, int port)
{
    m_ip = ip;
    m_port = port;
}

void RpcProvider::start()
{
    // 初始化线程池和队列
    TaskDispatcher * dispatcher = Singleton<TaskDispatcher>::instance();
    dispatcher->init(m_threads);
    // 初始化socket句柄
    SocketHandler * socket_handler = Singleton<SocketHandler>::instance();
    socket_handler->listen(m_ip, m_port); 

    ZkpClient zkCli;
    zkCli.Start();
    // service_name为永久性节点    method_name为临时性节点
    // /service_name   /UserServiceRpc
    std::string service_path = "/" + m_service;
    zkCli.Create(service_path.c_str(), nullptr, 0);

    for (auto &mp : get_func_name())
    {
        std::string method_path = service_path + "/" + mp;
        char method_path_data[128] = {0};
        sprintf(method_path_data, "%s:%d", m_ip.c_str(), m_port);
        // ZOO_EPHEMERAL表示znode是一个临时性节点
        zkCli.Create(method_path.c_str(), method_path_data, strlen(method_path_data), ZOO_EPHEMERAL);
        serv_storage(mp);
    }

    // rpc服务端准备启动，打印信息
    LOG_DEBUG("RpcProvider start service at ip: %s port: %d", m_ip.c_str(), m_port);

    socket_handler->handle(m_connects, m_wait_time);
}

void RpcProvider::set_threads(int threads)
{
    m_threads = threads;
}

void RpcProvider::set_connects(int connects)
{
    m_connects = connects;
}

void RpcProvider::set_wait_time(int wait_time)
{
    m_wait_time = wait_time;
}

void RpcProvider::set_service(std::string name){
    m_service = name;
}

std::unordered_set<std::string> RpcProvider::get_func_name(){
    return m_func_name;
}

void RpcProvider::serv_storage(const std::string &name){
    MyDB mysql;
    if(!mysql.connect()){
        LOG_DEBUG("mysql connect Fail!");
    }

    bool flag = mysql.select_SQL("select * from rpc_serv where name = '" + name + "';");
    if(!flag){
        bool state = mysql.exeSQL("insert into rpc_serv values(null, '" + name + "', '0', '0', '0');");
    }
}