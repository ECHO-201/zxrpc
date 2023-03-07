#pragma once

#include <iostream>
#include <string>
#include "MyDB.h"
#include "model.h"

#include <map>

using namespace watcher::mysql;

class User : model {
public:
    User();
    ~User();
    std::string Login(Json::Value &json);
    
    std::string get_log(Json::Value &json);
    std::string del_log(Json::Value &json);
    std::string regis(Json::Value &json);
    std::string run(const TcpConnectionPtr &conn, Json::Value &json, int &num);
    MsgHandler getHandler(int msgid);
private:
        
    MyDB *mydb; 
    TcpConnectionPtr conn_ptr;
    std::map<const int, MsgHandler> _msgHandlerMap;
};
