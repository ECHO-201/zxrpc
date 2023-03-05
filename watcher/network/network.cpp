#include "network.h"
#include "user.h"
#include "MyDB.h"
#include "config_file.h"
#include "redis.h"

using namespace watcher::redis;
using namespace watcher::tools;

Server::Server(muduo::net::EventLoop *loop, const muduo::net::InetAddress &listenAddr, int numThreads):loop_(loop), 
server_(loop, listenAddr, "Server"), 
numThreads_(numThreads){
	server_.setConnectionCallback(boost::bind(&Server::onConnection, this, boost::placeholders::_1));
	server_.setMessageCallback(boost::bind(&Server::onMessage, this, boost::placeholders::_1, 
	boost::placeholders::_2, boost::placeholders::_3));
	server_.setThreadNum(numThreads);
}

void Server::start(){
	server_.start();
}

void Server::onConnection(const muduo::net::TcpConnectionPtr &conn){ // TCP连接与断开时都会调用它
	LOG_INFO << "Server - client(" << conn->peerAddress().toIpPort() << ") -> server(" << 
	conn->localAddress().toIpPort() << ") is " << (conn->connected() ? "UP":"DOWN");
	if(!conn->connected()){//客户端断开连接
		Redis redis;
		if(!redis.connect())
		{
			LOG_ERROR << "connect error!";
			return;
		}
		if(redis.del(conn->name())){	
			conn->shutdown();  // 关闭套接字的写端
		}
    }
}

void Server::onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, 
	muduo::Timestamp time){
	muduo::string msg(buf->retrieveAllAsString()); // 取走数据
	LOG_INFO << conn->name() << "echo " << msg.size() << "bytes, " << "data received at " 
	<< time.toString();
	
	std::cout<< msg <<std::endl;
	funct.json_msg(msg);
	msg = funct.run(conn);
	
  	conn->send(msg);
}

void Server::func_bind(std::string func_name, std::string model_name, int num){
	funct.bind(func_name, model_name, num);
}