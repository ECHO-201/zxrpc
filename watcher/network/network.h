#include <muduo/net/TcpServer.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <boost/bind.hpp>
#include "function.h"

namespace watcher {
namespace network {
class Server{
public:
	Server(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr, int numThreads);
	void start();

	void func_bind(std::string func_name, std::string model_name, int num);

private:
	void onConnection(const muduo::net::TcpConnectionPtr &conn);
	void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp time);
	
	muduo::net::EventLoop *loop_;
	muduo::net::TcpServer server_;
	int numThreads_;
	
	Function funct;
};

}}