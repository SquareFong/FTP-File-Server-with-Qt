#ifndef FTPTRANSMITTER_SERVER_H
#define FTPTRANSMITTER_SERVER_H

#include"ftptransmitter_socket.h"
#include<QtNetwork/QTcpServer>

class FTPTransmitterServer : public QTcpServer{
private:
    vector<QString> tokens;
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
    void addToken(QString token){
        tokens.push_back(token);
    }
public:
    FTPTransmitterServer(uint16_t port){
        listen(QHostAddress::Any,port);
        connect(this,&FTPTransmitterServer::newConnection,this,&FTPTransmitterServer::server_New_Connect);
    }
};


#endif // FTPTRANSMITTER_SERVER_H
