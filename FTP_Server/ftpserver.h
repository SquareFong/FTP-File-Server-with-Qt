#ifndef FTPSERVER_H
#define FTPSERVER_H

#include"mytcpserver.h"
#include"usermanager.h"

#include<QThread>
class FTPServer:public QObject{
    MyTCPServer *control;
    MyTCPServer *transfer;
    QThread *thread;
public:
    FTPServer(uint16_t controlPort, uint16_t transferPort):
        control(new MyTCPServer(controlPort)),
        transfer(new MyTCPServer(transferPort)),
        thread(nullptr){
        thread = new QThread(this);
        control->moveToThread(thread);
        thread->start();
    }
};

#endif // FTPSERVER_H
