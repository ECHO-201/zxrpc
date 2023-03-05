#ifndef SERVICE_H
#define SERVICE_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class service;
}

class service : public QWidget
{
    Q_OBJECT

public:
    explicit service(QWidget *parent = nullptr);
    ~service();

    void getTable();
    void setTable();
    void delNode();
private:
    Ui::service *ui;
    QTcpSocket *socket;
};

#endif // SERVICE_H
