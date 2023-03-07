#pragma once

#include <list>
#include <string>
#include "Socket.h"
#include "EventPoller.h"
#include "ObjectPool.h"
#include "Mutex.h"
#include "Task.h"

using namespace zxrpc::tools;
using namespace zxrpc::thread;
using std::string;
using std::list;

namespace zxrpc {
    namespace socket {

        class SocketHandler
        {
        public:
            SocketHandler();
            ~SocketHandler();

            void listen(const string & ip, int port);
            void attach(Socket * socket);
            void detach(Socket * socket);
            void remove(Socket * socket);
            void handle(int max_connections, int wait_time);
            void handleMain(int max_connections, int wait_time);

        private:
            EventPoller * m_epoll;
            Socket * m_server;
            ObjectPool<Socket> m_sockpool;
            Mutex m_mutex;
        };
    }
}
