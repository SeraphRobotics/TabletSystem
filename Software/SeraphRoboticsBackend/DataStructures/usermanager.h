#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QDir>
#include <QString>
#include "user.h"
#include <QPixmap>

class UserDataManager : public QObject
{
    Q_OBJECT
public:
    explicit UserDataManager(QObject *parent = 0);

    QPixmap getIcon(QString user_id);

    User getUserByPwd(QString pwd);

    User getUserByID(QString id);

    bool hasUserID(QString id);

    QList< User > getUsers();

signals:

public slots:
    void changeDir(QString);

    void changeUserPin(QString id, QString Pin);

    void addUser(User u);

    void removeUser(User u);

    void updateUser(User u);

    void removeAll();


private slots:
    void rescan();

private:
    void writeToDisk();

private:
    QList<User> users_;
};

#endif // USERMANAGER_H
