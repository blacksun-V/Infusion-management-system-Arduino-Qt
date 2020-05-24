#include "checkaccount.h"
#include "ui_checkaccount.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#pragma execution_character_set("utf-8")
extern QString id,password;
checkAccount::checkAccount(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::checkAccount)
{
    ui->setupUi(this);
    tempSocket=socket;
    crA=new creatAcount(socket);

    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(6);
    QStringList header;
    header<<"已经注册的非管理员账户名称";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setShowGrid(true);
    setWindowTitle(tr("查看所有的用户"));
}

checkAccount::~checkAccount()
{
    delete ui;
}

void checkAccount::on_alter_clicked()
{
    crA->show();
}

void checkAccount::on_freshen_clicked()
{
    QString temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"admin\",\"need\":\"check\"}")\
            .arg(id).arg(password);
    QByteArray send=QByteArray::fromStdString(temp.toStdString());
    tempSocket->write(send);
}

void checkAccount::checkFunc(QByteArray data)
{
    //读取服务器返回的信息
    qDebug()<<QString::fromStdString(data.toStdString())<<endl;
    QJsonDocument root_doc;
    QJsonObject root_obj;
    QJsonParseError err;
    root_doc = QJsonDocument::fromJson(data,&err);
    root_obj = root_doc.object();
    QJsonArray check=root_obj.value("check").toArray();
    int lengthChecked=check.size();
    ui->tableWidget->setRowCount(lengthChecked);
    for(int i=0;i<lengthChecked;i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(check.at(i).toString()));
    }
}
