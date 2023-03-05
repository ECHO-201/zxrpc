#pragma once

#include <string>
#include <unordered_set>

#include "Singleton.h"
using namespace zxrpc::tools;

#include "FunctionHandler.h"

#include "Workflow.h"
using namespace zxrpc::work;

#include "MyDB.h"
using namespace zxrpc::mysql;

namespace zxrpc {
namespace rpc {

class RpcProvider
{
public:
    RpcProvider();
    ~RpcProvider();

    void listen(const std::string & ip, int port);
    void start();
    void set_threads(int threads);
    void set_connects(int connects);
    void set_wait_time(int wait_time);
    void set_service(std::string name);
    std::unordered_set<std::string> get_func_name();
    void serv_storage(const std::string &name);

    template <typename F>
    void bind(const std::string &name, F func)
    {
        m_func_name.insert(name);
        m_handler.bind(name, func);
    }

    void call(const std::string & name, DataStream & in, DataStream & out)
    {
        m_handler.call(name, in, out);
    }

private:
    std::string m_ip;
    int m_port;
    int m_threads;
    int m_connects;
    int m_wait_time;

    std::string m_service;
    std::unordered_set<std::string> m_func_name;

    FunctionHandler m_handler;
};

}}
