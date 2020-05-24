#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "ipconnet.h"
#include "hardwarecheck.h"
#include "checkaccount.h"
#include <QTimer>
namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QTcpSocket &socket,QWidget *parent = nullptr);
    ~loginDialog();
    int admin;
    IpConnet *myip;
    QTcpSocket *tempSocket;
    hardwareCheck *h;
    checkAccount *c;
    QTimer *update_time;
    void identity(QString id,QString password);
private slots:

    void loginFunc();

    void readData();

    void on_buttonLogin_clicked();

    void on_buttonExit_2_clicked();

signals:
    void checkFunc(QByteArray data);
    void updateFunc(QByteArray data);

private:
    Ui::loginDialog *ui;
};

#endif // LOGINDIALOG_H
