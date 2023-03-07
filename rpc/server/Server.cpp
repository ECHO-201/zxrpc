#include "Server.h"
using namespace zxrpc::server;

#include "Singleton.h"
using namespace zxrpc::tools;

#include "TaskDispatcher.h"
using namespace zxrpc::thread;

#include "SocketHandler.h"
using namespace zxrpc::socket;

Server::Server() : m_ip(""), m_port(0), m_threads(1024), m_connects(1024)
{
}

Server::~Server()
{
}

void Server::listen(const string &ip, int port)
{
    m_ip = ip;
    m_port = port;
}

void Server::start()
{
    // 初始化线程池和消息队列
    TaskDispatcher *dispatcher = Singleton<TaskDispatcher>::instance(); // dispatcher事件分发
    dispatcher->init(m_threads);

    // 初始化socket句柄
    SocketHandler *socket_handler = Singleton<SocketHandler>::instance();
    socket_handler->listen(m_ip, m_port);
    socket_handler->handleMain(m_connects, m_wait_time);
}

void Server::set_threads(int threads)
{
    m_threads = threads;
}

void Server::set_connects(int connects)
{
    m_connects = connects;
}

void Server::set_waitTime(int wait_time)
{
    m_wait_time = wait_time;
}
