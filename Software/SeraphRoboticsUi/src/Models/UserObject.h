#ifndef USEROBJECT_H
#define USEROBJECT_H

#include <QObject>
#include <QMetaType>
#include <QVariant>

#include "NameObject.h"

/**
 * @brief The UserObject class
 * Class used to store user data.
 */
class UserObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id NOTIFY sigIdChanged)
    Q_PROPERTY(QString password READ password NOTIFY sigPasswordChanged)
    Q_PROPERTY(QString iconPath READ iconPath NOTIFY sigIconPathChanged)

    Q_PROPERTY(NameObject* name READ name NOTIFY sigNameChanged)

public:
    UserObject(NameObject *name,
               const QString &id = QString::null,
               const QString &password = QString::null,
               const QString &iconPath = QString::null,
               const QString &degree = QString::null,
               QObject *parent = 0);

    QString id() const;
    QString password() const;
    QString iconPath() const;
    NameObject* name();

    void setId(const QString &id);
    void setPassword(const QString &password);
    void setName(NameObject *firstName);

    QString getSpecificValue(const QString &name) const;

signals:
    void sigIdChanged() const;
    void sigPasswordChanged() const;
    void sigNameChanged() const;
    void sigIconPathChanged() const;

private:
    NameObject* m_NameObject;
    QString m_Id;
    QString m_Password;
    QString m_IconPath;
    QString m_Degree;
};

#endif // USEROBJECT_H

