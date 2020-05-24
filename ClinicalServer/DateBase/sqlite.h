#ifndef SQLITE_H
#define SQLITE_H

#include <QtSql>
#include <QSqlError>
extern QSqlDatabase db;//db为全局变量，所以无论在类内部还是外部都是可以操作的
bool createConnection();

#endif // SQLITE_H
