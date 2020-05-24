#include "hardwarecreat.h"
#include "ui_hardwarecreat.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#pragma execution_character_set("utf-8")
extern QString id,password;
hardwareCreat::hardwareCreat(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hardwareCreat)
{
    ui->setupUi(this);
    ui->flag->setText(tr("删除设备时只用填写该项"));
    tempSocket=socket;
    setWindowTitle(tr("添加新的硬件设备"));
}

hardwareCreat::~hardwareCreat()
{
    delete ui;
}

void hardwareCreat::on_confirm_clicked()
{
    QFile file("../info.txt");
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QJsonDocument root_doc;
    QJsonParseError err;
    QByteArray data=file.readAll();
    file.close();
    root_doc = QJsonDocument::fromJson(data,&err);
    if(err.error!=QJsonParseError::NoError&&!data.isEmpty())//如果json文档不是空的，但有错误
    {
        qDebug()<<"info.txt is wrong"<<err.error<<endl;
    }
    else
    {
        QJsonObject root_obj=root_doc.object();
        if(ui->checkBox->isChecked())
        {
            QString temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"doctor\",\"need\":\"delete\",\"delete\":\"%3\"}")\
                   .arg(id).arg(password).arg(ui->flag->text());
            QByteArray send=QByteArray::fromStdString(temp.toStdString());
            tempSocket->write(send);
            root_obj.remove(ui->flag->text());
            root_doc.setObject(root_obj);
            data=root_doc.toJson();
        }
        else
        {
            QString flag=ui->flag->text();
            QString temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"doctor\",\"need\":\"insert\",\"insert\":\"%3\"}")\
                   .arg(id).arg(password).arg(flag);
            QByteArray send=QByteArray::fromStdString(temp.toStdString());
            tempSocket->write(send);

            QJsonObject info_obj;
            QJsonValue name=ui->name->text();
            QJsonValue textEdit=ui->textEdit->toPlainText();
            info_obj.insert("name",name);
            info_obj.insert("textEdit",textEdit);

            root_obj.insert(flag,info_obj);
            root_doc.setObject(root_obj);
            data=root_doc.toJson();
        }
    }

    file.open(QIODevice::WriteOnly|QIODevice::Text);
    file.write(data);
    file.close();

    ui->flag->setText(tr("删除设备时只用填写该项"));
    ui->name->clear();
    ui->textEdit->clear();
    ui->checkBox->setCheckState(Qt::Unchecked);
    this->hide();
}
