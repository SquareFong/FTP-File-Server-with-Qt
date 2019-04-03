#pragma once

#include <QtNetwork/QTcpSocket>
#include <string>
#include <iostream>
#include <QtDebug>
#include <QObject>
using namespace std;

class testFTPServer : public QObject{
    Q_OBJECT
private:
    QTcpSocket *socket;
private slots:
    void socket_Read_Data()
    {
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
        if(!buffer.isEmpty())
        {
            QString str = "this is a tcp connect test";
            str = tr(buffer);
            //刷新显示
            qDebug() << str;
        }
    }


    void socket_Disconnected(){
        qDebug() << "Disconnected";
    }

    void write_DataToSocket(){QByteArray data;
        socket->write(data);
    }

signals:
    void write_toSocket();
public:

    testFTPServer(string hostAddress, uint16_t port);
    testFTPServer(QObject *parent=nullptr);
//    virtual ~testFTPServer(){
//        socket->disconnectFromHost();
//        socket->close();
//    }
};
