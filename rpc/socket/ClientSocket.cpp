#include "ClientSocket.h"
#include "Logger.h"

using namespace zxrpc::tools;
using namespace zxrpc::socket;

#include <cerrno>
#include <cstring>
using namespace std;

ClientSocket::ClientSocket() : Socket()
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        LOG_ERROR("create client socket error: errno=%d errstr=%s", errno, strerror(errno));
    }
}

ClientSocket::ClientSocket(const string& ip, int port) : Socket(ip, port)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        LOG_ERROR("create client socket error: errno=%d errstr=%s", errno, strerror(errno));
        return;
    }
    Socket::connect(ip, port);
}

ClientSocket::~ClientSocket()
{
    close();
}

