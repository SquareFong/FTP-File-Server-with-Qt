#include <QCoreApplication>
#include <QtNetwork/QTcpSocket>
#include "testftpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    testFTPServer *socket = new testFTPServer();
    socket->write("UA anonymous\n\n");
//    Master *m;
//    m = new Master();
    return a.exec();
}
