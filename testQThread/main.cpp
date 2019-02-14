#include <QCoreApplication>
#include "testthread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TestThread *t = new TestThread(0);
    TestThread *p = new TestThread(1);
    t->start();
    p->start();
//    int i=5;
//    while(i--){
//        t = new TestThread(i);
//        t->start();
//    }

    return a.exec();
}
