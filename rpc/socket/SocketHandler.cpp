#include "SocketHandler.h"
#include "Logger.h"
#include "config_file.h"
#include "Singleton.h"
#include "Task.h"
#include "AutoLock.h"
#include "TaskDispatcher.h"
#include "ServerSocket.h"
#include "TaskFactory.h"
using namespace zxrpc::task;
using namespace zxrpc::tools;
using namespace zxrpc::socket;
using namespace zxrpc::thread;

SocketHandler::SocketHandler()
{
}

SocketHandler::~SocketHandler()
{
    if (m_epoll != NULL)
    {
        delete m_epoll;
        m_epoll = NULL;
    }
    if (m_server != NULL)
    {
        delete m_server;
        m_server = NULL;
    }
}

void SocketHandler::listen(const string & ip, int port)
{
    m_server = new ServerSocket(ip, port);
}

void SocketHandler::attach(Socket * socket)
{
    AutoLock lock(&m_mutex);
    m_epoll->add(socket->m_sockfd, (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::detach(Socket * socket)
{
    AutoLock lock(&m_mutex);
    m_epoll->del(socket->m_sockfd, (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::remove(Socket * socket)
{
    socket->close();
    m_sockpool.release(socket);
}

void SocketHandler::handle(int max_connections, int wait_time)
{
    m_epoll = new EventPoller(false);
    m_epoll->create(max_connections);
    m_epoll->add(m_server->m_sockfd, m_server, (EPOLLIN | EPOLLHUP | EPOLLERR));
    m_sockpool.init(max_connections);

    LOG_DEBUG("epoll wait time: %d ms", wait_time);
    while (true)
    {
        int num = m_epoll->wait(wait_time);
        if (num == 0)
        {
            // debug("no events");
            continue;
        }
        for (int i = 0; i < num; ++i)
        {
            if (m_server == static_cast<Socket *>(m_epoll->m_events[i].data.ptr))
            {
                LOG_DEBUG("socket accept event");
                int soctfd = m_server->accept();
                Socket * socket = m_sockpool.allocate();
                if (socket == NULL)
                {
                    LOG_DEBUG("socket pool is empty");
                    break;
                }
                socket->m_sockfd = soctfd;
                socket->set_non_blocking();
                attach(socket);
            }
            else
            {
                Socket *socket = static_cast<Socket *>(m_epoll->m_events[i].data.ptr);
                if (m_epoll->m_events[i].events & EPOLLHUP)
                {
                    LOG_DEBUG("socket %d closed by peer.", socket->m_sockfd);
                    detach(socket);
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLERR)
			    {
                    LOG_DEBUG("socket %d error.", socket->m_sockfd);
                    detach(socket);
                    remove(socket);
			    }
                else if (m_epoll->m_events[i].events & EPOLLIN)
                {
                    LOG_DEBUG("socket read event");
                    detach(socket);
                    Task *task = TaskFactory::create(socket);
                    Singleton<TaskDispatcher>::instance()->assign(task);
                }
            }
        }
    }
}

void SocketHandler::handleMain(int max_connections, int wait_time)
{
    m_epoll = new EventPoller(false);
    m_epoll->create(max_connections);
    m_epoll->add(m_server->m_sockfd, m_server, (EPOLLIN | EPOLLHUP | EPOLLERR));
    m_sockpool.init(max_connections);

    LOG_DEBUG("epoll wait time: %d ms", wait_time);
    while (true)
    {
        int num = m_epoll->wait(wait_time);
        if (num == 0)
        {
            // debug("no events");
            continue;
        }
        for (int i = 0; i < num; ++i)
        {
            if (m_server == static_cast<Socket *>(m_epoll->m_events[i].data.ptr))
            {
                LOG_DEBUG("socket accept event");
                int soctfd = m_server->accept();
                Socket * socket = m_sockpool.allocate();
                if (socket == NULL)
                {
                    LOG_DEBUG("socket pool is empty");
                    break;
                }
                socket->m_sockfd = soctfd;
                socket->set_non_blocking();
                attach(socket);
            }
            else
            {
                Socket *socket = static_cast<Socket *>(m_epoll->m_events[i].data.ptr);
                if (m_epoll->m_events[i].events & EPOLLHUP)
                {
                    LOG_DEBUG("socket %d closed by peer.", socket->m_sockfd);
                    detach(socket);
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLERR)
			    {
                    LOG_DEBUG("socket %d error.", socket->m_sockfd);
                    detach(socket);
                    remove(socket);
			    }
                else if (m_epoll->m_events[i].events & EPOLLIN)
                {
                    LOG_DEBUG("socket read event");
                    detach(socket);
                    Task *task = TaskFactory::createWork(socket);
                    Singleton<TaskDispatcher>::instance()->assign(task);
                }
            }
        }
    }
}