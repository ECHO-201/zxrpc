#include "info.h"
#include "ui_info.h"
#include "user.h"
#include "singleton.h"

info::info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::info)
{
    ui->setupUi(this);

    setInfo();
}

info::~info()
{
    delete ui;
}

void info::setInfo(){
    User *user = Singleton<User>::instance();
    ui->ids->setText(user->getId());
    ui->name->setText(user->getName());
    ui->phone->setText(user->getPhone());
}

