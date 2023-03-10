#include "function.h"
#include "user.h"
#include "watcher.h"
#include <muduo/base/Logging.h>

Function::Function(){

}

Function::~Function(){

}

void Function::bind(std::string func_name, std::string model_name, int num){
    func[func_name].first = model_name;
    func[func_name].second = num;
}

std::string Function::run(const TcpConnectionPtr &conn){
    model* md = get_model(func[root["function"].asString()].first);
    int func_num = func[root["function"].asString()].second;
    std::string msg = md->run(conn, root, func_num);

    delete md;
    return msg;
}

void Function::json_msg(std::string msg){
    Json::Reader reader;
    reader.parse(msg, root);
}

model* Function::get_model(std::string model_name){
    if(model_name == "User"){
        return (model*)new User();
    }else if(model_name == "Watcher"){
        return (model*)new Watcher();
    }

    return nullptr;
}
