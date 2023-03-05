#include "WorkerThread.h"

#include "Logger.h"
#include "Singleton.h"
using namespace zxrpc::tools;

#include "Task.h"
#include "ThreadPool.h"
using namespace zxrpc::thread;

WorkerThread::WorkerThread() : Thread()
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::cleanup(void* ptr)
{
    LOG_INFO("worker thread cleanup handler: %p", ptr);
}

void WorkerThread::run()
{
    sigset_t mask;
    if (0 != sigfillset(&mask))
    {
        LOG_ERROR("worker thread sigfillset failed!");
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
    {
        LOG_ERROR("worker thread pthread_sigmask failed");
    }
    pthread_cleanup_push(cleanup, this);// 销毁时的回调

    while (true)
    {
        // 等待任务
        m_mutex.lock();
        while (m_task == NULL)
            m_cond.wait(&m_mutex);
        m_mutex.unlock();

        // 初始化状态
        int rc = 0;
        int old_state = 0;

        rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state); // 设置为忙碌线程

        m_task->run();
        m_task->destroy();
        m_task = NULL;

        Singleton<ThreadPool>::instance()->move_to_idle_list(this);

        rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state); // 设置为空闲线程
        pthread_testcancel(); // linux的cancel-point
    }
    pthread_cleanup_pop(1);
}
