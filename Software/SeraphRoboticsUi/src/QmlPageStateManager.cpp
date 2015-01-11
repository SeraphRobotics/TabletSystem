#include "QmlPageStateManager.h"

#include "ApplicationSettingsManager.h"

#include <QDebug>

QmlPageStateManager::QmlPageStateManager(QObject *parent) :
    QObject(parent)
{
    connect(&ApplicationSettingsManager::getInstance(),
            SIGNAL(setStateOnApplicationStart(const QString&)),
            this, SLOT(setState(const QString&)));
}

void QmlPageStateManager::setState(const QString &newState)
{
    qDebug()<<__FUNCTION__<<newState;

    if(newState.isEmpty() || newState == m_CurrentState)
    {
        qDebug()<<"Warning: state name is empty,"
                  "duplicated, or popup open, this should not happen:)";
        return;
    }

    m_PreviousState = m_CurrentState;
    m_CurrentState = newState;
    emit sigSwitchToSpecificScreen(newState);
}

QString QmlPageStateManager::previousState() const
{
    qDebug()<<__FUNCTION__<<m_PreviousState;
    return m_PreviousState;
}

void QmlPageStateManager::emitSaveToToolbar() const
{
    qDebug()<<__FUNCTION__;
    emit sigSaveToToolbar();
}
void QmlPageStateManager::emitUseOnlyForThisPatient() const
{
    emit sigUseOnlyForThisPatient();
}

