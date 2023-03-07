#include "RpcCaller.h"
#include "ZkpClient.h"
using namespace zxrpc::rpc;

#include "Logger.h"
#include "Singleton.h"
#include "config_file.h"
using namespace zxrpc::tools;


RpcCaller::RpcCaller()
{
    config_file::setPath("./config/server.ini");
}

RpcCaller::~RpcCaller()
{
    m_socket.close();
}

bool RpcCaller::connect()
{
    return m_socket.connect(m_ip, m_port);
}

void RpcCaller::connect(const std::string & ip, int port){
    m_ip = ip;
    m_port = port;
    m_socket.connect(m_ip, m_port);
}

void RpcCaller::get_host_data(const string &method_name){
    ZkpClient zkCli;
    zkCli.Start();
    //  /zxrpc/method_name
    std::string method_path = "/" + m_service + "/" + method_name;
    // 127.0.0.1:9966
    std::string host_data = zkCli.GetData(method_path.c_str());
    if (host_data == "")
    {
        LOG_DEBUG("%s is not exist!", method_path.c_str());
        return;
    }
    int idx = host_data.find(":");
    if (idx == -1)
    {
        LOG_DEBUG("%s address is invalid!", method_path.c_str());
        return;
    }
    m_ip = host_data.substr(0, idx);
    m_port = atoi(host_data.substr(idx + 1, host_data.size() - idx).c_str());
}

void RpcCaller::set_service(const string & service_name){
    m_service = service_name;
}
