#pragma once

#include "model.h"
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <jsoncpp/json/json.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;


class Function{
public:
	Function();
    ~Function();

    void bind(std::string func_name, std::string model_name, int num);

    std::string run(const TcpConnectionPtr &conn);

    void json_msg(std::string msg);

    model* get_model(std::string model_name);

private:
    std::map<std::string, std::pair<std::string, int>> func;
    Json::Value root;
};