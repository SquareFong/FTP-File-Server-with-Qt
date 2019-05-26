#include"getfile.h"

GetFile::GetFile():isReceiving(false){
    connect(this,&QTcpSocket::readyRead, this, &GetFile::readSocketData);
}

void GetFile::readSocketData(){
    QByteArray b = readAll();
    qDebug() << "DATA:" << b;
    qDebug() << "DATA END!";
    if(isReceiving == true){
        isReceiving = false;
        QFile file(fileName);
        file.write(b);
    }
    else if(b[0] == '2'){
        write("DATA\n\n");
        isReceiving = true;
    }

}

void GetFile::addDownloadTask_testGet(QHostAddress host, uint16_t port,QString token, QString fileName){
    connectToHost(host, port);
    remotePort = port;
    this->token = token;
    this->fileName = fileName;
    this->write((QString("TOKEN ").append(this->token).append("\n\n")).toUtf8());
    //qDebug() << host << remotePort << this->token << this->fileName;
}
