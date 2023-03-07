#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QButtonGroup>
#include "singleton.h"
#include <QDebug>
#include "config_file.h"
#include "network.h"
#include <QByteArray>
#include <QCryptographicHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = Singleton<QTcpSocket>::instance();
    socket->abort();
    socket->connectToHost(ip, port);

    connect(ui->login, &QPushButton::clicked, this,&MainWindow::on_login_clicked);
    connect(ui->register_, &QPushButton::clicked, this,&MainWindow::on_reg_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_clicked()
{
    QString userName = this->ui->userName->text();
    QByteArray md5 = QCryptographicHash::hash(this->ui->password->text().toUtf8(), QCryptographicHash::Md5);

    network *net = Singleton<network>::instance();

    QJsonObject jsonObject;
       jsonObject.insert("function", "login");
       jsonObject.insert("id", userName);
       jsonObject.insert("passwd", QString(md5));
       jsonObject.insert("mac_addr", net->getMac());

    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::check);
    socket->write(dataArray);
}

void MainWindow::check(){
    QByteArray data;
    data = socket->readAll();
    QString str = tr(data);
    qDebug() << str;
    QByteArray buf = str.toUtf8();
    QJsonDocument jd = QJsonDocument::fromJson(buf);
    QJsonObject jo = jd.object();

    if (jo["status"] != "1")
    {
        QMessageBox::information(NULL, "提示", "登录错误！", QMessageBox::Ok);
        disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::check);
    }
    else
    {
        QMessageBox::information(NULL, "提示", "登录成功！", QMessageBox::Ok);
        disconnect(socket, &QTcpSocket::readyRead, this, &MainWindow::check);
        mp = new mainpage;
        mp->show();
        this->close();
    }
}

void MainWindow::on_reg_clicked(){
    reg = new regis;
    reg->show();
}
