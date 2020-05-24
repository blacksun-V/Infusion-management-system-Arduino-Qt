#ifndef HARDWARECHECK_H
#define HARDWARECHECK_H

#include <QWidget>
#include "hardwarecreat.h"
#include <QTcpSocket>

#include <QMediaPlayer>
namespace Ui {
class hardwareCheck;
}

class hardwareCheck : public QWidget
{
    Q_OBJECT

public:
    explicit hardwareCheck(QTcpSocket *socket, QWidget *parent = nullptr);
    ~hardwareCheck();

    hardwareCreat *hdC;
    QTcpSocket *tempSocket;
    QMediaPlayer *player;

    void readjson(QString flag,QString &name,QString &textEdit);
private slots:

    void on_alter_clicked();


    void updateFunc(QByteArray data);


    void on_pushButton_clicked();

    void timeUpdate();

private:
    Ui::hardwareCheck *ui;
};

#endif // HARDWARECHECK_H
