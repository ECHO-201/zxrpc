#include "logpage.h"
#include "ui_logpage.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QButtonGroup>
#include "singleton.h"
#include <QDebug>


logpage::logpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logpage)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    socket = Singleton<QTcpSocket>::instance();
    getTable();

    ui->tableWidget_2->setColumnCount(2); //设置列数
    connect(ui->query, &QPushButton::clicked, this, &logpage::getTable);
    connect(socket, &QTcpSocket::readyRead, this, &logpage::setTable);
    connect(ui->del_2, &QPushButton::clicked, this, &logpage::deleteLog);
}

logpage::~logpage()
{
    delete ui;
}

void logpage::getTable(){

    QJsonObject jsonObject;
       jsonObject.insert("function", "get_log");

    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();
       socket->write(dataArray);
}

void logpage::setTable(){
    QByteArray data;
    data = socket->readAll();
    QString str = tr(data);
    str = str.simplified();
    int temp = 0;
    if(str == "0"){
        return;
        //QMessageBox::information(NULL, "提示", "添加失败,课程id"+ui->id_2->text()+"有误", QMessageBox::Ok);
    }else{
        for(int i=0; i<str.size(); ++i){
            if(str[i]=='|'){
                ++temp;
            }
        }

        ui->tableWidget_2->setRowCount(temp/2);    //设置行数
        QStringList header;
        QStringList list = str.split("|");
        header<<"id"<<"record";
        ui->tableWidget_2->setHorizontalHeaderLabels(header);
        ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
        ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget_2->setColumnWidth(0, 50);
        ui->tableWidget_2->setColumnWidth(1, 600);
        int p = 0;
        for(int i = 0; i < temp / 2; ++i){
           ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(list[p++]));
           QTableWidgetItem *item = new QTableWidgetItem(list[p++]);
           item->setTextAlignment(Qt::AlignCenter);
           ui->tableWidget_2->setItem(i,1,item);
        }
        ui->tableWidget_2->show();
    }
};

void logpage::deleteLog(){
    int n = ui->tableWidget_2->currentIndex().row();
    QString str = ui->tableWidget_2->item(n, 0)->text();
    QJsonObject jsonObject;
       jsonObject.insert("function", "del_log");
       jsonObject.insert("log_id", str);
    QJsonDocument jsonDocument;
       jsonDocument.setObject(jsonObject);
       QByteArray dataArray = jsonDocument.toJson();
       socket->write(dataArray);
};
