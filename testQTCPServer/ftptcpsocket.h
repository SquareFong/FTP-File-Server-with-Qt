#ifndef FTPTCPSOCKET_H
#define FTPTCPSOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QString>
class FtpSocket:public QObject{
    Q_OBJECT

private:
    unsigned  bufferSize;
    QTcpSocket *socket;
signals:
    //为外部提供接收信号Qstring:数据，long long:tcp描述符
    void readRead(QByteArray, long long);
    void disconnected(long long);
private slots:
    void dataReceived()
    {
        unsigned i=0;
        /*bytesAvailable()表示有效数据*/
        QByteArray buff(int(bufferSize), 0);
        char temp;
        while(i<bufferSize && socket->bytesAvailable() > 0)
        {
            qDebug() << socket->bytesAvailable();
            temp = 0;
            socket->getChar(&temp);
            buff[i] = temp;
            ++i;
        }
        emit readRead(buff,socket->socketDescriptor());
    }

    void slotDisconnected()
    {
        emit disconnected(socket->socketDescriptor());
    }
public:
    FtpSocket(QTcpSocket *parent=nullptr):bufferSize(1024),socket(parent){
        //Q_UNUSED(parent);
        //readyRead()是QIODevice的signal，由QTcpSocket继承而来
        //QTcpSocket被看成一个QIODevice，readyRead()信号在有数据到来时发出。
        connect(socket,&QTcpSocket::readyRead,this, &FtpSocket::dataReceived);

        //disconnected()信号在断开连接时发出
        connect(this,&FtpSocket::disconnected,this,&FtpSocket::slotDisconnected);

    }
    qintptr socketDescriptor(){
        return socket->socketDescriptor();
    }
    void writedata(QByteArray data){
        socket->write(data);
    }

};

#endif // FTPTCPSOCKET_H
