#pragma once
#include <string>
#include <jsoncpp/json/json.h>
#include <functional>
#include <muduo/net/TcpConnection.h>
using namespace muduo;
using namespace muduo::net;


using MsgHandler = std::function<std::string(Json::Value &json)>;

class model
{
public:
    model(){};
    virtual ~model(){};

    virtual std::string run(const TcpConnectionPtr &conn, Json::Value &root, int &num) = 0;

    std::string json_string(Json::Value &json){
        Json::FastWriter writer;
        return writer.write(json);
    }
};

