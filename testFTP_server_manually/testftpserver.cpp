#include "testftpserver.h"
#include<unistd.h>
testFTPServer::testFTPServer():isResolve(false),down(false),up(false){
    connectToHost("127.0.0.1",6666);
    hostName = QHostAddress("127.0.0.1");
    connect(this,&QTcpSocket::readyRead,this,&testFTPServer::readSocketData);
}

bool testFTPServer::resolveCommand(const QString &command, uint16_t &port, QString &token){
    if(isResolve){
        isResolve = false;
        if(command.at(0)==QChar('2')){
            QStringList temp = command.split(QChar(' '));
            auto it = temp.begin();
            ++it;
            QStringList tempToken = (*it).split(':');
            auto it2 = tempToken.begin();
            ++it2;
            ++it;
            QStringList tempPort = (*it).split(':');
            auto it3 = tempPort.begin();
            ++it3;
            port = uint16_t((*it3).toUInt());
            token = (*it2);
        }
        return true;
    }
    return false;
}

void testFTPServer::readSocketData(){
    QString b = readAll();
    qDebug() << socketDescriptor() << ':' << b;
    {
        uint16_t port;
        QString token;
        if(resolveCommand(b,port,token))
        {
            qDebug() << hostName << port << token << fileName;
            if(down) {
                emit addDownloadTask_test(hostName, port, token, fileName);
            }
            else if(up){
                emit addUploadTask_test(hostName, port, token, fileName);
            }
        }
    }

    string str;
    cout << ">>";
    getline(cin,str);
    if(str.substr(0,2) == "DL" || str.substr(0,2) == "UL") {
        if(str.substr(0,2) == "DL")
            down = true;
        else {
            up = true;
        }
        fileName = QString::fromStdString(str.substr(3));
        isResolve = true;
        qDebug() << "filename:" << fileName;
    }
    write((str+"\n\n").c_str());
}
