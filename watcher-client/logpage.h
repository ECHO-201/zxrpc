#ifndef LOGPAGE_H
#define LOGPAGE_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class logpage;
}

class logpage : public QWidget
{
    Q_OBJECT

public:
    explicit logpage(QWidget *parent = nullptr);
    ~logpage();
    void setTable();
    void getTable();
    void deleteLog();
private:
    Ui::logpage *ui;
    QTcpSocket *socket;
};

#endif // LOGPAGE_H
