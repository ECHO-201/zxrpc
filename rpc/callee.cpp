#include <iostream>
#include <algorithm>
#include "proto/echo.pb.h"
#include "Logger.h"

#include "LoadSoLibrary.h"

#include "Rpcprovider.h"

using namespace zxrpc::rpc;


string hello(const string & name)
{
    Workflow *workflow = Singleton<Workflow>::instance();

    ostringstream os;
    os << (int)(1);
    const string work = os.str();

    const string input = name;
    string output;

    workflow->run(work, input, output);
    return "hello, " + output;
}

string fail_method(const string & name)
{
    return "-1";  //假设请求失败
}

int sum(int a, int b)
{
    //return a + b;
    
    typedef int (*calc_func)(int, int);
    // void* handle = dlopen("./plugin/libctest.so", RTLD_LAZY);
    // if (!handle) {
    //     LOG_DEBUG("dlerror is %s", dlerror());
    //     exit(EXIT_FAILURE);
    // }
    // string function_name = "add";
    // calc_func func = reinterpret_cast<calc_func>(
    //     dlsym(handle, function_name.c_str()));
    // int x = func(a, b);
    // dlclose(handle);
    LoadSoLibrary* loadSoLibrary = new LoadSoLibrary();
    std::string PATH = "./plugin/libctest.so";
    std::string function_name = "add";
    calc_func func = reinterpret_cast<calc_func>(
        loadSoLibrary->run<calc_func>(PATH, function_name));
    int x = func(a, b);
    delete loadSoLibrary;
    loadSoLibrary = NULL;
    return x;
}

class Request : public Serializable
{
public:
    Request() {}
    Request(const string &name) : m_name(name) {}
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

Response upper(const Request & req)
{
    string name = req.name();
    transform(name.begin(), name.end(), name.begin(), ::toupper);
    return Response(name);
}

string content(const string & name)
{
    proto_test::EchoMessage request, response;
    response.ParseFromArray(name.c_str(), name.size());
    string content = response.content();
    request.set_content("content = " + content);
    std::string rspString;
    request.SerializeToString(&rspString);
    return rspString;
}

int main()
{
    RpcProvider *rpcProvider = Singleton<RpcProvider>::instance();
    rpcProvider->listen("127.0.0.1", 9966);
    rpcProvider->set_service("zxrpc");
    rpcProvider->bind("hello", hello);
    rpcProvider->bind("sum", sum);
    rpcProvider->bind("upper", upper);
    rpcProvider->bind("content", content);
    rpcProvider->bind("fail_method", fail_method);
    rpcProvider->start();

    return 0;
}
