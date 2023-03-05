#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "logpage.h"
#include "service.h"

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
private:
    Ui::mainpage *ui;
    logpage* log;
    service* serv;
};

#endif // MAINPAGE_H
