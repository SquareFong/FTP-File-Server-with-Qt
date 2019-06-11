#ifndef MASTER_H
#define MASTER_H

#include"getfile.h"
#include"testftpserver.h"
#include<QObject>
class Master :  public QObject{
    Q_OBJECT
    GetFile *file;
    testFTPServer *t;
private slots:
    void sendToken(QString token);
public:
    Master();
};

#endif // MASTER_H
