#include "ipconnet.h"
#include "ui_ipconnet.h"
#include <QMessageBox>
#include <QFile>
#pragma execution_character_set("utf-8")
IpConnet::IpConnet(QTcpSocket &socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpConnet)
{
    ui->setupUi(this);
    tempSocket = &socket;
    setWindowTitle(tr("连接服务器"));
}

IpConnet::~IpConnet()
{
    delete ui;
}

void IpConnet::on_connect_clicked()
{
    //在此处添加连接服务器的代码，并自动保存上次成功连接时的IP和端口号
    QString ip=ui->IP->text();
    QString port=ui->port->text();

    tempSocket->connectToHost(ip,port.toInt());
    if (tempSocket->waitForConnected(1000))
    {
        QFile file("../hostIp.txt");
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        QString txt=tr("{\"ip\":\"%1\",\"port\":\"%2\"}").arg(ip).arg(port);
        file.write(txt.toUtf8());
        file.close();
        QDialog::accept();
    }
    else
    {
        QMessageBox::information(this,tr("未连接成功"),tr("请检查服务器IP地址和端口号是否正确"),QMessageBox::Ok);
    }
}

void IpConnet::on_exit_clicked()
{
    QDialog::reject();
}
