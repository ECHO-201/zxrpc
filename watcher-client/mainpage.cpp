#include "mainpage.h"
#include "ui_mainpage.h"
#include "singleton.h"
#include "user.h"
#include <QJsonObject>
#include <QJsonDocument>

mainpage::mainpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainpage)
{
    ui->setupUi(this);

    getUser();

    connect(socket, &QTcpSocket::readyRead, this, &mainpage::on_user_clicked);
    connect(ui->toLog, &QPushButton::clicked, this, &mainpage::on_log_clicked);
    connect(ui->toService, &QPushButton::clicked, this, &mainpage::on_service_clicked);
    connect(ui->info, &QPushButton::clicked, this, &mainpage::on_info_clicked);
    connect(ui->upPasswd, &QPushButton::clicked, this, &mainpage::on_upPasswd_clicked);
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

void mainpage::on_info_clicked(){
    inf = new info;
    inf->show();
}

void mainpage::on_upPasswd_clicked(){
    up = new upPasswd;
    up->show();
}

void mainpage::on_user_clicked(){
    QByteArray data;
    data = socket->readAll();
    QString str = tr(data);
    qDebug() << str;
    QByteArray buf = str.toUtf8();
    QJsonDocument jd = QJsonDocument::fromJson(buf);
    QJsonObject jo = jd.object();

    User *user = Singleton<User>::instance();
    QString name = jo["name"].toString();
    user->setId(jo["id"].toString());
    user->setName(name);
    user->setPhone(jo["phone"].toString());

    ui->name->setText(name);

    disconnect(socket, &QTcpSocket::readyRead, this, &mainpage::on_user_clicked);
}

void mainpage::getUser(){
    socket = Singleton<QTcpSocket>::instance();

    QJsonObject jsonObject;
       jsonObject.insert("function", "get_user");

    QJsonDocument jsonDocument;
      jsonDocument.setObject(jsonObject);
      QByteArray dataArray = jsonDocument.toJson();


    socket->write(dataArray);
}
