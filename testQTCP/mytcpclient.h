#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <string>
using namespace std;
class MyTcpClient : public QObject{
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

public:
    MyTcpClient(){
        socket=new QTcpSocket();
    }
    MyTcpClient(string hostAddress, uint16_t port){

        socket=new QTcpSocket();
        socket->connectToHost(QString(hostAddress.c_str()), port);
        //server->listen(QHostAddress(QString(hostAddress.c_str())), quint16(port));
        //连接信号槽
        connect(socket, &QTcpSocket::readyRead, this, &MyTcpClient::socket_Read_Data);
        connect(socket, &QTcpSocket::disconnected, this, &MyTcpClient::socket_Disconnected);
    }
};

#endif // MYTCPCLIENT_H
