#include <QCoreApplication>
#include "mytcpclient.h"
int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

//    return a.exec();
    MyTcpClient client("127.0.0.1",6666);
}
