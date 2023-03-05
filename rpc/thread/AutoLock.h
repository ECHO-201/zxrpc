#pragma once

#include "Mutex.h"

namespace zxrpc {
namespace thread {

class AutoLock
{
public:
    AutoLock(Mutex* mutex);

    /**
     * @brief 销毁时解锁
     */
    ~AutoLock();

private:
    Mutex* m_mutex;
};

}}
