#include "master.h"

Master::Master(){
    file = new GetFile();
    t = new testFTPServer();
    connect(t,&testFTPServer::addDownloadTask_test,file, &GetFile::addDownloadTask_testGet);
    t->write("UA anonymous\n\n");
}
