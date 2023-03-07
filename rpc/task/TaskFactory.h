#pragma once

#include "Socket.h"
using namespace zxrpc::socket;

#include "Task.h"
using namespace zxrpc::thread;

#include "RpcTask.h"
#include "WorkTask.h"
using namespace zxrpc::task;

namespace zxrpc {
namespace task {

class TaskFactory
{
public:
    static Task* create(Socket * socket)
    {
        return new RpcTask(socket);
    }
    static Task* createWork(Socket * socket)
    {
        return new WorkTask(socket);
    }
};

}}