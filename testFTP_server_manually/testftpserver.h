#ifndef TESTFTPSERVER_H
#define TESTFTPSERVER_H

#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <string>
#include <iostream>
using namespace std;
class testFTPServer : public QTcpSocket{
    Q_OBJECT
private:
    QString fileName;
    QHostAddress hostName;
    //只有上传下载的时候resolve
    bool resolveCommand(const QString &command,
                        uint16_t &port,QString &token);
    bool isResolve;
    bool down;
    bool up;
signals:
    void addUploadTask_test(QHostAddress host, uint16_t port,QString token, QString fileName);
    void addDownloadTask_test(QHostAddress host, uint16_t port,QString token, QString fileName);
    void sendToken(QString token);
private slots:
    void readSocketData();
//public slots:
//    void writeD(const char *s){
//        write(s);
//    }
public:
    testFTPServer();
};


#endif // TESTFTPSERVER_H
