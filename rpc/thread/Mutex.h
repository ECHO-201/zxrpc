#pragma once
#include <pthread.h>

namespace zxrpc {
namespace thread {

class Mutex
{
    friend class Condition;

public:
    Mutex();
    ~Mutex();

    int lock();

    /**
     * @brief 返回调用该锁的引用
     */
    int try_lock();

    int unlock();

private:
    pthread_mutex_t     m_mutex;
};

}}
