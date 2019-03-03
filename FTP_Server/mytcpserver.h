#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QThread>
#include <vector>
#include <iostream>
#include "mytcpsocket.h"
using namespace std;

class MyTCPServer : public QTcpServer{
private:
    vector<MyTCPSocket*> ftpsocketList;
signals:
    //断开连接
    void disconnected(long long);
private slots:
    void server_New_Connect(){
        //获取客户端连接
        MyTCPSocket *socket = new MyTCPSocket(this->nextPendingConnection());
        //socket->setSocketDescriptor(server->nextPendingConnection())
        //连接QTcpSocket的信号槽，以读取新数据
        QObject::connect(socket, &MyTCPSocket::readRead, this, &MyTCPServer::socket_Read_Data);
        QObject::connect(socket, &MyTCPSocket::disconnected, this, &MyTCPServer::socket_Disconnected);
        QThread *t=new QThread(this);
        socket->moveToThread(t);
        t->start();
        qDebug() << "Client connected:" << socket->socketDescriptor() ;
        ftpsocketList.push_back(socket);
    }


    //为外部提供接收信号Qstring:数据，long long:tcp描述符
    void socket_Read_Data(QByteArray buffer, long long socketDescriptor){
        //qDebug() << socketDescriptor << ':' << buffer;

        QByteArray::iterator it=buffer.begin();
        while (it != buffer.end()) {
            if(*it != 0){
                cout << char(*it);
                ++it;
            }
            else {
                cout << endl;
                break;
            }
        }

        for(unsigned i=0; i<ftpsocketList.size();  ++i){
            if(ftpsocketList[i]->socketDescriptor() == socketDescriptor){
                ftpsocketList[i]->writedata("message received");
            }
        }
        qDebug() << socketDescriptor << "answer sended";
        cout << endl;
    }

    void socket_Disconnected(long long socketDescriptor){
        qDebug() << socketDescriptor << "disconnected";
    }
public:
    MyTCPServer(QObject *parent, uint16_t port):QTcpServer (parent){
        listen(QHostAddress::Any,port);
        connect(this,&MyTCPServer::newConnection,this,&MyTCPServer::server_New_Connect);
    }
    MyTCPServer(uint16_t port){
        listen(QHostAddress::Any,port);
        connect(this,&MyTCPServer::newConnection,this,&MyTCPServer::server_New_Connect);
    }
    ~MyTCPServer(){

    }
};


#endif // MYTCPSERVER_H
