#include "testftp_server.h"

testFTPServer::testFTPServer(QObject *parent):QObject (parent){

}
testFTPServer::testFTPServer(string hostAddress, uint16_t port):socket(nullptr){

    socket=new QTcpSocket(this);
    socket->connectToHost(QString(hostAddress.c_str()), port);
    //server->listen(QHostAddress(QString(hostAddress.c_str())), quint16(port));
    //连接信号槽
    connect(socket, &QTcpSocket::readyRead, this, &testFTPServer::socket_Read_Data);
    connect(socket, &QTcpSocket::disconnected, this, &testFTPServer::socket_Disconnected);
    //connect(IT,&inputThread::ready,this,&testFTPServer::writeDataToSocket);

    connect(this, &testFTPServer::write_toSocket, this, &testFTPServer::write_DataToSocket);

    qDebug() << "connected to " << QString(hostAddress.c_str()) << ":" << port;
}
