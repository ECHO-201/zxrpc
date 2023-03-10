#ifndef UP_PASSWD_H
#define UP_PASSWD_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class up_passwd;
}

class up_passwd : public QWidget
{
    Q_OBJECT

public:
    explicit up_passwd(QWidget *parent = nullptr);
    ~up_passwd();

private slots:
    void on_update_clicked();
    void on_check_clicked();

private:
    Ui::up_passwd *ui;
    QTcpSocket *socket;
};

#endif // UP_PASSWD_H
