#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QString>
#include <string>
#include <QThread>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QNetworkInterface>
#include <unistd.h>
using namespace std;

class MyTcpServer : public QObject{
private slots:

    void server_New_Connect(){
        //获取客户端连接
        socket = server->nextPendingConnection();
        //连接QTcpSocket的信号槽，以读取新数据
        QObject::connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::socket_Read_Data);
        QObject::connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::socket_Disconnected);

        qDebug() << server->serverAddress();
        qDebug() << "ID:" << ID << "A Client connected!";
    }


    void socket_Read_Data(){
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
        if(!buffer.isEmpty())
        {
            QString str;
            str = tr(buffer);
            qDebug() << "ID: " << ID << str;
        }
        usleep(1000000);
        socket->write(to_string(++No).c_str());

        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i) {
//            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
//                ipAddressesList.at(i).toIPv4Address()) {
//                ipAddress = ipAddressesList.at(i).toString();
//                break;
//            }
            qDebug() << ipAddressesList.at(i);
        }
    }

    void socket_Disconnected(){
        qDebug() << "ID: " << ID << "Disconnected";
    }
private:
    QTcpServer *server;
    QTcpSocket *socket;
    int ID;
    int No=0;
public:
    MyTcpServer(){
        server = new QTcpServer(this);
        //socket = new QTcpSocket();
        ID=-1;
    }
    MyTcpServer(int id, string hostAddress, uint16_t port):ID(id){
        server = new QTcpServer(this);
        //socket = new QTcpSocket();
        server->listen(QHostAddress::Any, quint16(port));
        //server->listen(QHostAddress(QString(hostAddress.c_str())), quint16(port));
        //连接信号槽
        connect(server,&QTcpServer::newConnection,this,&MyTcpServer::server_New_Connect);
        //connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::socket_Read_Data);
        qDebug() << "Listening to any address:" << port;
    }
    ~MyTcpServer(){
        server->close();
        delete server;
        //delete socket;
    }
    void Read_Data(){
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
        if(!buffer.isEmpty())
        {
            QString str;
            str = tr(buffer);
            qDebug() << "ID: " << ID << str;
        }
    }
};

#endif // MYTCPSERVER_H
