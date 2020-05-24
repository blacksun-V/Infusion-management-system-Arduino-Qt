#include "checkaccount.h"
#include "hardwarecheck.h"
#include "logindialog.h"
#include <QApplication>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
QString id,password;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTcpSocket *socket;
    socket=new QTcpSocket();
    loginDialog dlg(*socket);//登陆并初始化socket的值

    if(dlg.exec()==QDialog::Accepted){
        return a.exec();
    }

}
