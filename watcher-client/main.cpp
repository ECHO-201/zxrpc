#include "mainwindow.h"
#include <QApplication>
#include "singleton.h"
#include "socket.h"
#include "network.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTcpSocket *socket = Singleton<QTcpSocket>::instance();
    network *net = Singleton<network>::instance();
    net->setAddress();

    w.show();
    return a.exec();
}
