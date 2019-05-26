#ifndef FTPCONTROLLER_SOCKET_H
#define FTPCONTROLLER_SOCKET_H

#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <random>
#include "usermanager.h"
#include"filemanager.h"
using namespace std;


static int generatorCounter=0;

class FTPControllerSocket : public QObject{
    Q_OBJECT
private:
    unsigned  bufferSize;
    QTcpSocket *socket;
    UserManager *usermanager;
    bool isLogin;
    string username;
    FileManager fileManager;
    int controllerPort;
    int transmitterPort;
    //将命令字和数字关联
    /**
    * ("UA",0)
    * ("LS",1)
    * ("PWD",2)
    * ("CD",3)
    * ("DL",4)
    * ("RM",5)
    * ("MKD",6)
    * ("UL",7)
    **/
    map<std::string,int> commands;
signals:
    //为内部提供接收信号Qstring:数据，long long:tcp描述符
    void dataReceived(QByteArray);

    void disconnected(long long);

    //发送用户认证信息
    void authentication(string username, string userpassword);

    //internal signals
    void sendFile(FileManager *fm,QString filepath);

    void receiveFile(FileManager *fm,QString filepath);

    //external signals
    void addToken(int status, QString token);

    void addTask(QString token, QString filePath);
private:
    QString tokenGenerator(){
        ++generatorCounter;
        time_t t=time(0);
        return QString((to_string(unsigned(t)) + to_string(generatorCounter)).c_str());
    }
    //用户认证函数
    void userAuthentication(vector<string> &commad){
        //UA
        //没有参数，直接返回错误
        if(commad.size()==1) {
            socket->write("530 account information needed\n\n");
        }
        //只有一个参数，默认密码为空以继续认证
        else if(commad.size() == 2) {
            if(usermanager->authentication(commad[1])) {
                username=commad[1];
                isLogin = true;
                fileManager.setRoot("/home/square/Share/");
                socket->write("230\n\n");
            }
            else{
                socket->write("530 Invalid username or password\n\n");
            }
        }
        //有用户名和密码的认证
        else if(commad.size() == 3){
            if(usermanager->authentication(commad[1],commad[2])) {
                username=commad[1];
                isLogin = true;
                fileManager.setRoot("/home/square/Share/");
                socket->write("230\n\n");
            }
            else {
                socket->write("530 Invalid username or password\n\n");
            }
        }else {
            socket->write("530 redundant parameters\n\n");
        }
    }

    //列出当前目录
    void listFiles(vector<string> &commad){
        //LS
        if(isLogin){
            if(commad.size()==1) {
                QFileInfoList inf = fileManager.list();
                QString buffer;
                QFileInfoList::iterator it=inf.begin();
                while(it != inf.end()){
                    buffer.append((*it).fileName()+"\n");
                    ++it;
                }
                if(buffer.length() != 0){
                    buffer.append("\n\n");
                    socket->write(buffer.toUtf8());
                }
                else {
                    socket->write("550 no such directory\n\n");
                }
            }
            else if(commad.size()==2){
                QFileInfoList inf = fileManager.list(commad[1].c_str());
                QString buffer;
                QFileInfoList::iterator it=inf.begin();
                while(it != inf.end()){
                    buffer.append((*it).fileName()+"\n");
                    ++it;
                }
                if(buffer.length() != 0){
                    buffer.append("\n\n");
                    socket->write(buffer.toUtf8());
                }
                else {
                    socket->write("550 no such directory\n\n");
                }
            }
            else{
                socket->write("550  redundant parameters\n\n");
            }
        }
        else{
            socket->write("530 please login first\n\n");
        }
    }

    void printWorkingDirectory(){
        if(isLogin){
            socket->write(fileManager.printWorkingDirectory().toUtf8());
        }
        else {
            socket->write("530 please login first\n\n");
        }
    }

    void changeDirectory(vector<string> &commad){
        if(isLogin){
            //没有参数，返回主目录
            if(commad.size()==1) {
                fileManager.changeDirectory();
                socket->write("250 changed to home\n\n");
            }//只有一个参数，默认密码为空以继续认证
            else if(commad.size() == 2) {
                if(fileManager.changeDirectory(commad[1].c_str())){
                    socket->write("250 directory changed success\n\n");
                }
                else {
                    socket->write("550 directory changed fail\n\n");
                }
            }
            else{
                socket->write("530 redundant parameters\n\n");
            }
        }
        else {
            socket->write("530 please login first\n\n");
        }

    }

    void remove(vector<string> &command){

        if(isLogin){
            if(command.size() == 1){
                socket->write("520 more parameters need");
            }else {
                vector<string>::iterator it = command.begin();
                ++it;
                QString buff;
                while(it != command.end()){
                    if(fileManager.exists((*it).c_str())){
                        if(!fileManager.rm((*it).c_str())){
                            buff.append(("550 " + (*it) + " remove fail\n").c_str());
                        }
                    }
                    else {
                        buff.append(("550 " + (*it) + " not exists\n").c_str());
                    }
                    ++it;
                }
                if(buff.length() != 0)
                    socket->write(buff.append(" \n\n").toUtf8());
                else {
                    socket->write("250 remove success\n\n");
                }
            }
        }
        else {
            socket->write("530 please login first\n\n");
        }
    }

    void makeDirectory(vector<string> &command){
        if(isLogin){
            if(command.size() == 2){
                if(fileManager.exists(command[1].c_str())){
                    socket->write("550 directory already exists\n\n");
                }
                else {
                    if(fileManager.mkdir(command[1].c_str())){
                        socket->write("250 folder create success\n\n");
                    }
                    else {
                        socket->write("250 fail to create folder\n\n");
                    }
                }
            }
            else{
                socket->write("550 parameters error\n\n");
            }
        }
        else {
            socket->write("530 please login first\n\n");
        }
    }

    void download(vector<string> &command){
        if(isLogin){
            if(command.size() == 2){
                auto it = command.begin();
                ++it;

                if(!fileManager.isFile((*it).c_str())){
                    socket->write(("file " + (*it) + " does not exit\n\n").c_str());
                }
                else {
                    QString token=tokenGenerator();
                    emit addToken(-1, token);
                    it = command.begin();
                    ++it;
                    while(it != command.end()){
                        emit addTask(token,fileManager.getAbsolutepath((*it).c_str()));
                        ++it;
                    }
                    socket->write(QString("220 token:%1 port:%2").arg(token,QString::number(transmitterPort)).toUtf8());
                }
            }else{
                socket->write("550 parameters error\n\n");
            }
        }
        else {
            socket->write("530 please login first\n\n");
        }
    }

    void upload(vector<string> &command){
        if(isLogin){
            if(command.size() == 2){
                auto it = command.begin();
                ++it;
                while(it != command.end()){
                    emit receiveFile(&fileManager, (*it).c_str());
                    ++it;
                }
            }else{
                socket->write("550 parameters error\n\n");
            }
        }
        else {
            socket->write("530 please login first\n\n");
        }
    }

private slots:
    //读取数据
    void readData() {
        /*bytesAvailable()表示有效数据*/
        QByteArray buff(int(bufferSize), 0);
        char temp;
        QByteArray::iterator p=buff.begin();
        int endOfMessage = 2;
        //从socket buffer中读取数据
        while(p != buff.end() && socket->bytesAvailable() > 0 && endOfMessage != 0) {
            temp = 0;
            socket->getChar(&temp);
            if(temp == '\n'){
                --endOfMessage;
            }
            else if(endOfMessage != 2){
                endOfMessage=2;
            }
            *p = temp;
            ++p;
        }
        if (socket->bytesAvailable() > 0) {
            // 还有数据在socket中
            qDebug() << socket->socketDescriptor() << ":too much data";
            //返回错误信息
            //socket->write("");
        }
        else {
            //数据读取完成
            --p;
            *p=0;
            --p;
            *p=0;
            emit dataReceived(buff);
        }
    }

    //处理读取的数据
    void processData(QByteArray buff) {
        vector<string> command;
        QByteArray::iterator it=buff.begin();
        string temp(bufferSize,'\0');
        string::iterator s=temp.begin();
        while (it != buff.end() && (*it) != 0) {
            if((*it) != ' ') {
                *s = *it;
                ++s;
            }
            else {
                command.push_back(temp.substr(0,s-temp.begin()));
                temp=string(bufferSize,'\0');
                s=temp.begin();
            }
            ++it;
        }

        if(s!=temp.begin()) {
            command.push_back(temp.substr(0,s-temp.begin()));
        }

        //处理分好的词
        /**
        * ("UA",0)
        * ("LS",1)
        * ("PWD",2)
        * ("CD",3)
        * ("DL",4)
        * ("RM",5)
        * ("MKD",6)
        * ("UL",7)
        **/
        if(commands.find(command[0]) != commands.end()){
            switch (commands[command[0]]) {
            case 0:
                userAuthentication(command);
                break;
            case 1:
                listFiles(command);
                break;
            case 2:
                printWorkingDirectory();
                break;
            case 3:
                changeDirectory(command);
                break;
            case 4:
                download(command);
                break;
            case 5:
                remove(command);
                break;
            case 6:
                makeDirectory(command);
                break;
            case 7:
                upload(command);
                break;
            default:
                socket->write("500 Unknown command\n\n");
                break;
            }
        }
        else {
            socket->write("500 Unknown command\n\n");
        }
    }

    void slotDisconnected() {
        emit disconnected(socket->socketDescriptor());
        qDebug() << socket->socketDescriptor() << ": disconnect";
    }

public:
    FTPControllerSocket(UserManager *manager, QTcpSocket *parent=nullptr)
        :bufferSize(1024),socket(parent),
          usermanager(manager), isLogin(false){
        //readyRead()是QIODevice的signal，由QTcpSocket继承而来
        //QTcpSocket被看成一个QIODevice，readyRead()信号在有数据到来时发出。
        connect(socket,&QTcpSocket::readyRead,this, &FTPControllerSocket::readData);

        //disconnected()信号在断开连接时发出
        connect(this,&FTPControllerSocket::disconnected,this,&FTPControllerSocket::slotDisconnected);

        //
        connect(this,&FTPControllerSocket::dataReceived,this,&FTPControllerSocket::processData);

        //初始化命令和
        commands.insert(pair<string,int>("UA",0));
        commands.insert(pair<string,int>("LS",1));
        commands.insert(pair<string,int>("PWD",2));
        commands.insert(pair<string,int>("CD",3));
        commands.insert(pair<string,int>("DL",4));
        commands.insert(pair<string,int>("RM",5));
        commands.insert(pair<string,int>("MKD",6));
        commands.insert(pair<string,int>("UL",6));
    }

    void setPorts(int controller, int transmitter){
        transmitterPort=transmitter;
        controllerPort=controller;
    }

    qintptr socketDescriptor() {
        return socket->socketDescriptor();
    }

    void writedata(QByteArray data) {
        socket->write(data);
    }

};


#endif // FTPCONTROLLER_SOCKET_H
