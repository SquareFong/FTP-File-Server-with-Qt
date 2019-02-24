#include <QCoreApplication>
#include "mytcpclient.h"
#include <unistd.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpClient client0("127.0.0.1",6666);
    client0.write_Data();

    return a.exec();
}
