#include "network.h"
#include "config_file.h"
#include "func_bind.h"
#include "watcher.h"
#include <iostream>

using namespace watcher::tools;
using namespace watcher::network;

int main()
{
	LOG_INFO << "pid = " << getpid();
	muduo::net::EventLoop loop;
	
	config_file::setPath("./conf/server.ini");
	short int port = config_file::instance()->GetNumberInfo("server", "port", 10001);
	muduo::net::InetAddress listenAddr(port);

	Server server(&loop, listenAddr, 2); // 开2线程
	
	server.func_bind("login", "User", LOGIN);
	server.func_bind("get_log", "User", GET_LOG);
	server.func_bind("del_log", "User", DEL_LOG);
	server.func_bind("regis", "User", REGISTER);
	server.func_bind("get_node", "Watcher", GET_NODE);
	server.func_bind("del_node", "Watcher", DEL_NODE);

	server.start();

	loop.loop();
    return 0;
}
