#ifndef NETWORK_H
#define NETWORK_H

#include <QString>

class network
{
public:
    network();

    void setAddress();

    QString getMac();
private:
    QString macAddress;
};

#endif // NETWORK_H
