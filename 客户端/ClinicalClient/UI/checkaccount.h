#ifndef CHECKACCOUNT_H
#define CHECKACCOUNT_H

#include <QWidget>
#include <QTcpSocket>
#include "creatacount.h"
namespace Ui {
class checkAccount;
}

class checkAccount : public QWidget
{
    Q_OBJECT

public:
    explicit checkAccount(QTcpSocket *socket, QWidget *parent = nullptr);
    ~checkAccount();
    creatAcount *crA;
    QTcpSocket *tempSocket;

private slots:
    void on_alter_clicked();

    void on_freshen_clicked();

    void checkFunc(QByteArray data);
private:
    Ui::checkAccount *ui;
};

#endif // CHECKACCOUNT_H
