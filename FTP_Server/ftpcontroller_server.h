#ifndef FTPCONTROLLER_SERVER_H
#define FTPCONTROLLER_SERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QThread>
#include <vector>
#include <iostream>
#include "ftpcontroller_socket.h"
using namespace std;

class FTPControllerServer : public QTcpServer{
private:
    vector<FTPControllerSocket*> ftpsocketList;
    UserManager usermanager;
signals:
    //断开连接
    void disconnected(long long);

    void addToken(int status, QString token);

    void addTask(QString token, QString filePath);

private slots:
    void newToken(int status, QString token){
        emit addToken(status,token);
    }

    void newTask(QString token, QString filePath){
        emit addTask(token,filePath);
    }

    void server_New_Connect(){
        //获取客户端连接
        FTPControllerSocket *socket = new FTPControllerSocket(&usermanager,this->nextPendingConnection());
        //socket->setSocketDescriptor(server->nextPendingConnection())
        //连接QTcpSocket的信号槽，以读取新数据
        //QObject::connect(socket, &FTPControllerSocket::readData, this, &FTPControllerServer::socket_Read_Data);
        QObject::connect(socket, &FTPControllerSocket::disconnected, this, &FTPControllerServer::socket_Disconnected);

        QObject::connect(socket, &FTPControllerSocket::disconnected, this, &FTPControllerServer::socket_Disconnected);

        connect(socket,&FTPControllerSocket::addToken,this,&FTPControllerServer::newToken);
        connect(socket,&FTPControllerSocket::addTask,this,&FTPControllerServer::newTask);

        //        QThread *t=new QThread();
//        socket->moveToThread(t);
//        t->start();
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
//    FTPControllerServer(QObject *parent, uint16_t port):QTcpServer (parent){
//        listen(QHostAddress::Any,port);
//        connect(this,&FTPControllerServer::newConnection,this,&FTPControllerServer::server_New_Connect);
//    }
    FTPControllerServer(uint16_t port){
        listen(QHostAddress::Any,port);
        connect(this,&FTPControllerServer::newConnection,this,&FTPControllerServer::server_New_Connect);
    }
    ~FTPControllerServer(){

    }
};


#endif // FTPCONTROLLER_SERVER_H
