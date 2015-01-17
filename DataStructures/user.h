#ifndef USER_H
#define USER_H
#include "basicstructures.h"
#include <QDomNode>

struct User{
    QString id;
    Name name;
    QString pwd;
    QString iconfile;
    User();
    bool operator==(User u);
};

User userFromNode(QDomNode node);

QDomNode nodeFromUsers(User u);
#endif // USER_H
