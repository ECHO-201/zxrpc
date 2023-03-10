#ifndef INFO_H
#define INFO_H

#include <QWidget>

namespace Ui {
class info;
}

class info : public QWidget
{
    Q_OBJECT

public:
    explicit info(QWidget *parent = nullptr);
    ~info();

    void setInfo();
private:
    Ui::info *ui;
};

#endif // INFO_H
