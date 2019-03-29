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
testFTPServer::testFTPServer(QObject *parent):QObject (parent){

}
testFTPServer::testFTPServer(string hostAddress, uint16_t port):socket(nullptr){

    socket=new QTcpSocket(this);
    socket->connectToHost(QString(hostAddress.c_str()), port);
    //server->listen(QHostAddress(QString(hostAddress.c_str())), quint16(port));
    //连接信号槽
    connect(socket, &QTcpSocket::readyRead, this, &testFTPServer::socket_Read_Data);
    connect(socket, &QTcpSocket::disconnected, this, &testFTPServer::socket_Disconnected);
    //connect(IT,&inputThread::ready,this,&testFTPServer::writeDataToSocket);

    connect(this, &testFTPServer::write_toSocket, this, &testFTPServer::write_DataToSocket);

    qDebug() << "connected to " << QString(hostAddress.c_str()) << ":" << port;
}

