#include <QCoreApplication>
#include "mytcpserver.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MyTcpServer *server1 = new MyTcpServer(1,"127.0.0.1",6666);

    return a.exec();
}
