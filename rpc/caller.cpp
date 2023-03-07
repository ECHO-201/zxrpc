#include "RpcCaller.h"
#include "Load_balance.h"
using namespace zxrpc::rpc;

#include "proto/echo.pb.h"
#include <time.h>
#include <unistd.h>

#include "MyDB.h"
using namespace zxrpc::mysql;

class Request : public Serializable
{
public:
    Request() {}
    Request(const string & name) : m_name(name) {}
    ~Request() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};

class Response : public Serializable
{
public:
    Response() {}
    Response(const string & name) : m_name(name) {}
    ~Response() {}

    const string & name() const
    {
        return m_name;
    }

    SERIALIZE(m_name)

private:
    string m_name;
};

void stroge_succ_fail(MyDB *mysql, string name, string reply){    
    if(reply != "-1"){
        mysql->exeSQL("update rpc_serv set succ = succ + 1 where name = '" + name + "';");
    }else{
        mysql->exeSQL("update rpc_serv set fail = fail + 1 where name = '" + name + "';");
    }
}


int main()
{   
    RpcCaller *rpcCaller = new RpcCaller();

    LoadBalance loadbl;
    loadbl.set_service("zxrpc");
    //loadbl.set_service("bkrpc");
    std::string ld = loadbl.ramdomStrategy();
    rpcCaller->set_service(ld);
    rpcCaller->get_host_data("sum");
    rpcCaller->connect();
    MyDB *mysql = new MyDB();
    mysql->connect();

    auto reply = rpcCaller->call<string>("hello", "shadow");
    std::cout << reply << std::endl;
    
    // auto reply = rpcCaller->call<int>("sum", 3, 4);
    // std::cout << reply << std::endl;
    // stroge_succ_fail(mysql, "sum", std::to_string(reply));

    auto method = rpcCaller->call<string>("fail_method", "xxx");
    stroge_succ_fail(mysql, "fail_method", method);

    //auto total = client.call<int>("sum", 1, 2);
    //std::cout << total << std::endl;

    // Request req("BK-201");
    // auto resp = rpcCaller->call<Response>("upper", req);
    // std::cout << resp.name() << std::endl;

    // proto_test::EchoMessage request, response;
    // request.set_content("shadow");
    // std::string requestString;
    // request.SerializeToString(&requestString);
    // auto rsp = rpcCaller->call<string>("content", requestString);
    // response.ParseFromArray(rsp.c_str(), rsp.size());
    // std::cout << response.content() << std::endl;
    
    //client.send("sum", 1, 2);

    delete mysql;
    mysql = NULL;

    delete rpcCaller;
    rpcCaller = NULL;

    return 0;
}