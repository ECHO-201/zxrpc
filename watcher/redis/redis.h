#pragma once

#include <string>
#include <hiredis/hiredis.h>
 
namespace watcher{
namespace redis{
class Redis
{
public:
 
    Redis();
    //释放资源
    ~Redis();

    //创建连接
    bool connect();
	bool connect(std::string &ip, int port);    

    //get请求
    std::string get(std::string key);
	
    //set请求
	void set(std::string key, std::string value);
    bool del(std::string key);

private:
    std::string m_ip;
    int m_port;
    redisContext* m_connect;
	redisReply* m_reply;
				
};

}}

