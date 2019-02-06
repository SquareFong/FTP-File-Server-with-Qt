#include <QCoreApplication>
#include "mytcpserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTcpServer server("127.0.0.1",6666);
    return a.exec();
}
