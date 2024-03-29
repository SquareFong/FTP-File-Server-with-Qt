#ifndef FTPTRANSMITTER_SERVER_H
#define FTPTRANSMITTER_SERVER_H

#include"ftptransmitter_socket.h"
#include<QtNetwork/QTcpServer>

class FTPTransmitterServer : public QTcpServer{
    Q_OBJECT
private:
    vector<TransmitTask> tokens;
    vector<FTPTransmitterSocket*> ftpsocketList;
private slots:
    void server_New_Connect(){
        FTPTransmitterSocket *socket = new FTPTransmitterSocket(tokens, this->nextPendingConnection());
        QObject::connect(socket, &FTPTransmitterSocket::disconnected, this, &FTPTransmitterServer::socket_Disconnected);
        qDebug() << "Client connected:" << socket->socketDescriptor() ;
        ftpsocketList.push_back(socket);
    }

    void socket_Disconnected(long long socketDescriptor){
        qDebug() << socketDescriptor << "disconnected";
    }
public slots:
    void addToken(int status, QString token){
        tokens.push_back(TransmitTask(status,token));
    }
    void addTask(QString token, QString filePath){
        auto it = tokens.begin();
        while(it != tokens.end()){
            if((*it).token == token){
                (*it).filesPath.push_back(filePath);
                return;
            }
        }
    }
public:
    FTPTransmitterServer(uint16_t port){
        listen(QHostAddress::Any,port);
        connect(this,&FTPTransmitterServer::newConnection,this,&FTPTransmitterServer::server_New_Connect);
    }
};


#endif // FTPTRANSMITTER_SERVER_H
