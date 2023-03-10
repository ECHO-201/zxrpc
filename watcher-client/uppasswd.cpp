#include "uppasswd.h"
#include "ui_uppasswd.h"
#include "singleton.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include "user.h"

upPasswd::upPasswd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::upPasswd)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    socket = Singleton<QTcpSocket>::instance();
    User *user = Singleton<User>::instance();
    ui->ids->setText(user->getId());

    connect(ui->update, &QPushButton::clicked, this, &upPasswd::on_update_clicked);
    connect(socket, &QTcpSocket::readyRead, this, &upPasswd::on_check_clicked);
}

upPasswd::~upPasswd()
{
    delete ui;
}

void upPasswd::on_update_clicked(){
    QString passwd = ui->passwd->text();
    QString passwd_2 = ui->passwds->text();

    if(passwd != passwd_2){
        QMessageBox::information(NULL, "提示", "输入的密码不一致", QMessageBox::Ok);
        return;
    }


    QByteArray md5 = QCryptographicHash::hash(passwd.toUtf8(), QCryptographicHash::Md5);
    QJsonObject jsonObject;
       jsonObject.insert("function", "up_passwd");
       jsonObject.insert("passwd", QString(md5));

    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();

    socket->write(dataArray);
}

void upPasswd::on_check_clicked(){
    QByteArray data;
    data = socket->readAll();
    QString str = tr(data);
    qDebug() << str;
    QByteArray buf = str.toUtf8();
    QJsonDocument jd = QJsonDocument::fromJson(buf);
    QJsonObject jo = jd.object();

    if(jo["status"] == "succ"){
        QMessageBox::information(NULL, "提示", "修改成功", QMessageBox::Ok);
        this->close();
    }else{
        QMessageBox::information(NULL, "提示", "修改失败", QMessageBox::Ok);
    }
}
