#ifndef REGIS_H
#define REGIS_H

#include <QWidget>
#include <QTcpSocket>


namespace Ui {
class regis;
}

class regis : public QWidget
{
    Q_OBJECT

public:
    explicit regis(QWidget *parent = nullptr);
    ~regis();

private slots:
    void on_save_clicked();
    void on_check_clicked();

private:
    Ui::regis *ui;
    QTcpSocket *socket;

};

#endif // REGIS_H
