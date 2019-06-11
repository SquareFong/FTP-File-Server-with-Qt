#include "master.h"

Master::Master(){
    file = new GetFile();
    t = new testFTPServer();
    connect(t,&testFTPServer::addDownloadTask_test,file, &GetFile::addDownloadTask_testGet);
    connect(t,&testFTPServer::addUploadTask_test,file,&GetFile::addUploadTask);
    connect(t,&testFTPServer::sendToken,this,&Master::sendToken);
    t->write("OK\n\n");
}

void Master::sendToken(QString token){
    file->write(("TOKEN "+(token).toStdString() + "\n\n").c_str());
}
