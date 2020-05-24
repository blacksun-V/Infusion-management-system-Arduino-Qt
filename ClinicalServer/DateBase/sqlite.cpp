#include "sqlite.h"
#include <QDebug>

bool createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../my.db");
    if(!db.open()){
        //提示出错
            qDebug()<<db.lastError().text();
        return false;
    }

    QSqlQuery query;

    //user表包括医生和管理员，type（doctor/admin）,password为密码
    query.exec("create table user(userid varchar primary key,type varchar,password varchar)");
    query.exec(QString("insert OR IGNORE into user values('admin','admin','123456')"));
    query.exec(QString("insert OR IGNORE into user values('wss','doctor','123456')"));
    //doctor表为医生和其所控制的硬件的对应关系，userid 为医生id，hwflag为硬件标识
    //qDebug()<<"drop function is "<<query.exec("drop table doctor");
    qDebug()<<"create function is "<<query.exec("create table doctor(userid varchar,hwflag varchar,primary key(userid , hwflag))");
    qDebug()<<"insert function is "<<query.exec(QString("insert OR IGNORE into doctor values('wss','aa')"));
    qDebug()<<"insert function is "<<query.exec(QString("insert OR IGNORE into doctor values('wss','bb')"));
    //status表为硬件状态表,stop  0表示没有停止，1表示输液停止，connect 1表示
    //query.exec("drop table status");
    query.exec("create table status(hwflag varchar primary key,stop bool)");
    query.exec(QString("insert or ignore into status values('aa',0)"));
    query.exec(QString("insert or ignore into status values('bb',0)"));


    query.exec("select * from status");
    while(query.next())
    {

        qDebug()<<query.value(0).toString()<<" | "<<query.value(1).toString();
    }

    query.exec("select * from user");
    while(query.next())
    {
        qDebug()<<query.value(0).toString()<<" | "<<query.value(1).toString()<<" | "<<query.value(2).toString();
    }

    query.exec("select * from doctor");
    while(query.next())
    {
        qDebug()<<query.value(0).toString()<<" | "<<query.value(1).toString();
    }

    return true;
}
