#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <string>
#include <QtDebug>
#include <QByteArray>
#include <unistd.h>
using namespace std;
class MyTcpClient : public QObject{
private:
    QTcpSocket *socket;
    int No=0;
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
        usleep(1000000);
        socket->write(to_string(++No).c_str());
    }


    void socket_Disconnected(){
        qDebug() << "Disconnected";
    }

public:
//    MyTcpClient(){
//        socket=new QTcpSocket();
//    }
    ~MyTcpClient(){
        socket->disconnectFromHost();
        socket->close();
    }
    MyTcpClient(string hostAddress, uint16_t port){

        socket=new QTcpSocket(this);
        socket->connectToHost(QString(hostAddress.c_str()), port);
        //server->listen(QHostAddress(QString(hostAddress.c_str())), quint16(port));
        //连接信号槽
        connect(socket, &QTcpSocket::readyRead, this, &MyTcpClient::socket_Read_Data);
        connect(socket, &QTcpSocket::disconnected, this, &MyTcpClient::socket_Disconnected);
        qDebug() << "connected to " << QString(hostAddress.c_str()) << ":" << port;
    }

    void write_Data()
    {
            socket->write(QByteArray("this is a message from tcp client\n"));
            qDebug() << "message sended" ;
            //socket->disconnectFromHost();
    }

};

#endif // MYTCPCLIENT_H
