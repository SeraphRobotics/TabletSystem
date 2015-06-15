#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QObject>
#include <QString>
/**
 * @brief The QmlPageStateManager class
 * Class used to switch between screens.
 */
class QmlPageStateManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString previousState READ previousState)

public:
    explicit QmlPageStateManager(QObject *parent = 0);

    Q_INVOKABLE void setState(const QString &newState);

    QString previousState() const;

    /// @note - Gray Popup helper functions.
    Q_INVOKABLE void emitSaveToToolbar() const;
    Q_INVOKABLE void emitUseOnlyForThisPatient() const;

signals:
    void sigSwitchToSpecificScreen(const QString &pageName) const;

    void sigSaveToToolbar() const;
    void sigUseOnlyForThisPatient() const;

private:
    QString m_PreviousState;
    QString m_CurrentState;
};

#endif // STATEMANAGER_H
