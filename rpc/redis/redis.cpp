#include "redis.h"
#include "Logger.h"
#include "config_file.h"

using namespace zxrpc::tools;  
using namespace zxrpc::redis;  

Redis::Redis(){

}

Redis::~Redis()
{
    this->m_connect = NULL;
    this->m_reply = NULL;	    	    
}

bool Redis::connect(){
    m_ip = config_file::instance()->GetStringInfo("redis", "redisip", "0.0.0.0");
    m_port = config_file::instance()->GetNumberInfo("redis", "redisport", 6379);
    return connect(m_ip, m_port);
}

bool Redis::connect(std::string &ip, int port)
{
    this->m_connect = redisConnect(ip.c_str(), port);
    if(this->m_connect != NULL && this->m_connect->err)
    {
        LOG_DEBUG("connect error: %s", this->m_connect->errstr);
        return false;
    }
    return true;
}

std::string Redis::get(std::string key)
{
    this->m_reply = (redisReply*)redisCommand(this->m_connect, "GET %s", key.c_str());
    std::string str;
    if(this->m_reply->str == NULL){
        freeReplyObject(this->m_reply);
        return str;
    }

    str = this->m_reply->str;
    freeReplyObject(this->m_reply);
    return str;
}

void Redis::set(std::string key, std::string value)
{
    redisCommand(this->m_connect, "SET %s %s", key.c_str(), value.c_str());
}
