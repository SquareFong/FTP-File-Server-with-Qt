#ifndef FTPTRANSMITTER_SOCKET_H
#define FTPTRANSMITTER_SOCKET_H

#include<QtNetwork/QTcpSocket>
#include<QObject>
#include<QString>
#include<map>
#include"filemanager.h"
using namespace std;
struct TransmitTask{
public:
    int status;
    QString token;
    vector<QString> filesPath;
    TransmitTask(int Status, QString Token):
        status(Status),token(Token){

    }
};
class FTPTransmitterSocket : public QObject{
    Q_OBJECT
private:
    unsigned  bufferSize;
    //to verify if the message is from client
    QString token;

    //0 for not setting,
    //1 for receive(puts command),
    //-1 for send(gets command)
    int status;

    QString filePath;

    QTcpSocket *socket;

    /**
    *("TOKEN",0));
    *("DATA",1));
    **/
    map<std::string,int> commands;

    const vector<TransmitTask> &tokens;

    const TransmitTask *task;

    FileManager *filemanager;
signals:
    void disconnected(long long);

    //为内部提供接收信号Qstring:数据，long long:tcp描述符
    void dataReceived(QByteArray);
private:
    bool setToken(QString token){
        //auto it = tokens.begin();
        for(auto &it: tokens){
            if(it.token == token ){
                this->token = token;
                this->status = it.status;
                task = &it;
                return true;
            }
        }
        return false;
    }

    void TOKEN(vector<string> &command){
        if(this->token.length() != 0){
            socket->write("530 already signed in\n\n");
            return;
        }
        if(command.size() == 2){
            if(setToken(command[1].c_str()))
                socket->write("230 authentication success\n\n");
            else {
                socket->write("530 fail to authentication\n\n");
            }
        }
        else {
            socket->write("520 wrong paraments\n\n");
        }
    }

    void DATA(vector<string> &command){
        if(this->token.length() == 0){
            socket->write("530 not status set\n\n");
            return;
        }else {
            //sent data to client
            if(status < 0){
                for (auto &it:task->filesPath){
                    QFile file(it);
                    if(file.open(QIODevice::ReadOnly)){
                        while (!file.atEnd()) {
                            QByteArray datas=file.read(1000);
                            socket->write(datas);
                            socket->waitForBytesWritten(1000);
                        }
                    }else {
                        socket->write("500 fail to open the file\n\n");
                    }
                    //socket->write(it.mid(t+1).append('/').toUtf8());
                }
                socket->write("DATAEND\n\n");
            }else if(status > 0){
                QFile temp(task->filesPath[0]);
                if(temp.open(QIODevice::ReadWrite|QIODevice::Append)){
                    temp.write(command[1].c_str());
                    socket->write("250 received");
                }
                else {
                    socket->write("550 file open faild");
                }
            }
        }
    }
private slots:
    void slotDisconnected() {
        emit disconnected(socket->socketDescriptor());
        qDebug() << socket->socketDescriptor() << ": disconnect";
    }

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
            //socket->readAll();
            //返回错误信息
            socket->write("500\n\n");
        }

        {
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
        bool firstBlank=true;
        while (it != buff.end() && (*it) != 0) {
            if(firstBlank){
                if((*it) != ' ') {
                    *s = *it;
                    ++s;
                }
                else {
                    command.push_back(temp.substr(0,ulong(s-temp.begin())));
                    temp=string(bufferSize,'\0');
                    s=temp.begin();
                    firstBlank = false;
                }
            }else {
                *s = *it;
                ++s;
            }
            ++it;
        }

        command.push_back(temp.substr(0,ulong(s-temp.begin())));


        //处理分好的词
        /**
        *("TOKEN",0));
        *("DATA",1));
        **/
        if(commands.find(command[0]) != commands.end()){
            switch (commands[command[0]]) {
            case 0:
                TOKEN(command);
                break;
            case 1:
                DATA(command);
                break;

            }
        }
        else {
            socket->write("500 Unknown command\n\n");
        }


    }

public:
    bool setPath(QString filePath){
        this->filePath = filePath;
        return true;
    }

    bool setStatus(int status){
        this->status = status;
        return true;
    }

//    bool setToken(QString clientToken){
//        token = clientToken;
//        return true;
//    }

    qintptr socketDescriptor() {
        return socket->socketDescriptor();
    }

    void writedata(QByteArray data) {
        socket->write(data);
    }

    FTPTransmitterSocket(const vector<TransmitTask> &Tokens,QTcpSocket *parent):
        bufferSize(1024),socket(parent),tokens(Tokens){
        connect(this,&FTPTransmitterSocket::disconnected, this,&FTPTransmitterSocket::slotDisconnected);
        connect(this,&FTPTransmitterSocket::dataReceived,this,&FTPTransmitterSocket::processData);
        connect(socket, &QTcpSocket::readyRead, this,&FTPTransmitterSocket::readData);

        //初始化命令和
        commands.insert(pair<string,int>("TOKEN",0));
        commands.insert(pair<string,int>("DATA",1));
    }
};

#endif // FTPTRANSMITTER_SOCKET_H
