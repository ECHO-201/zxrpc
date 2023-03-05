#include "mainpage.h"
#include "ui_mainpage.h"

mainpage::mainpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainpage)
{
    ui->setupUi(this);

    connect(ui->toLog, &QPushButton::clicked, this, &mainpage::on_log_clicked);
    connect(ui->toService, &QPushButton::clicked, this, &mainpage::on_service_clicked);
}

mainpage::~mainpage()
{
    delete log;
    delete serv;
    delete ui;
}

void mainpage::on_log_clicked(){
    log = new logpage;
    log->show();
}

void mainpage::on_service_clicked(){
    serv = new service;
    serv->show();
}
