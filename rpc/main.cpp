#include <iostream>

#include "System.h"
#include "config_file.h"
#include "Singleton.h"
#include "Logger.h"
using namespace zxrpc::tools;

#include "Server.h"
using namespace zxrpc::server;


int main()
{
    
    config_file::setPath("./config/main.ini");
    const std::string ip = config_file::instance()->GetStringInfo("server", "ip", "0.0.0.0");
    short port = config_file::instance()->GetNumberInfo("server", "port", 8888);
    int threads = config_file::instance()->GetNumberInfo("server", "threads", 32);
    int max_conn = config_file::instance()->GetNumberInfo("server", "max_conn", 10000);

    Server *server = Singleton<Server>::instance();
    server->listen(ip, port);
    server->set_threads(threads);
    server->set_connects(max_conn);
    server->start();

    return 0;
}
