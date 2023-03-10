#pragma once

#include "model.h"
#include <map>

class Watcher : model
{
public:
    Watcher();
    ~Watcher();
    
    std::string GetChildrenData(Json::Value &json);
    std::string DelNode(Json::Value &json);
    std::string run(const TcpConnectionPtr &conn, Json::Value &json, int &num); 
    MsgHandler getHandler(int msgid);

private:
    std::string dir;
    TcpConnectionPtr conn_ptr;
    std::map<const int, MsgHandler> _msgHandlerMap;
};

