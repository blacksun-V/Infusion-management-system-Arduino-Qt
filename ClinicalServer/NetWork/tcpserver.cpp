#include "tcpserver.h"

tcpserver::tcpserver(QObject *parent) : QObject(parent)
{


    bool ok = tcpServer.listen(QHostAddress::Any,8080);
    while(ok!=1)
    {
        ok = tcpServer.listen(QHostAddress::Any,8080);
    }

    qDebug()<<"try to listen port 8080"<<endl;
    qDebug()<<"IP: "<<QNetworkInterface().allAddresses().at(1).toString();
    qDebug()<<"PORT: "<<tcpServer.serverPort();
    qDebug()<<"server listen succeed!"<<endl;

    connect(&tcpServer,SIGNAL(newConnection()),this,SLOT(NewConnectSlot()));

}

void tcpserver::sendDateToPcClinical(QTcpSocket* socket,QByteArray send)
{
    socket->write(send);
}

bool tcpserver::jsonParser(QByteArray &s,QTcpSocket *socket)//返回false代表断开，返回true代表继续连接
{
    QJsonParseError err;
    QJsonDocument root_doc = QJsonDocument::fromJson(s,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        qDebug()<<"json err";
        return true;
    }
    else
    {
        QJsonObject root_obj=root_doc.object();
        QJsonValue clinicalType=root_obj.value("clinical");
        if(clinicalType=="hw")//硬件端信息处理
        {
            QJsonValue flag = root_obj.value("flag");
            QJsonValue status = root_obj.value("hwStatus");
            if(status=="end")
            {
                qDebug()<<"hwstatus is end"<<"Passive disconnect"<<endl;
                //进行数据库操作
                if(db.isOpen())
                {

                    QSqlQuery query;
                    QString check = tr("select * from status where hwflag='%1'").arg(flag.toString());
                    query.exec(check);
                    query.last();
                    if(query.at()>=0)//检查是否有该器件在库中
                    {
                        QString sql=tr("update status set stop=1 where hwflag='%1'").arg(flag.toString());

                        bool ok=query.exec(sql);
                        qDebug()<<sql<<endl<<"ok is "<<ok<<endl;
                    }
                }
                else
                {
                    qDebug()<<"error: database connect is closed!"<<endl;
                }

                return false;
            }
            else if(status == "begin")
            {
                qDebug()<<"run status==begin ";
                if(db.isOpen())//数据库是打开
                {
                    QSqlQuery query;
                    QString check = tr("select * from status where hwflag='%1'").arg(flag.toString());
                    query.exec(check);
                    query.last();
                    if(query.at()<0)//检查是否有该器件在库中，如果没有则添加进来
                    {
                        qDebug()<<"new hardware first connect";
                        check=tr("insert OR IGNORE into status values('%1',0)").arg(flag.toString());
                        query.exec(check);
                    }
//                    else if(query.at()>=0)//如果有则更新
//                    {
//                        QString sql=tr("update status set stop=0 where hwflag='%1'").arg(flag.toString());
//                        query.exec(sql);
//                    }
                }
                else
                {
                    qDebug()<<"error: database connect is closed!"<<endl;
                }
                return true;
            }
        }
        else if(clinicalType=="pc")  //pc客户端信息处理
        {
            //先判断身份
            QJsonValue idV=root_obj.value("id");
            QJsonValue pwdV=root_obj.value("password");
            QJsonValue typeV=root_obj.value("type");
            if(!identity(idV.toString(),typeV.toString(),pwdV.toString()))//身份错误
            {
                return false;
            }
            else//身份正确
            {
                QJsonValue need=root_obj.value("need");
                if(typeV=="admin")
                {
                    if(need == "adduser")
                    {
                        QJsonValue newid=root_obj.value("newid");
                        QJsonValue newpassword=root_obj.value("newpassword");
                        QString check=tr("insert OR IGNORE into user values('%1','doctor','%2')")\
                                .arg(newid.toString()).arg(newpassword.toString());
                        QSqlQuery query;
                        query.exec(check);
                    }
                    else if(need == "delete")
                    {
                        QJsonValue deleteId = root_obj.value("deleteId");
                        QString check=tr("delete from user where userid = '%1' and type = 'doctor'").arg(deleteId.toString());
                        QSqlQuery query;
                        query.exec(check);
                    }
                    else if(need == "check")
                    {
                        QString check=tr("select userid from user where type = 'doctor'");
                        QSqlQuery query;
                        query.exec(check);
                        QJsonDocument document;
                        QJsonArray array;
                        while(query.next())
                        {
                            array.append(query.value(0).toString());
                        }
                        QJsonObject object;
                        object.insert("func","check");
                        object.insert("check",array);
                        document.setObject(object);
                        QByteArray send;
                        send=document.toJson();
                        sendDateToPcClinical(socket,send);
                    }
                    else if(need == "login")
                    {
                        QByteArray send;
                        send="{\"func\":\"login\",\"login\":\"right\"}";
                        sendDateToPcClinical(socket,send);
                    }
                    else
                    {
                        return true;
                    }
                }
                else if(typeV=="doctor")
                {
                    if(need == "insert")
                    {
                        QJsonValue insert=root_obj.value("insert");
                        QString check;
                        QSqlQuery query;
                        check=tr("insert or ignore into doctor values('%1','%2')")\
                                    .arg(idV.toString()).arg(insert.toString());
                        query.exec(check);

                        check = tr("insert or ignore into status values('%1',0)").arg(insert.toString());
                        query.exec(check);
                    }
                    else if(need == "delete")
                    {
                        QJsonValue deleteId = root_obj.value("delete");
                        QString check;
                        check = tr("delete from doctor where hwflag = '%1' and userid = '%2'")\
                                .arg(deleteId.toString()).arg(idV.toString());
                        qDebug()<<check;
                        QSqlQuery query;
                        query.exec(check);
                        QString sql=tr("update status set stop=0 where hwflag='%1'").arg(deleteId.toString());
                        query.exec(sql);
                    }
                    else if(need == "update")
                    {
                        QJsonDocument document;
                        QJsonObject object;
                        QJsonArray begin;
                        QJsonArray end;
                        QString check;
                        QByteArray send;
                        QSqlQuery query;
                        check = tr("select status.hwflag from doctor, status where doctor.hwflag=status.hwflag and status.stop=0 and doctor.userid='%1'")\
                                .arg(idV.toString());
                        qDebug()<<check<<endl;
                        query.exec(check);
                        while(query.next())
                        {
                            begin.append(query.value(0).toString());
                        }
                        check = tr("select status.hwflag from doctor, status where doctor.hwflag=status.hwflag and status.stop=1 and doctor.userid='%1'")\
                                .arg(idV.toString());
                        qDebug()<<check<<endl;
                        query.exec(check);
                        while(query.next())
                        {
                            end.append(query.value(0).toString());
                        }
                        object.insert("func","update");
                        object.insert("begin",begin);
                        object.insert("end",end);
                        document.setObject(object);
                        send = document.toJson();
                        sendDateToPcClinical(socket,send);
                    }
                    else if(need == "login")
                    {
                        QByteArray send;
                        send="{\"func\":\"login\",\"login\":\"right\"}";
                        sendDateToPcClinical(socket,send);
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            return true;
        }
    }
    return true;
}

bool tcpserver::identity(QString id,QString type,QString password)
{
    QSqlQuery query;
    QString check=tr("select type, password from user where userid ='%1'").arg(id);
    query.exec(check);
    query.next();
    if(query.value(0)==type && query.value(1)==password)
    {
        qDebug()<<"identity is ture!";
        return true;
    }
    else
    {
        qDebug()<<"identity is false!";
        return false;
    }
}

void tcpserver::NewConnectSlot()
{
    qDebug()<<"there is new connect";
    currentClient = tcpServer.nextPendingConnection();
    qDebug()<<"clinical IP :"<<currentClient->peerAddress()<<\
              ":"<<currentClient->peerPort()<<endl;
    tcpList.append(currentClient);
    connect(currentClient,SIGNAL(readyRead()),this,SLOT(ReadDate()));
    connect(currentClient,SIGNAL(disconnected()),this,SLOT(DisConnectSlot()));
}

void tcpserver::DisConnectSlot()
{
    for(int i=0;i<tcpList.length();i++)
    {
        if(tcpList[i]->state()==QAbstractSocket::UnconnectedState)
        {
            qDebug()<<tcpList[i]->peerAddress().toString()<<":"<<tcpList[i]->peerPort()<<"  Active disconnct"<<endl;
            tcpList.removeAt(i);
        }
    }
}

void tcpserver::ReadDate()
{
    for(int i=0;i<tcpList.length();i++)
    {
        QByteArray s = tcpList[i]->readAll();
        std::string str = s.toStdString();
        QString Qstr = QString::fromStdString(str);
        qDebug()<<Qstr<<endl;
        if(s.isEmpty())
        {
            continue;
        }
        else if(jsonParser(s,tcpList[i])==false)
        {
            tcpList[i]->disconnectFromHost();
            tcpList.removeAt(i);
        }
    }
}
