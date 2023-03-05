#include "ThreadPool.h"
#include "WorkerThread.h"
#include "Logger.h"


using namespace zxrpc::tools;
using namespace zxrpc::thread;

ThreadPool::ThreadPool() : m_thread_size(0)
{
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::create(int thread_size)
{
    AutoLock lock(&m_mutex_idle);
    m_thread_size = thread_size;
    for (int i = 0; i < thread_size; ++i)
    {
        Thread* thread = new WorkerThread();
        LOG_DEBUG("create thread is %p", thread);
        m_list_idle.insert(thread);
        thread->start();
    }
}

Thread* ThreadPool::get_idle_thread()
{
    AutoLock lock(&m_mutex_idle);
    while (m_list_idle.size() == 0)
        m_cond_idle.wait(&m_mutex_idle);
    return *m_list_idle.begin();
}

void ThreadPool::move_to_idle_list(Thread *thread)
{
    m_mutex_idle.lock();
    m_list_idle.insert(thread);
    m_cond_idle.signal();
    m_mutex_idle.unlock();

    m_mutex_busy.lock();
    std::set<Thread *>::iterator it = m_list_busy.find(thread);
    if (it != m_list_busy.end())
        m_list_busy.erase(it);
    m_cond_busy.signal(); // 发送信号，wait接收
    m_mutex_busy.unlock();
}

void ThreadPool::move_to_busy_list(Thread* thread)
{
    m_mutex_busy.lock();
    while (m_list_busy.size() == (size_t)(m_thread_size))
        m_cond_busy.wait(&m_mutex_busy);
    m_list_busy.insert(thread);
    m_mutex_busy.unlock();

    m_mutex_idle.lock();
    std::set<Thread *>::iterator it = m_list_idle.find(thread);
    if (it != m_list_idle.end())
        m_list_idle.erase(it);
    m_mutex_idle.unlock();
}

int ThreadPool::get_idle_thread_numbers()
{
    AutoLock lock(&m_mutex_idle);
    return m_list_idle.size();
}

int ThreadPool::get_busy_thread_numbers()
{
    AutoLock lock(&m_mutex_busy);
    return m_list_busy.size();
}

void ThreadPool::assign(Task *task)
{
    if (task == NULL)
    {
        LOG_ERROR("assign a null task to thread pool");
        return;
    }
    LOG_DEBUG("assign a new task: %p to thread pool", task);

    m_mutex_busy.lock();
    while (m_list_busy.size() == (size_t)(m_thread_size))
        m_cond_busy.wait(&m_mutex_busy);
    m_mutex_busy.unlock();

    Thread* thread = get_idle_thread();
    if (thread != NULL)
    {
        move_to_busy_list(thread);
        thread->set_task(task);
    }
    else
    {
        LOG_ERROR("thread is null, assign a task failed");
    }
}
