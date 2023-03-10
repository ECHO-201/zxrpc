#include "user.h"
#include <string>
#include "config_file.h"
#include "func_bind.h"
#include "Logger.h"
#include "redis.h"
#include "Time.h"

using namespace watcher::redis;
using namespace watcher::tools;
using namespace watcher::mysql; 

User::User(){
    mydb = new MyDB();
    //config_file::setPath("../conf/server.ini");

    _msgHandlerMap.insert({LOGIN, bind(&User::Login, this, _1)});
    _msgHandlerMap.insert({GET_LOG, bind(&User::get_log, this, _1)});
    _msgHandlerMap.insert({DEL_LOG, bind(&User::del_log, this, _1)});
    _msgHandlerMap.insert({REGISTER, bind(&User::regis, this, _1)});
    _msgHandlerMap.insert({GET_USER, bind(&User::get_user, this, _1)});
    _msgHandlerMap.insert({UP_PASSWD, bind(&User::update_passwd, this, _1)});
};

User::~User(){
    delete mydb;
};

std::string User::Login(Json::Value &json){
    std::vector<user> vec;
    std::string id = json["id"].asString();
    vec = mydb->exe_select("select * from user where id='"+ id +"' and passwd ='"
    + json["passwd"].asString() +"';");

    Redis redis;
    if(!redis.connect())
    {
        LOG_DEBUG("connect error!");
        return "";
    }
    redis.set(conn_ptr->name(), id);

    Json::Value j_v;
    std::string status = vec.size() != 0 ? "1" : "0";
    j_v["status"] = status;

    if(status == "1"){
        std::string mac_addr = json["mac_addr"].asString();
        Time time;
        
        bool state = mydb->exeSQL("insert into LOGIN values(null, '" + id + 
        "','" + mac_addr + "','" + conn_ptr->peerAddress().toIpPort() + "','" + time.getTime() + "');");
        if(!state){
            LOG_ERROR("insert LOGIN id = %s, mac_add = %s, ip = %s", id.c_str(), 
                mac_addr.c_str(), conn_ptr->peerAddress().toIpPort().c_str());
        }
    }

    return json_string(j_v);
};

std::string User::get_log(Json::Value &json){
    std::vector<log_data> vec;
    vec = mydb->get_log("select id, ip, port, record from ERROR_log;");
    std::string str = "";
    for(unsigned int i = 0; i < vec.size(); ++i){
        log_data ld = vec[i];
        str += ld.id + "|" + ld.record + "  " + ld.ip + "  " + ld.port + "|";
    }
    std::cout << str << std::endl;
    return str;
}

std::string User::del_log(Json::Value &json){
    std::string log_id = json["log_id"].asString();
    
    mydb->begin_transaction();
    
    bool flag = mydb->exeSQL("delete from ERROR_log where id = '" + log_id + "';");
    if(!flag){
        LOG_ERROR("delete log fail!");
        return "";
    }
    

    Redis redis;
    if(!redis.connect())
    {
        LOG_ERROR("connect error!");
        return "";
    }
    Time time;
	std::string name = redis.get(conn_ptr->name());
	bool state = mydb->exeSQL("insert into op_log values('" + name + "', '" +
                 time.getTime() + "', '" + "delete" + "','"+ "delete " + log_id + "');");
	if(!state){
		LOG_ERROR("insert %s del %s fail!", name.c_str(), 
		log_id.c_str());
        mydb->rollback_transaction();
        return "";
	}
    
    mydb->commit_transaction();
    return get_log(json);
}

std::string User::regis(Json::Value &json){
    std::string id = json["id"].asString();
    bool state = mydb->exeSQL("insert into user values('" + id + "','" 
    + json["name"].asString() + "','" + json["passwd"].asString() + "','" 
    + json["phone"].asString() + "');");
    
    Json::Value j_v;
    if(state){
        j_v["status"] = "succ";
    }else{
        j_v["status"] = "fail";
    }

   std::string mac_addr = json["mac_addr"].asString();
   Time time;
    
    state = mydb->exeSQL("insert into REGISTER_log values(null, '" + id + 
    "','" + mac_addr + "','" + conn_ptr->peerAddress().toIpPort() + "','" + time.getTime() + "');");
    if(!state){
        LOG_ERROR("insert REGISTER_log id = %s, mac_add = %s", id.c_str(), mac_addr.c_str());
    }
    
    return json_string(j_v);
}

std::string User::get_user(Json::Value &json){
    std::vector<user> vec;
    Redis redis;
    if(!redis.connect())
    {
        LOG_DEBUG("connect error!");
        return "";
    }
    std::string id = redis.get(conn_ptr->name());
    vec = mydb->get_user("select name, phone from user where id = '" + id + "';");

    Json::Value j_v;
    j_v["id"] = id;
    j_v["name"] = vec[0].name;
    j_v["phone"] = vec[0].phone;

    return json_string(j_v);
}

std::string User::update_passwd(Json::Value &json){
    Redis redis;
    if(!redis.connect())
    {
        LOG_DEBUG("connect error!");
        return "";
    }
    std::string id = redis.get(conn_ptr->name());
    std::cout << "update user set passwd = '" << json["passwd"].asString() <<
    "' where id = '" << id << "';" << std::endl;
    bool flag = mydb->exeSQL("update user set passwd = '" + json["passwd"].asString() +
    "' where id = '" + id + "';");

    Json::Value j_v;

    if(flag){
        j_v["status"] = "succ";
    }else{
        j_v["status"] = "fail";
    }
    
    return json_string(j_v);
}

std::string User::run(const TcpConnectionPtr &conn, Json::Value &json, int &num){
    std::string msg;
    conn_ptr = conn;
    auto msgHandler = getHandler(num);
    msg = msgHandler(json);
    
    return msg;
}

MsgHandler User::getHandler(int msgid)
{
    auto it = _msgHandlerMap.find(msgid);
    if(it == _msgHandlerMap.end()){
        return [=](Json::Value &json){
            LOG_ERROR("msgid: %d can not find handler!", msgid);
            return "";
        };     
    }
    else{
         return _msgHandlerMap[msgid];
    }
}
