#include "ftpcontroller_server.h"

FTPControllerServer::FTPControllerServer(uint16_t port):
controllerPort(0),transmitterPort(0)
{
    listen(QHostAddress::Any,port);
    connect(this,&FTPControllerServer::newConnection,this,&FTPControllerServer::server_New_Connect);
}

void FTPControllerServer::newToken(int status, QString token){
    emit addToken(status,token);
}

void FTPControllerServer::newTask(QString token, QString filePath){
    emit addTask(token,filePath);
}
