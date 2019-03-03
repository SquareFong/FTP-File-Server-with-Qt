#include <QCoreApplication>
#include "ftpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FTPServer *s= new FTPServer(6666,6667);

    return a.exec();
}
