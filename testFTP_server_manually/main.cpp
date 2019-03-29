#include <QCoreApplication>
#include "testftp_server.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    testFTPServer *t=new testFTPServer("127.0.0.1",6666);
//    string c;
//    while (getline(cin,c)) {
//        t->write_Data(c+"\n\n");
//    }
    QByteArray data("UA anonymous");
    //emit t->WritetoSocket(data);
    return a.exec();
}
