#include <QCoreApplication>
#include <tcpserver.h>
#include <QtSql>
QSqlDatabase db;//全局变量在类的内部也可以访问
#include <sqlite.h>
#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    createConnection();
    tcpserver m;

    a.exec();
    db.close();
    db.~QSqlDatabase();
    return 0;
}
