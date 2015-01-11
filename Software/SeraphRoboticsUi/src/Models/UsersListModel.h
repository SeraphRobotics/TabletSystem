#ifndef USERSLISTMODEL_H
#define USERSLISTMODEL_H

#include <QQmlListProperty>
#include <QObject>
#include <QDebug>
#include <QString>

#include "UserObject.h"

/**
 * @brief The UsersListModel class
 */
class UsersListModel : public QObject
{
    Q_OBJECT
public:

    explicit UsersListModel(QObject *parent = 0);

    Q_PROPERTY(QQmlListProperty<QObject> list READ usersList
               NOTIFY sigUsersListModificate)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex
               NOTIFY sigCurrentIndexChanged)

    Q_INVOKABLE void removeLast();

    Q_INVOKABLE void appendNewUser(UserObject *user);
    Q_INVOKABLE void appendNewUser(const QString &firstName,
                                  const QString &lastName,
                                  const QString &title,
                                  const QString &id,
                                  const QString &password,
                                  const QString &iconPath,
                                  const QString &degree);

    Q_INVOKABLE void updateUser(unsigned i,
                                const QString &firstName,
                                const QString &lastName,
                                const QString &title,
                                const QString &id,
                                const QString &password,
                                const QString &iconPath,
                                const QString &degree);
    Q_INVOKABLE UserObject* getSpecificItem(const int &index) const;
    Q_INVOKABLE bool checkIfValueExist(const QString &value,
                                       const QString &valueName);
    Q_INVOKABLE void removeAt(int i);
    Q_INVOKABLE void resetList();
    Q_INVOKABLE void saveUserList();

    Q_INVOKABLE QString getNameById(const QString &id);

    QQmlListProperty<QObject> usersList();
    int currentIndex() const;
    Q_INVOKABLE int size() const;

    void setCurrentIndex(const int &index);

signals:
    void sigUsersListModificate() const;
    void sigUsersListReset(); //Undo
    void sigSaveUserList(QList<QObject*> *usersList) const; //Save
    void sigCurrentIndexChanged() const;

private:
    QList<QObject*> m_UsersList;

    /// @note m_CurrentIndex is used for storing current user list index.
    /// Set in startScreen page and use to get values for specific user.
    int m_CurrentIndex;
};
Q_DECLARE_METATYPE(UsersListModel*)
#endif // USERSLISTMODEL_H
