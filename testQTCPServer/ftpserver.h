#ifndef FTPSERVER_H
#define FTPSERVER_H
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <vector>
#include "ftptcpsocket.h"
using namespace std;

class ftpserver : public QTcpServer{
private:
    vector<FtpSocket*> ftpsocketList;
signals:
    //为外部提供接收信号Qstring:数据，long long:tcp描述符
    void readRead(QByteArray, long long);
    //断开连接
    void disconnected(long long);
private slots:
    void server_New_Connect(){
        //获取客户端连接
        FtpSocket *socket = new FtpSocket(this->nextPendingConnection());
        //socket->setSocketDescriptor(server->nextPendingConnection())
        //连接QTcpSocket的信号槽，以读取新数据
        QObject::connect(socket, &FtpSocket::readRead, this, &ftpserver::socket_Read_Data);
        QObject::connect(socket, &FtpSocket::disconnected, this, &ftpserver::socket_Disconnected);
        qDebug() << "Client connected:" << socket->socketDescriptor() ;
        ftpsocketList.push_back(socket);
    }


    void socket_Read_Data(QByteArray buffer, long long socketDescriptor){
        qDebug() << socketDescriptor << ':' << buffer;
        for(unsigned i=0; i<ftpsocketList.size();  ++i){
            if(ftpsocketList[i]->socketDescriptor() == socketDescriptor){
                ftpsocketList[i]->writedata("message received");
            }
        }
        qDebug() << socketDescriptor << "answer sended";

    }

    void socket_Disconnected(long long socketDescriptor){
        qDebug() << socketDescriptor << "disconnected";
    }
public:
    ftpserver(QObject *parent, uint16_t port):QTcpServer (parent){
        listen(QHostAddress::Any,port);
        connect(this,&ftpserver::newConnection,this,&ftpserver::server_New_Connect);
    }
    ftpserver(uint16_t port){
        listen(QHostAddress::Any,port);
        connect(this,&ftpserver::newConnection,this,&ftpserver::server_New_Connect);
    }
};

#endif // FTPSERVER_H
