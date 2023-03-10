#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "logpage.h"
#include "service.h"
#include "info.h"
#include "uppasswd.h"

namespace Ui {
class mainpage;
}

class mainpage : public QWidget
{
    Q_OBJECT

public:
    explicit mainpage(QWidget *parent = nullptr);
    ~mainpage();
    void on_log_clicked();
    void on_service_clicked();
    void on_user_clicked();
    void getUser();
    void on_info_clicked();
    void on_upPasswd_clicked();

private:
    Ui::mainpage *ui;
    logpage* log;
    service* serv;
    info* inf;
    upPasswd* up;
    QTcpSocket *socket;
};

#endif // MAINPAGE_H
