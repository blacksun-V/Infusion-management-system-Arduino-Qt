#include "hardwarecheck.h"
#include "ui_hardwarecheck.h"
#include "logindialog.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#pragma execution_character_set("utf-8")
extern QString id,password;
hardwareCheck::hardwareCheck(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hardwareCheck)
{
    ui->setupUi(this);
    tempSocket=socket;
    hdC=new hardwareCreat(socket);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(6);

    QStringList header;
    header<<"硬件标识"<<"该患者的输液状态"<<"患者的姓名"<<"备注信息";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setShowGrid(true);
    setWindowTitle(tr("当前输液状态"));

    //语音提醒
    player=new QMediaPlayer;
//    QDir tempDir("./music.mp3");
//    QString dir=tempDir.absolutePath();
//    qDebug()<<"dir is :"<<dir<<endl;
    player->setMedia(QUrl::fromLocalFile("D:/WorkSpace/QT_Projects/ClinicalClient/music.mp3"));
    player->setVolume(50);
}

hardwareCheck::~hardwareCheck()
{
    delete ui;
}

void hardwareCheck::readjson(QString flag, QString &name, QString &textEdit)
{
    QFile file("../info.txt");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QJsonDocument root_doc;
    QJsonParseError err;
    QByteArray data=file.readAll();
    root_doc = QJsonDocument::fromJson(data,&err);
    if(err.error!=QJsonParseError::NoError&&!data.isEmpty())//如果json文档不是空的，但有错误
    {
        qDebug()<<"info.txt is wrong"<<err.error<<endl;
    }
    else
    {

        QJsonObject root_obj=root_doc.object();
        QJsonObject info_obj=root_obj.value(flag).toObject();
        QJsonValue tempName=info_obj.value("name");
        QJsonValue tempTextEdit=info_obj.value("textEdit");
        name=tempName.toString();
        textEdit=tempTextEdit.toString();
    }
    file.close();
}

void hardwareCheck::on_alter_clicked()
{
    hdC->show();
}

void hardwareCheck::updateFunc(QByteArray data)
{
    //读取服务器返回的信息，并显示
    qDebug()<<QString::fromStdString(data.toStdString())<<endl;
    QJsonDocument root_doc;
    QJsonArray begin;
    QJsonArray end;
    QJsonObject root_obj;
    QJsonParseError err;
    root_doc = QJsonDocument::fromJson(data,&err);
    root_obj = root_doc.object();
    QJsonValue temp=root_obj.value("begin");
    begin=temp.toArray();
    temp=root_obj.value("end");
    end=temp.toArray();
    int lengthEnd = end.size();
    if(lengthEnd>0)
    {
        player->play();
    }
    else
    {
        player->stop();
    }
    int lengthBegin = begin.size();
    ui->tableWidget->setRowCount(lengthEnd+lengthBegin);
    int i;
    QString name,textEdit;
    for(i=0;i<lengthEnd;i++)
    {

        ui->tableWidget->setItem(i,0,new QTableWidgetItem(end.at(i).toString()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem("已经完成输液"));
        readjson(end.at(i).toString(),name,textEdit);
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(name));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(textEdit));
    }
    for(int j=0;j<lengthBegin;j++)
    {
        ui->tableWidget->setItem(j+i,0,new QTableWidgetItem(begin.at(j).toString()));
        ui->tableWidget->setItem(j+i,1,new QTableWidgetItem("开始输液"));
        readjson(begin.at(j).toString(),name,textEdit);
        ui->tableWidget->setItem(j+i,2,new QTableWidgetItem(name));
        ui->tableWidget->setItem(j+i,3,new QTableWidgetItem(textEdit));
    }
}

void hardwareCheck::on_pushButton_clicked()//这是刷新的按钮的槽函数
{
    QString temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"doctor\",\"need\":\"update\"}")\
           .arg(id).arg(password);
    QByteArray send=QByteArray::fromStdString(temp.toStdString());
    tempSocket->write(send);
}

void hardwareCheck::timeUpdate()
{
    on_pushButton_clicked();
}
