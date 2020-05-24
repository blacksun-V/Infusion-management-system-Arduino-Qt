#ifndef CREATACOUNT_H
#define CREATACOUNT_H

#include <QWidget>
#include <QTcpSocket>
namespace Ui {
class creatAcount;
}

class creatAcount : public QWidget
{
    Q_OBJECT

public:
    explicit creatAcount(QTcpSocket *socket, QWidget *parent = nullptr);
    ~creatAcount();
    QTcpSocket *tempSocket;
private slots:
    void on_confirm_clicked();

private:
    Ui::creatAcount *ui;
};

#endif // CREATACOUNT_H
