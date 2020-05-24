#include "creatacount.h"
#include "ui_creatacount.h"
#pragma execution_character_set("utf-8")
extern QString id,password;
creatAcount::creatAcount(QTcpSocket *socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::creatAcount)
{
    ui->setupUi(this);
    ui->newid->setText(tr("删除用户时只用填写该项！"));
    tempSocket=socket;
    setWindowTitle(tr("添加新的用户"));
}

creatAcount::~creatAcount()
{
    delete ui;
}

void creatAcount::on_confirm_clicked()
{
    if(!ui->checkBox->isChecked())
    {
        QString temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"admin\",\"need\":\"adduser\",\"newid\":\"%3\",\"newpassword\":\"%4\"}")\
               .arg(id).arg(password).arg(ui->newid->text()).arg(ui->newpassword->text());
        QByteArray send=QByteArray::fromStdString(temp.toStdString());
        tempSocket->write(send);
    }
    else
    {
        QString temp=tr("{\"clinical\":\"pc\",\"id\":\"%1\",\"password\":\"%2\",\"type\":\"admin\",\"need\":\"delete\",\"deleteId\":\"%3\"}")\
               .arg(id).arg(password).arg(ui->newid->text());
        QByteArray send=QByteArray::fromStdString(temp.toStdString());
        tempSocket->write(send);
    }
    ui->newid->setText(tr("删除用户时只用填写该项！"));
    ui->newpassword->clear();
    ui->checkBox->setCheckState(Qt::Unchecked);
    this->hide();
}
