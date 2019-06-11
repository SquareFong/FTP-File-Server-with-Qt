#include"getfile.h"

GetFile::GetFile():isReceiving(false),isSending(false){
    connect(this,&QTcpSocket::readyRead, this, &GetFile::readSocketData);
    connect(this,&QTcpSocket::bytesWritten,this, &GetFile::waitforRead);
}
void GetFile::waitforRead(){
    waitForReadyRead(3000);
}
void GetFile::readSocketData(){
    QByteArray b = readAll();
    if(b[0] == '2' ){
        if(isReceiving)
            write("DATA to test if server would send immediatly\n\n");
        else if(isSending){
            QFile file(fileName);
            if(file.open(QIODevice::ReadOnly)){
                while (!file.atEnd()) {
                    QByteArray datas=file.read(1000);
                    write("DATA " + datas);
                    waitForBytesWritten(1000);
                }
            }else {
                write("500 fail to open the file\n\n");
            }
        }
    }else {
        if(isReceiving){
            if(b.mid(b.length()-9) != QByteArray("DATAEND\n\n")){
                QFile file(fileName);
                if(file.open(QIODevice::ReadWrite|QIODevice::Append))
                {
                    file.write(b);
                    file.close();
                }
                else {
                    qDebug() << "write fail" ;
                }
            }
            else {
                QFile file(fileName);
                if(file.open(QIODevice::ReadWrite|QIODevice::Append))
                {
                    file.write(b.mid(0,b.length()-9));
                    file.close();
                }
                else {
                    qDebug() << "write fail" ;
                }
                qDebug() << "Finished receiving: " << fileName;
            }
        }
        else{
            qDebug() << "null else if branch";
        }
        /*
        else if(b[0] == '2'){
            write("DATA\n\n");
            isReceiving = true;
        }*/
    }
    waitForBytesWritten(1000);
}

void GetFile::addDownloadTask_testGet(QHostAddress host, uint16_t port,QString token, QString fileName){
    connectToHost(host, port);
    waitForConnected(1000);
    remotePort = port;
    this->token = token;
    this->fileName = fileName;
    isReceiving=true;
    write(("TOKEN "+(token).toStdString() + "\n\n").c_str());
    flush();
    waitForReadyRead(1000);
    //qDebug() << host << remotePort << this->token << this->fileName;
}

void GetFile::addUploadTask(QHostAddress host, uint16_t port,QString token, QString fileName){
    connectToHost(host, port);
    waitForConnected(1000);
    remotePort = port;
    this->token = token;
    this->fileName = fileName;
    isSending=true;
    write(("TOKEN "+(token).toStdString() + "\n\n").c_str());
    flush();
    waitForReadyRead(1000);
}
