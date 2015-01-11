#ifndef QTQUICK2APPLICATIONVIEWER_H
#define QTQUICK2APPLICATIONVIEWER_H

#include <QtQuick/QQuickView>

#include <QDebug>

class QtQuick2ApplicationViewer : public QQuickView
{
    Q_OBJECT

    Q_PROPERTY(bool capsLockPressed READ capsLockPressed
               NOTIFY sigCapsLockPressedChanged())

public:
    explicit QtQuick2ApplicationViewer(QWindow *parent = 0);
    virtual ~QtQuick2ApplicationViewer();

    void setMainQmlFile(const QString &file);
    void addImportPath(const QString &path);
    void showExpanded();

    bool capsLockPressed() const;

public slots :

    Q_INVOKABLE void lockResize();
    Q_INVOKABLE void unlockResize();

signals:
    void sigCapsLockPressedChanged() const;

protected:
    virtual void keyReleaseEvent(QKeyEvent * e);

private:
    void _setCapsLockPressed();

private slots:
    void _windowsActiveChanged();

private:
    class QtQuick2ApplicationViewerPrivate *d;
    QSize m_defaultMaximumSize;
    QSize m_normalMinimumSize;

    bool m_CapsLockPressed;
};

#endif // QTQUICK2APPLICATIONVIEWER_H
