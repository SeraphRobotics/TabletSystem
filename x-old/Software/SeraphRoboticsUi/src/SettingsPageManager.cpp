#include "SettingsPageManager.h"

#include <QDebug>

SettingsPageManager::SettingsPageManager(QObject *parent) :
    QObject(parent),
    m_CurrentSelectedDirection("right")
   ,m_ShellModificationsState("up")
{
}

QString SettingsPageManager::mainSettingsPageState() const
{
    qDebug()<<__FUNCTION__<<" "<<m_MainSettingsPageState;
    return m_MainSettingsPageState;
}

QString SettingsPageManager::currentSelectedDirection() const
{
    qDebug()<<__FUNCTION__<<" "<<m_CurrentSelectedDirection;
    return m_CurrentSelectedDirection;
}

QString SettingsPageManager::shellModificationsState() const
{
    qDebug()<<__FUNCTION__<<" "<<m_ShellModificationsState;
    return m_ShellModificationsState;
}

QString SettingsPageManager::modificationState() const
{
    qDebug()<<__FUNCTION__<<" "<<m_ModificationPageState;
    return m_ModificationPageState;
}

void SettingsPageManager::setMainSettingsPageState(const QString &mainSettingsPageState)
{
    if(m_MainSettingsPageState != mainSettingsPageState)
    {
        m_MainSettingsPageState = mainSettingsPageState;
        emit sigMainSettingsPageStateChanged();
    }
}

void SettingsPageManager::setCurrentSelectedDirection(const QString &currentSelectedDirection)
{
    if(m_CurrentSelectedDirection != currentSelectedDirection)
    {
        qDebug()<<__FUNCTION__<<currentSelectedDirection;
        m_CurrentSelectedDirection = currentSelectedDirection;
        emit sigCurrentSelectedDirectionChanged();
    }
}

void SettingsPageManager::setShellModificationsState(const QString &shellModificationsState)
{
    if(m_ShellModificationsState != shellModificationsState)
    {
        qDebug()<<__FUNCTION__<<shellModificationsState;
        m_ShellModificationsState = shellModificationsState;
        emit sigShellModificationsStateChanged();
    }
}

void SettingsPageManager::setModificationPageState(const QString &modificationState)
{
    if(m_ModificationPageState != modificationState)
    {
        qDebug()<<__FUNCTION__<<modificationState;
        m_ModificationPageState = modificationState;
        emit sigModificationStateChanged();
    }
}
