#include "usermanager.h"
#include <QTextStream>
#include <QSettings>

UserDataManager::UserDataManager(QObject *parent) :
    QObject(parent)
{
    //THESE VALUES SHOULD BE LOADED FROM QSETTINGS

    QSettings settings;
    dir_= settings.value("users-directory",QDir::currentPath()).toString();
    filename_ = settings.value("users-file","users.xml").toString();
    rescan();

}

QPixmap UserDataManager::getIcon(QString user_id){
    if(hasUserID(user_id)){
        return QPixmap(getUserByID(user_id).iconfile);
    }
    return QPixmap();
}


QList<User> UserDataManager::getUsers(){
    return users_;
}

bool UserDataManager::hasUserID(QString id){
    for(int i=0;i<users_.size();i++){
        User u = users_.at(i);
        if(u.id==id){
            return true;
        }
    }
    return false;
}

User UserDataManager::getUserByPwd(QString pwd){
    for(int i=0;i<users_.size();i++){
        User u = users_.at(i);
        if(u.pwd==pwd){
            return u;
        }
    }
    User return_u;
    return_u.id="";
    return return_u;
}

User UserDataManager::getUserByID(QString id){
    for(int i=0;i<users_.size();i++){
        User u = users_.at(i);
        if(u.id==id){
            return u;
        }
    }
    User return_u;
    return_u.id="";
    return return_u;
}

void UserDataManager::changeDir(QString dir){
    QDir d(dir);
    if(d.exists()){
        dir_=d;
        rescan();
        //SET QSETTINGS
    }
}

void UserDataManager::changeUserPin(QString id, QString Pin){
    if(hasUserID(id)){
        User u = getUserByID(id);
        u.pwd=Pin;
        writeToDisk();
    }
}

void UserDataManager::addUser(User u){
    if(!hasUserID(u.id)){
        users_.append(u);
        writeToDisk();
    }
}

void UserDataManager::removeUser(User u){
    if(hasUserID(u.id)){
        users_.removeOne(u);
        writeToDisk();
    }
}


void UserDataManager::updateUser(User u){
    for(int i=0;i<users_.size();i++){
        User u_old = users_[i];
        if(u.id==u_old.id){
            users_[i] = u;
            writeToDisk();
            return;
        }
    }

}


void UserDataManager::rescan(){
    QDomDocument d("UserData");

    QFile file(filename_); //TODO: implement QDir dir_.path()+
    if (!file.open(QIODevice::ReadOnly)){return;}

    if (!d.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement usersEl = d.documentElement();
    if(!("users"==usersEl.nodeName().toLower())){return;}

    QDomNodeList userEls = usersEl.childNodes();
    for(int i=0;i<userEls.size();i++){
        QDomNode mchild = userEls.at(i);
        if(!mchild.isElement()){continue;}
        if(("user"==mchild.nodeName().toLower())){
            users_.append(userFromNode(mchild));
        }
    }

}

void UserDataManager::writeToDisk(){
    QDomDocument d("UserFile");

    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly)){return;}

    QDomElement node = d.createElement("Users");

    for(int i=0; i<users_.size(); i++){
        node.appendChild(nodeFromUsers(users_.at(i)));
    }
    d.appendChild(node);

    QTextStream f(&file);
    f<<d.toString();
    file.close();
}

