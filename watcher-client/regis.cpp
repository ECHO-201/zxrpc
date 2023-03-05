#include "regis.h"
#include "ui_regis.h"
#include "singleton.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>
#include <network.h>

regis::regis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regis)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    socket = Singleton<QTcpSocket>::instance();


    connect(ui->save, &QPushButton::clicked, this, &regis::on_save_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &regis::on_check_clicked);
}

regis::~regis()
{
    delete ui;
}

void regis::on_save_clicked(){
    QString id = this->ui->id->text();
    QString name = this->ui->name->text();
    QString passwd = this->ui->passwd->text();
    QString passwd_2 = this->ui->passwd_2->text();
    QString phone = this->ui->phone->text();

    if(passwd != passwd_2){
        QMessageBox::information(NULL, "提示", "输入的密码不一致", QMessageBox::Ok);
        return;
    }

    network *net = Singleton<network>::instance();

    QByteArray md5 = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5);
    QJsonObject jsonObject;
       jsonObject.insert("function", "regis");
       jsonObject.insert("name", name);
       jsonObject.insert("id", id);
       jsonObject.insert("passwd", QString(md5));
       jsonObject.insert("phone", phone);
       jsonObject.insert("mac_addr", net->getMac());

    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();

    socket->write(dataArray);
}

void regis::on_check_clicked(){
    QByteArray data;
    data = socket->readAll();
    QString str = tr(data);
    qDebug() << str;
    QByteArray buf = str.toUtf8();
    QJsonDocument jd = QJsonDocument::fromJson(buf);
    QJsonObject jo = jd.object();

    if(jo["status"] == "succ"){
        QMessageBox::information(NULL, "提示", "注册成功", QMessageBox::Ok);
        this->close();
    }else{
        QMessageBox::information(NULL, "提示", "注册失败, 账号已存在", QMessageBox::Ok);
    }
}
