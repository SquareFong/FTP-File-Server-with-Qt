#ifndef FTPSERVER_H
#define FTPSERVER_H

#include "mytcpserver.h"

class FTPServer:public QObject{
    MyTcpServer control;
    MyTcpServer data;
public:
    FTPServer():control("127.0.0.1",21),data("127.0.0.1",20){

    }
};

#endif // FTPSERVER_H
