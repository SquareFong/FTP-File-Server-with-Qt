#ifndef INPUTMODULE_H
#define INPUTMODULE_H

#include<QObject>
#include<iostream>
#include<QString>
#include<QThread>
#include<string>
using namespace std;
class InputModule:public QThread{
signals:
    void emessage(const char *);
public:
    void run(){
        string str;
        cout << ">>";
        while(getline(cin,str)){
            emit emessage(str.c_str());
            cout << ">>";
        }
    }
};

#endif // INPUTMODULE_H
