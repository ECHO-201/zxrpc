#pragma once

#include "Task.h"
using namespace zxrpc::thread;

#include "Socket.h"
using namespace zxrpc::socket;

#include "DataStream.h"
using namespace zxrpc::serialize;

namespace zxrpc {
namespace task {

class RpcTask : public Task
{
public:
    RpcTask(Socket * socket);
    virtual ~RpcTask();

    void call_method(std::string func);

    virtual void run();
    virtual void destroy();
};

}}
