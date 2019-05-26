#ifndef FTPSERVER_H
#define FTPSERVER_H

#include"ftpcontroller_server.h"
#include"ftptransmitter_server.h"

#include<QThread>
class FTPServer:public QObject{
    Q_OBJECT
private:
    FTPControllerServer *control;
    FTPTransmitterServer *transmitter;
    //QThread *thread;
public:
    FTPServer(uint16_t controlPort, uint16_t transferPort):
        control(new FTPControllerServer(controlPort)),
        transmitter(new FTPTransmitterServer(transferPort))
        {
        control->setPorts(controlPort,transferPort);
        connect(control,&FTPControllerServer::addToken,transmitter,&FTPTransmitterServer::addToken);
        connect(control,&FTPControllerServer::addTask,transmitter,&FTPTransmitterServer::addTask);
    }
};

#endif // FTPSERVER_H
