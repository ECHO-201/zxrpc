#pragma once

#include <pthread.h>
#include <signal.h>
#include <list>
#include "Thread.h"
#include "ThreadPool.h"
#include "Task.h"


namespace zxrpc {
namespace thread {

class TaskDispatcher : public Thread
{
public:
    TaskDispatcher(); // 与主线程分离
    ~TaskDispatcher();

    void init(int threads);
    /**
     * @brief 把任务放入任务队列
     */
    void assign(Task* task);
    void handle(Task* task);
    virtual void run();

private:
    std::list<Task *> m_tasks;
    Mutex m_mutex;

    // 条件变量
    Condition m_cond;
};

}}
