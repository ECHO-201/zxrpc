#ifndef UPPASSWD_H
#define UPPASSWD_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class upPasswd;
}

class upPasswd : public QWidget
{
    Q_OBJECT

public:
    explicit upPasswd(QWidget *parent = nullptr);
    ~upPasswd();

private slots:
    void on_update_clicked();
    void on_check_clicked();

private:
    Ui::upPasswd *ui;
    QTcpSocket *socket;
};

#endif // UPPASSWD_H
