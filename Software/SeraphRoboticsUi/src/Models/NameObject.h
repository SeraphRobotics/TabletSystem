#ifndef NAMEOBJECT_H
#define NAMEOBJECT_H

#include <QObject>

/**
 * @brief The NameObject class
 * Class used to store doctor's full name with title.
 * NameObject = title+firstName+lastName
 */
class NameObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString firstName READ firstName
               NOTIFY sigFirstNameChanged)
    Q_PROPERTY(QString lastName READ lastName
               NOTIFY sigLastNameChanged)
    Q_PROPERTY(QString title READ title
               NOTIFY sigTitleChanged)

public:
    explicit NameObject(const QString &firstName = QString::null,
                        const QString &lastName = QString::null,
                        const QString &title = QString::null,
                        QObject *parent = 0);


    QString firstName() const;
    QString lastName() const;
    QString title() const;

    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setTitle(const QString &title);

signals:
    void sigFirstNameChanged() const;
    void sigLastNameChanged() const;
    void sigTitleChanged() const;

private:
    QString m_Title;
    QString m_FirstName;
    QString m_LastName;
};

#endif // NAMEOBJECT_H
