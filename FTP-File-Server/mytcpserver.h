#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H


#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class MyTcpServer:public QObject{
private slots:
    void server_New_Connect(){
        //获取客户端连接
        socket = server->nextPendingConnection();
        //连接QTcpSocket的信号槽，以读取新数据
        QObject::connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::socket_Read_Data);
        QObject::connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::socket_Disconnected);

        qDebug() << "A Client connected!";
    }
    void socket_Read_Data(){
        QByteArray buffer;
        //读取缓冲区数据
        buffer = socket->readAll();
        if(!buffer.isEmpty())
        {
            QString str;
            str = tr(buffer);
            qDebug() << "receive " << str;
        }
    }
    void socket_Disconnected(){
        qDebug() << "Disconnected";
    }
private:
    QTcpServer *server;
    QTcpSocket *socket;
public:
    MyTcpServer(){
        server = new QTcpServer();
        socket=new QTcpSocket();
    }
    MyTcpServer(QString hostAddress, unsigned short port){
        server = new QTcpServer();
        socket=new QTcpSocket();
        server->listen(QHostAddress(hostAddress), port);
        //连接信号槽
        connect(server,&QTcpServer::newConnection,this,&MyTcpServer::server_New_Connect);
        connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::socket_Read_Data);
    }
    ~MyTcpServer(){
        server->close();
        delete server;
        delete socket;
    }
};


#endif // MYTCPSERVER_H
