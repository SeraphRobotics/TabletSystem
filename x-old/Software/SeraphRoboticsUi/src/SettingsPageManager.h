#ifndef SETTINGSPAGEMANAGER_H
#define SETTINGSPAGEMANAGER_H

#include <QObject>

/**
 * @brief The SettingsPageManager class
 * Class to store the states of SettingsPage children items.
 */
class SettingsPageManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString mainSettingsPageState READ mainSettingsPageState
               WRITE setMainSettingsPageState
               NOTIFY sigMainSettingsPageStateChanged)

    Q_PROPERTY(QString shellModificationsState READ shellModificationsState
               WRITE setShellModificationsState
               NOTIFY sigShellModificationsStateChanged)

    Q_PROPERTY(QString modificationState READ modificationState
               WRITE setModificationPageState
               NOTIFY sigModificationStateChanged)

    Q_PROPERTY(QString currentSelectedDirection READ currentSelectedDirection
               WRITE setCurrentSelectedDirection
               NOTIFY sigCurrentSelectedDirectionChanged)

public:
    explicit SettingsPageManager(QObject *parent = 0);
    QString mainSettingsPageState() const;
    QString currentSelectedDirection() const;
    QString shellModificationsState() const;
    QString modificationState() const;

    void setMainSettingsPageState(const QString &mainSettingsPageState);
    void setCurrentSelectedDirection(const QString &currentSelectedDirection);
    void setShellModificationsState(const QString &shellModificationsState);
    void setModificationPageState(const QString &modificationState);

signals:
    void sigMainSettingsPageStateChanged() const;
    void sigCurrentSelectedDirectionChanged() const;
    void sigShellModificationsStateChanged() const;
    void sigModificationStateChanged() const;

private:
    QString m_MainSettingsPageState;
    QString m_CurrentSelectedDirection;
    QString m_ShellModificationsState;
    QString m_ModificationPageState;
};

#endif // SETTINGSPAGEMANAGER_H
