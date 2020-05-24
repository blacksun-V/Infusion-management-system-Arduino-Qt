#ifndef HARDWARECREAT_H
#define HARDWARECREAT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class hardwareCreat;
}

class hardwareCreat : public QWidget
{
    Q_OBJECT

public:
    explicit hardwareCreat(QTcpSocket *socket, QWidget *parent = nullptr);
    ~hardwareCreat();
    QTcpSocket *tempSocket;
private slots:
    void on_confirm_clicked();

private:
    Ui::hardwareCreat *ui;
};

#endif // HARDWARECREAT_H
