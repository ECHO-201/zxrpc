#include "network.h"
#include <QNetworkInterface>
network::network()
{

}

void network::setAddress(){
    QList<QNetworkInterface> netwk = QNetworkInterface::allInterfaces();
    for(int i = 0; i < netwk.count(); ++i)
    {
        if(netwk[i].flags().testFlag(QNetworkInterface::IsUp) &&
                netwk[i].flags().testFlag(QNetworkInterface::IsRunning)
                && !netwk[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            macAddress = netwk[i].hardwareAddress();
            break;
        }
    }

}

QString network::getMac(){
    return macAddress;
}

