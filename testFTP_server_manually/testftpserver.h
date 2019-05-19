#ifndef TESTFTPSERVER_H
#define TESTFTPSERVER_H

#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <string>
#include <iostream>
using namespace std;
class testFTPServer : public QTcpSocket{
private slots:
    void readSocketData(){
        QByteArray b = readAll();
        qDebug() << socketDescriptor() << ':' << QString(b);
        string str;
        cout << ">>";
        getline(cin,str);
        write((str+"\n\n").c_str());
    }
public slots:
    void writeD(const char *s){
        write(s);
    }
public:
    testFTPServer(){
        connectToHost("127.0.0.1",6666);
        connect(this,&QTcpSocket::readyRead,this,&testFTPServer::readSocketData);
    }
};


#endif // TESTFTPSERVER_H
