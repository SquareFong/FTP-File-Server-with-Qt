#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QDebug>

class TestThread : public QThread{
    Q_OBJECT
    int id;
public:
    TestThread(int threadID):id(threadID){

    }
    void run(){
        while(true){
            qDebug() << "this is thread No." << id ;
            sleep(1);
        }
    }
};

#endif // TESTTHREAD_H
