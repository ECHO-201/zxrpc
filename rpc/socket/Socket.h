#pragma once

#include <iostream>
using namespace std;

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <string>
using namespace std;

namespace zxrpc {
namespace socket {

class Socket
{
    friend class SocketHandler;

public:
    Socket();
    Socket(const string &ip, int port);
    virtual ~Socket();

    bool bind(const string &ip, int port);
    bool listen(int backlog);
    bool connect(const string &ip, int port);
    bool close();

    int accept();
    int recv(char * buf, int len);
    int send(const char * buf, int len);
    
    /**
     * @brief 设置为非阻塞
    */
    bool set_non_blocking();
    /**
     * @brief 发送缓冲区
    */
    bool set_send_buffer(int size);
    /**
     * @brief 接收缓冲区
    */
    bool set_recv_buffer(int size);
    /**
     * @brief 设置TCP断开的方式
     * @param active 优雅断开和强制断开两种方式
     * @param seconds close()不立刻返回,延迟 seconds
    */
    bool set_linger(bool active, int seconds);
    /**
     * @brief 保持连接
    */
    bool set_keep_alive();
    /**
     * @brief 地址复用
    */
    bool set_reuse_addr();
    /**
     * @brief 端口复用
    */
    bool set_reuse_port();

protected:
    string  m_ip;
    int     m_port;
    int     m_sockfd;
};

}}
