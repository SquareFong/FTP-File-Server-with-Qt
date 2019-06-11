#ifndef GETFILE_H
#define GETFILE_H

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QByteArray>
#include <string>
#include <iostream>
using namespace std;

class GetFile:public QTcpSocket{
    Q_OBJECT
private:
    int remotePort;
    QString token;
    QString fileName;
    bool isReceiving;
    bool isSending;
public slots:
    void addDownloadTask_testGet(QHostAddress host, uint16_t port,QString token, QString fileName);
    void addUploadTask(QHostAddress host, uint16_t port,QString token, QString fileName);
private slots:
    void readSocketData();
    void waitforRead();
public:
    GetFile();
};

#endif // GETFILE_H
