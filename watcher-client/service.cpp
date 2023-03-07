#include "service.h"
#include "ui_service.h"
#include "singleton.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>

service::service(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::service)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    socket = Singleton<QTcpSocket>::instance();
    ui->tableWidget->setColumnCount(3); //设置列数
    getTable();

    connect(ui->query, &QPushButton::clicked, this, &service::getTable);
    connect(socket, &QTcpSocket::readyRead, this, &service::setTable);
    connect(ui->del, &QPushButton::clicked, this, &service::delNode);
}

service::~service()
{
    delete ui;
}

void service::getTable(){
    QJsonObject jsonObject;
       jsonObject.insert("function", "get_node");
       jsonObject.insert("path", "/");

    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();
       socket->write(dataArray);
}

void service::delNode(){
    int n = ui->tableWidget->currentIndex().row();
    QString str = ui->tableWidget->item(n, 0)->text();
    QJsonObject jsonObject;
       jsonObject.insert("function", "del_node");
       jsonObject.insert("serv_name", str);
    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();
       socket->write(dataArray);
}


void service::setTable(){
    QByteArray data;
    data = socket->readAll();
    QString str = tr(data);
    str = str.simplified();
    int temp = 0;
    if(str == "0"){
        return;
    }else{
        QJsonDocument jsonDocument;
        jsonDocument = QJsonDocument::fromJson(data);
        QJsonArray j_a = jsonDocument.array();
        QJsonObject j_o = j_a[0].toObject();
        ui->tableWidget->setRowCount(j_a.size());    //设置行数
        QStringList header;
        header<<"服务名"<< "ip" << "port";
        ui->tableWidget->setHorizontalHeaderLabels(header);
        ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget->setColumnWidth(2, 150);
        ui->tableWidget->setColumnWidth(3, 150);
        int p = 0;
        for(int i = 0; i < j_a.size(); ++i){
           QJsonObject j_o = j_a[i].toObject();
           ui->tableWidget->setItem(i, 0, new QTableWidgetItem(j_o["serv_name"].toString()));
           QTableWidgetItem *item = new QTableWidgetItem(j_o["ip"].toString());
           item->setTextAlignment(Qt::AlignCenter);
           ui->tableWidget->setItem(i, 1, item);
           QTableWidgetItem *item_port = new QTableWidgetItem(j_o["port"].toString());
           item->setTextAlignment(Qt::AlignCenter);
           ui->tableWidget->setItem(i, 2, item_port);
        }
        ui->tableWidget->show();
    }
};
