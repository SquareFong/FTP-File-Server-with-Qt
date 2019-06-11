#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include<QDir>
#include<QFileInfoList>
#include<QtDebug>
#include<vector>
#include<string>
#include<QFile>
using namespace std;

//将相对路径转换成绝对路径并执行
class FileManager{
    QString rootPath;
    QString relativePath;
    QDir dir;
private:
    QString jointPath(QString path){
        if(path[0] == '/'){
            return rootPath + path.mid(1);
        }
        else {
            QString tPath(rootPath);
            if(tPath[tPath.length()-1] == '/' && relativePath[0] == '/'){
                if(relativePath.length()>1){
                    tPath.append(relativePath.mid(1));
                }
            }
            else {
                tPath.append(relativePath);
            }
            if(tPath[tPath.length()-1] == '/' && path[0] == '/'){
                if(path.length()>1){
                    tPath.append(path.mid(1));
                }
            }else {
                tPath.append(path);
            }
            return tPath;
        }
    }
public:


    FileManager():relativePath("/"){

    }

    //仅初始化附近使用
    bool setRoot(QString rootpath){
        if(dir.exists(rootpath)){
            rootPath=rootpath;
            dir.cd(rootPath);
            return true;
        }
        return false;
    }

    QFileInfoList list(QString path=""){
        if(dir.exists(rootPath+relativePath+path)){
            return QDir(rootPath+relativePath+path).entryInfoList();
        }
        else{
            //返回空列表
            return QFileInfoList();
        }
    }

    QString printWorkingDirectory(){
        return relativePath;
    }

    bool changeDirectory(QString p=""){
        if(p.length() == 0){
            dir.cd(rootPath);
            relativePath = "";
            return true;
        }
        else {
            if(dir.cd(rootPath+relativePath+p)){
                relativePath.append(p.append('/'));
                return true;
            }
            else {
                return false;
            }
        }
    }

    bool exists(QString path){
        if(path[0] == '/')
            return dir.exists(rootPath+path);
        else
            return dir.exists(rootPath+relativePath+path);
    }
/**
//    bool rmdir(QString path){
//        if(dir.exists(rootPath+relativePath+path)){
//            QDir current(rootPath+relativePath+path);
//            QFileInfoList fileList = current.entryInfoList();
//            QFileInfoList::iterator it = fileList.begin();
//            while(it != fileList.end()){
//                if((*it).fileName() != QString(".")
//                        && (*it).fileName() != QString("..")){
//                    if((*it).isDir()){
//                        rmdir(path + "/" + (*it).fileName());
//                    }
//                    else{
//                        rm(path + "/" + (*it).fileName());
//                    }
//                }
//                ++it;
//            }
//            current.remove(".");
//            return true;
//        }
//        else{
//            //返回空列表
//            return false;
//        }
//        return true;
//    }
**/

    bool rm(QString path){
        if(dir.exists(rootPath+relativePath+path)){
            return QDir(rootPath+relativePath+path).removeRecursively();
        }
        else {
            return false;
        }
    }

    bool mkdir(QString path){
        return QDir().mkpath(rootPath+relativePath+path);
    }

    bool write(QString path, QByteArray content){
        QFile file(rootPath+relativePath+path);
        if(!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "Open failed.";
            return false;
        }
        file.write(content);
        return true;
    }

    bool isFile(QString path){
        QFileInfo fileinf(jointPath(path));
        return fileinf.isFile();
    }

    QString getAbsolutepath(QString path){
        return jointPath(path);
    }
};

#endif // FILEMANAGER_H
