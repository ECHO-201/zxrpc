#include "TaskDispatcher.h"
#include "Logger.h"

#include "Singleton.h"

using namespace zxrpc::tools;
using namespace zxrpc::thread;

TaskDispatcher::TaskDispatcher()
{
}

TaskDispatcher::~TaskDispatcher()
{
}

void TaskDispatcher::init(int threads)
{
    Singleton<ThreadPool>::instance()->create(threads);
    start();
}

void TaskDispatcher::assign(Task* task)
{
    LOG_DEBUG("task dispatcher assign task");
    m_mutex.lock();
    m_tasks.push_back(task);
    m_mutex.unlock();
    m_cond.signal();
}

void TaskDispatcher::handle(Task* task)
{
    LOG_DEBUG("task dispatcher handle task");
    ThreadPool * threadpool = Singleton<ThreadPool>::instance();
    if (threadpool->get_idle_thread_numbers() > 0)
    {
        threadpool->assign(task);
    }
    else
    {
        m_mutex.lock();
        m_tasks.push_front(task);
        m_mutex.unlock();
        LOG_DEBUG("all threads are busy!");
    }
}

void TaskDispatcher::run()
{
    sigset_t mask;
    if (0 != sigfillset(&mask))
    {
        LOG_ERROR("thread manager sigfillset failed!");
        return;
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    {
        LOG_ERROR("thread manager pthread_sigmask failed!");
        return;
    }
    while (true)
    {
        m_mutex.lock();
        while (m_tasks.empty())
            m_cond.wait(&m_mutex);
        Task* task = m_tasks.front();
        m_tasks.pop_front();
        m_mutex.unlock();
        handle(task);
    }
}
