#ifndef IPCONNET_H
#define IPCONNET_H

#include <QDialog>
#include <QTcpSocket>
namespace Ui {
class IpConnet;
}

class IpConnet : public QDialog
{
    Q_OBJECT

public:
    explicit IpConnet(QTcpSocket &socket, QWidget *parent = nullptr);
    ~IpConnet();
    QTcpSocket *tempSocket;
private slots:
    void on_connect_clicked();

    void on_exit_clicked();

private:
    Ui::IpConnet *ui;
};

#endif // IPCONNET_H
