#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>

class Socket
{
public:
    Socket();
    ~Socket();

    void write_data(QByteArray data);
    void connect();
    QByteArray read_data();
    QTcpSocket* get_sockfd();
private:
    QTcpSocket *socketfd;
};

#endif // SOCKET_H
