#include "UsersListModel.h"

UsersListModel::UsersListModel(QObject *parent) :
    QObject(parent),
    m_CurrentIndex(-1)
{
}

void UsersListModel::removeLast()
{
    if(m_UsersList.size() == 0)
        return;
    m_UsersList.removeLast();
    emit sigUsersListModificate();
}

void UsersListModel::appendNewUser(UserObject *user)
{
    m_UsersList.append(user);
    emit sigUsersListModificate();
}

void UsersListModel::appendNewUser(const QString &firstName,
                                  const QString &lastName,
                                  const QString &title,
                                  const QString &id,
                                  const QString &password,
                                  const QString &iconPath,
                                  const QString &degree)
{
    m_UsersList.append(new UserObject(
                           new NameObject(firstName,lastName,title,this),
                           id,
                           password,
                           iconPath,
                           degree,
                           this)
                       );
    emit sigUsersListModificate();
}

void UsersListModel::updateUser(unsigned i,
                                const QString &firstName,
                                const QString &lastName,
                                const QString &title,
                                const QString &id,
                                const QString &password,
                                const QString &iconPath,
                                const QString &degree)
{
    m_UsersList[i] = new UserObject(new
                                    NameObject(firstName,
                                               lastName,
                                               title,
                                               this),
                                    id,
                                    password,
                                    iconPath,
                                    degree,
                                    this);
    emit sigUsersListModificate();
}

QQmlListProperty<QObject> UsersListModel::usersList()
{
    return QQmlListProperty<QObject>(this, m_UsersList);
}

UserObject* UsersListModel::getSpecificItem(const int &index) const
{
    if(index < 0 || index > m_UsersList.size()-1)
    {
        qDebug()<<"Out of list range";
        return 0;
    }
    return qobject_cast<UserObject*>(m_UsersList.at(index));
}
/*
 * Function searchs for value of valueName type(id, password, iconPath, firstName, lastName, title) in m_UsersList.
 * Returns true if value is in list.
 * Returns false if value isn't in the list.
*/
bool UsersListModel::checkIfValueExist(const QString &value, const QString &valueName)
{
    ///  @note for some odd reason QString::null and != "" here not working
    if(value.isEmpty())
        return true;

    int index = -1;
    for( QObject *userObject: m_UsersList)
    {
        index++;
        if(qobject_cast<UserObject*>(userObject)->getSpecificValue(valueName) == value)
        {
            return true;
        }
    }
    return false;
}

int UsersListModel::currentIndex() const
{
    return m_CurrentIndex;
}

void UsersListModel::setCurrentIndex(const int &index)
{
    m_CurrentIndex = index;
    emit sigCurrentIndexChanged();
}

int UsersListModel::size() const
{
    return m_UsersList.size();
}

QString UsersListModel::getNameById(const QString &id)
{
    if(checkIfValueExist(id, "id"))
    {
        UserObject* obj = getSpecificItem(m_CurrentIndex);

        if(obj == 0)
            return "";

        return obj->name()->title() + " "
                + obj->name()->firstName() + " "
                + obj->name()->lastName();
    }
    else
    {
        return QString::null;
    }
}

void UsersListModel::removeAt(int i) {
    m_UsersList.removeAt(i);
    emit sigUsersListModificate();
}

void UsersListModel::resetList()
{
    emit sigUsersListReset();
    emit sigUsersListModificate();
}

void UsersListModel::saveUserList()
{
    emit sigSaveUserList(&m_UsersList);
}

