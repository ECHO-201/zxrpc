#pragma once

#include <string>
#include <hiredis/hiredis.h>
 
namespace zxrpc{
namespace redis{
class Redis
{
public:
 
    Redis();
    ~Redis();

    bool connect();
	bool connect(std::string &ip, int port);    

    std::string get(std::string key);
	
	void set(std::string key, std::string value);
 
private:
    std::string m_ip;
    int m_port;
    redisContext* m_connect;
	redisReply* m_reply;
				
};

}}

