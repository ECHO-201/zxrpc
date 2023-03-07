#pragma once

#include "Task.h"
using namespace zxrpc::thread;

#include "Socket.h"
using namespace zxrpc::socket;

namespace zxrpc {
namespace task {

class WorkTask : public Task
{
public:
    WorkTask(Socket * socket);
    virtual ~WorkTask();

    virtual void run();
    virtual void destroy();
};

}}
