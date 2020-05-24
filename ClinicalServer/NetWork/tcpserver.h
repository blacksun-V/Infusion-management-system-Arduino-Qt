#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QDebug>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QtSql>
//#include <sqlite.h>
extern QSqlDatabase db;
class tcpserver : public QObject
{
    Q_OBJECT
public:
    explicit tcpserver(QObject *parent = nullptr);
    void  sendDateToPcClinical(QTcpSocket* socket,QByteArray send);
    bool  jsonParser(QByteArray &s,QTcpSocket *socket);
    bool identity(QString id,QString type,QString password);
signals:

private:
    QTcpServer tcpServer;
    QTcpSocket *currentClient;
    QList<QTcpSocket*> tcpList;
private slots:
    void NewConnectSlot();
    void DisConnectSlot();
    void ReadDate();

};

#endif // TCPSERVER_H
