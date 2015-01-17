#include "NameObject.h"

NameObject::NameObject(const QString &firstName,
                       const QString &lastName,
                       const QString &title,
                       QObject *parent) :
    QObject(parent),
    m_Title(title),
    m_FirstName(firstName),
    m_LastName(lastName)

{

}

QString NameObject::firstName() const
{
    return m_FirstName;
}

QString NameObject::lastName() const
{
    return m_LastName;
}

QString NameObject::title() const
{
    return m_Title;
}

void NameObject::setFirstName(const QString &firstName)
{
    if(m_FirstName != firstName)
    {
        m_FirstName = firstName;
        emit sigFirstNameChanged();
    }
}

void NameObject::setLastName(const QString &lastName)
{
    if(m_LastName != lastName)
    {
        m_LastName = lastName;
        emit sigLastNameChanged();
    }
}

void NameObject::setTitle(const QString &title)
{
    if(m_Title != title)
    {
        m_Title = title;
        emit sigTitleChanged();
    }
}
