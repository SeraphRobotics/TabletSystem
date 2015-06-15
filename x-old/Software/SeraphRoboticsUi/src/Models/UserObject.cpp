#include "UserObject.h"

#include <QDebug>

UserObject::UserObject(NameObject *name,
                       const QString &id,
                       const QString &password,
                       const QString &iconPath,
                       const QString &degree,
                       QObject *parent) :
    QObject(parent),
    m_NameObject(name),
    m_Id(id),
    m_Password(password),
    m_IconPath(iconPath),
    m_Degree(degree)
{
    qDebug()<<"Password is"<<m_Password;
}

QString UserObject::id() const
{
    return m_Id;
}

QString UserObject::password() const
{
    return m_Password;
}

NameObject* UserObject::name()
{
    return m_NameObject;
}

QString UserObject::iconPath() const
{
    return m_IconPath;
}

void UserObject::setId(const QString &id)
{
    if(m_Id != id)
    {
        m_Id = id;
        emit sigIdChanged();
    }
}

void UserObject::setPassword(const QString &password)
{
    if(m_Password != password)
    {
        m_Password = password;
        emit sigPasswordChanged();
    }
}

void UserObject::setName(NameObject *name)
{
    if(name != m_NameObject && name != nullptr)
    {
        m_NameObject = name;
        emit sigNameChanged();
    }
}

QString UserObject::getSpecificValue(const QString &name) const
{
    if(name == "id")
        return m_Id;
    if(name == "password")
        return m_Password;
    if(name == "iconPath")
        return m_IconPath;
    if(name == "firstName")
        return m_NameObject->firstName();
    if(name == "lastName")
        return m_NameObject->lastName();
    if(name == "title")
        return m_NameObject->title();

    return QString::null;
}
