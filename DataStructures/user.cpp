#include "user.h"
#include <QUuid>

User::User(){
    id=QUuid::createUuid().toString();
}

bool User::operator==(User u){
    return (id==u.id);
}


User userFromNode(QDomNode node){
    User u;
    if(!("user"==node.nodeName().toLower())){return u;}

    u.id=node.toElement().attribute("id","");

    QDomNodeList mchildren = node.childNodes();
    for(int i=0; i < mchildren.length(); i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName().toLower();

        if("name"==name){
            u.name=nameFromNode(mchild);
        }else if("password"==name){
            u.pwd=el.text();
        }else if("icon"==name){
            u.iconfile=el.text();
        }
    }
    return u;
}

QDomNode nodeFromUsers(User u){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("user");
    node.setAttribute("id",u.id);

    node.appendChild(nodeFromName(u.name));


    QDomElement gender = d.createElement("password");
    gender.appendChild(d.createTextNode(u.pwd));
    node.appendChild(gender);

    QDomElement icon = d.createElement("icon");
    icon.appendChild(d.createTextNode(u.iconfile));
    node.appendChild(icon);

    return node;
}
