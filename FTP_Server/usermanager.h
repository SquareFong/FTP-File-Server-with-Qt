#ifndef USERMANAGER_H
#define USERMANAGER_H

#include<string>
#include<vector>
#include<QString>
using namespace std;
class UserManager{

    vector<string> users;
    vector<string> passwd;

public:
    UserManager(){
        users.push_back("anonymous");
        passwd.push_back("");
        users.push_back("square");
        passwd.push_back("987654");
    }
    bool authentication(string username, string password=""){
        auto it1=users.begin();
        auto it2=passwd.begin();
        while(it1!=users.end()){
            if(username==(*it1)){
                return password == (*it2);
            }

            ++it1; ++it2;
        }
        return false;
    }
};

#endif // USERMANAGER_H
