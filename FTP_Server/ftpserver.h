#ifndef FTPSERVER_H
#define FTPSERVER_H

#include"ftpcontroller_server.h"
#include"ftptransmitter_server.h"

#include<QThread>
class FTPServer:public QObject{
    FTPControllerServer *control;
    FTPTransmitterServer *transmitter;
    //QThread *thread;
public:
    FTPServer(uint16_t controlPort, uint16_t transferPort):
        control(new FTPControllerServer(controlPort)),
        transmitter(new FTPTransmitterServer(transferPort))
        {

    }
};

#endif // FTPSERVER_H
