#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>
#include "ipconnet.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
extern QString id,password;
#pragma execution_character_set("utf-8")
loginDialog::loginDialog(QTcpSocket &socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    ui->setupUi(this);
    tempSocket=&socket;
    setWindowTitle(tr("登录"));
    admin=0;
    myip=new IpConnet(socket);
    h=new hardwareCheck(&socket);
    c=new checkAccount(&socket);
    connect(tempSocket,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(this,SIGNAL(checkFunc(QByteArray)),c,SLOT(checkFunc(QByteArray)));//更新账户名单
    connect(this,SIGNAL(updateFunc(QByteArray)),h,SLOT(updateFunc(QByteArray)));//更新输液管理名单
    update_time = new QTimer(this);
    connect(update_time,SIGNAL(timeout()),h,SLOT(timeUpdate()));

}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::identity(QString id, QString password)
{
    QByteArray send;
    QString temp;
    if(ui->checkBox->isChecked())
    {
        temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"admin\",\"need\":\"login\"}")\
                .arg(id).arg(password);
    }
    else
    {
        temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"doctor\",\"need\":\"login\"}")\
                .arg(id).arg(password);
    }
    send=QByteArray::fromStdString(temp.toStdString());
    tempSocket->write(send);
}

void loginDialog::loginFunc()
{
    if(ui->checkBox->isChecked())
    {
            //QMessageBox::information(this,tr("调试"),tr("进入判断"),QMessageBox::Ok);
            //2 的意思是管理员登陆
            QDialog::accept();
            c->show();
    }
    else
    {
            //用户权限
            QDialog::accept();
            update_time->start(3000);
            h->show();
    }
}

void loginDialog::readData()
{

    QByteArray data = tempSocket->readAll();
    QJsonDocument root_doc;
    QJsonArray check,begin,end;
    QJsonObject root_obj;
    QJsonValue func;
    QJsonParseError err;
    root_doc = QJsonDocument::fromJson(data,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        qDebug()<<"read data is wrong! error: "<<err.error<<endl;
    }
    else
    {
        root_obj = root_doc.object();
        func = root_obj.value("func");
        if(func == "check")
        {

            emit checkFunc(data);
        }
        else if(func == "update")
        {
            emit updateFunc(data);
        }
        else if(func == "login")
        {
            QJsonValue login=root_obj.value("login");
            if(login == "right")
            {
                qDebug()<<QString::fromStdString(data.toStdString())<<endl;
                loginFunc();
            }
        }
    }

}

void loginDialog::on_buttonLogin_clicked()
{
    if(ui->userName->text().isEmpty())
    {
        QMessageBox::information(this,tr("请输入用户名"),tr("请先输入用户名再登陆"),QMessageBox::Ok);
    }
    else if(ui->password->text().isEmpty())
    {
        QMessageBox::information(this,tr("请输入密码"),tr("请先输入密码再登陆"),QMessageBox::Ok);
    }
    else
    {
        //读取原来存储的IP和端口号,然后进行连接
        QFile file("../hostIp.txt");
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QByteArray s=file.readAll();
        file.close();
        if(file.size()>0)
        {
            QJsonParseError err;
            QJsonDocument root_doc = QJsonDocument::fromJson(s,&err);
            if(err.error!=QJsonParseError::NoError)
            {
                qDebug()<<"json err: "<<QString::fromStdString(s.toStdString())<<endl;
            }
            else
            {
                QJsonObject root_obj=root_doc.object();
                QJsonValue ip=root_obj.value("ip");
                QJsonValue port=root_obj.value("port");
                qDebug()<<"json ip is"<<ip.toString()<<"json port is"<<port.toString().toInt()<<endl;
                tempSocket->connectToHost(ip.toString(),port.toString().toInt());
                tempSocket->waitForConnected(1000);
            }
        }

        //如果没有存储过IP地址，或者直接连接不成，则直接进入填写ip的界面，并存储ip和直接连接
        qDebug()<<"state : "<<tempSocket->state()<<endl;
        if(tempSocket->state() == QAbstractSocket::UnconnectedState||file.size()==0)//如果没有连接IP,则进入IP+PORT获取页面,这里的代码还没有写呢
        {
            myip->exec();
        }
        if(tempSocket->state() == QAbstractSocket::ConnectedState)
        {
            identity(ui->userName->text(),ui->password->text());
            id=ui->userName->text();
            password=ui->password->text();
        }
    }
}

void loginDialog::on_buttonExit_2_clicked()
{
    QDialog::reject();
}
