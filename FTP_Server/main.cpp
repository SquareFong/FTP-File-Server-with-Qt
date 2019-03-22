#include <QCoreApplication>
#include "ftpcontroller_server.h"
#include "ftpserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FTPControllerServer *s= new FTPControllerServer(6666);

    return a.exec();
}
