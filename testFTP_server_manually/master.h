#ifndef MASTER_H
#define MASTER_H

#include"inputmodule.h"
#include"testftpserver.h"
#include<QObject>
class Master :  public QObject{
    InputModule *inp;
    testFTPServer *t;
public:
    Master(){
        inp = new InputModule();
        t = new testFTPServer();
        inp->start();
        connect(inp,&InputModule::emessage,t,&testFTPServer::writeD);
    }
};

#endif // MASTER_H
