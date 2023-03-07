#include "watcher.h"
#include "ZkpClient.h"
#include <iostream>
#include "func_bind.h"
#include "Logger.h"
#include <string>
#include <config_file.h>
#include "redis.h"
#include "MyDB.h"
#include "Time.h"

using namespace watcher::redis;
using namespace watcher::tools;
using namespace watcher::mysql;
using namespace watcher::zookeeper;

Watcher::Watcher()
{
	dir = config_file::instance()->GetStringInfo("zookeeper", "zookeeperdir", "0.0.0.0");

	_msgHandlerMap.insert({GET_NODE, bind(&Watcher::GetChildrenData, this, _1)});
    _msgHandlerMap.insert({DEL_NODE, bind(&Watcher::DelNode, this, _1)});
}

Watcher::~Watcher()
{

}

std::string Watcher::GetChildrenData(Json::Value &json){
	String_vector s_v;
	ZkpClient zli;
    zli.Start();

    bool flag = zli.GetChildren(json["path"].asString().c_str(), 1, s_v);
    if(!flag){
        return "";
    }
	std::string msg, node_name;
	Json::Value jmsg;
	for(int i = 0; i < s_v.count; ++i){
		node_name = s_v.data[i];
		if(node_name != "zookeeper"){
			String_vector str_v;
			Json::Value j_v;
			const char* children_node = ("/" + node_name).c_str();
			bool flag = zli.GetChildren(children_node, 1, str_v);
			for(int i = 0; i < str_v.count; ++i){
				std::string child_name = str_v.data[i]; 
				//children +=  child_name + ":" + zli.GetData(("/" + node_name + "/" + child_name).c_str()) + "|";
				std::string host_data = zli.GetData(("/" + node_name + "/" + child_name).c_str());
				int idx = host_data.find(":");
				if (idx == -1)
				{
					LOG_DEBUG("%s address is invalid!", node_name.c_str());
					return "";
				}
				std::string ip = host_data.substr(0, idx);
				std::string port = host_data.substr(idx + 1, host_data.size()-idx).c_str();

				j_v["serv_name"] = child_name;
				j_v["ip"] = ip;
				j_v["port"] = port;
				jmsg.append(j_v);
			}
		}
	}

	Json::FastWriter writer;
    msg = writer.write(jmsg);

	return msg;
}

std::string Watcher::DelNode(Json::Value &json){
	ZkpClient zli;
    zli.Start();
	
	std::string serv_name = json["serv_name"].asString().c_str();
	const char *node_path = (dir + "/" + serv_name).c_str();
	bool flag = zli.DelNode(node_path);

	if(!flag){
        return "";
    }
	json["path"] = "/";
	Redis redis;
	if(!redis.connect())
    {
        LOG_ERROR("connect error!");
        return "";
    }

	MyDB *mydb = new MyDB();
	Time time;
	bool state = mydb->exeSQL("insert into op_service values('" + redis.get(conn_ptr->name()) + "', '" +
                 time.getTime() + "', '" + "delete" + "','"+ "delete " + serv_name + "');");
	if(!state){
		LOG_ERROR("insert %s del %s fail!", redis.get(conn_ptr->name()).c_str(), 
		serv_name.c_str());
	}

	delete mydb;

   	return GetChildrenData(json);
}


std::string Watcher::run(const TcpConnectionPtr &conn, Json::Value &json, int &num){
    std::string msg;
	conn_ptr = conn;
    auto msgHandler = getHandler(num);
    msg = msgHandler(json);
    return msg;
}

MsgHandler Watcher::getHandler(int msgid)
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