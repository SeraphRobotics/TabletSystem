#include "qtquick2applicationviewer.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>

#include <QDebug>

#include "Windows.h"

class QtQuick2ApplicationViewerPrivate
{
    QString mainQmlFile;
    friend class QtQuick2ApplicationViewer;
    static QString adjustPath(const QString &path);
};

QString QtQuick2ApplicationViewerPrivate::adjustPath(const QString &path)
{
#if defined(Q_OS_IOS)
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#elif defined(Q_OS_MAC)
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#elif defined(Q_OS_BLACKBERRY)
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("app/native/%1").arg(path);
#elif !defined(Q_OS_ANDROID)
    QString pathInInstallDir =
            QString::fromLatin1("%1/../%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
    pathInInstallDir =
            QString::fromLatin1("%1/%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#elif defined(Q_OS_ANDROID_NO_SDK)
    return QLatin1String("/data/user/qt/") + path;
#endif
    return path;
}

QtQuick2ApplicationViewer::QtQuick2ApplicationViewer(QWindow *parent)
    : QQuickView(parent)
    , d(new QtQuick2ApplicationViewerPrivate())
{
    setObjectName("QtQuick2ApplicationViewer");
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setResizeMode(QQuickView::SizeRootObjectToView);
    m_defaultMaximumSize = maximumSize();
    /// @note after discussion with adam we set this as our minium working size
    /// 1280-800 this is minimal working resolution.(Finall devices resolution as well.)
    m_normalMinimumSize = QSize(1280, 800);
    setMinimumSize(m_normalMinimumSize);

    connect(this,SIGNAL(activeChanged()),this,SLOT(_windowsActiveChanged()));
}

QtQuick2ApplicationViewer::~QtQuick2ApplicationViewer()
{
    delete d;
}

void QtQuick2ApplicationViewer::setMainQmlFile(const QString &file)
{
    d->mainQmlFile = QtQuick2ApplicationViewerPrivate::adjustPath(file);
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_NO_SDK)
    setSource(QUrl(QLatin1String("assets:/")+d->mainQmlFile));
#else
    setSource(QUrl::fromLocalFile(d->mainQmlFile));
#endif
}

void QtQuick2ApplicationViewer::addImportPath(const QString &path)
{
    engine()->addImportPath(QtQuick2ApplicationViewerPrivate::adjustPath(path));
}

void QtQuick2ApplicationViewer::showExpanded()
{
#if defined(Q_WS_SIMULATOR) || defined(Q_OS_QNX)
    showFullScreen();
#else
    show();
#endif
}

void QtQuick2ApplicationViewer::_setCapsLockPressed()
{
    auto checkCapsLockState = [] ()
    {
        // platform dependent method of determining if CAPS LOCK is on
#ifdef Q_OS_WIN32 // MS Windows version
        return GetKeyState(VK_CAPITAL) == 1;
#else // X11 version (Linux/Unix/Mac OS X/etc...)
        Display * d = XOpenDisplay((char*)0);
        bool caps_state = false;
        if (d)
        {
            unsigned n;
            XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
            caps_state = (n & 0x01) == 1;
        }
        return caps_state;
#endif;
    };

    m_CapsLockPressed = checkCapsLockState();
    qDebug()<<"Check caps lock pressed state, current is :"<<m_CapsLockPressed;
    emit sigCapsLockPressedChanged();
}

bool QtQuick2ApplicationViewer::capsLockPressed() const
{
    return m_CapsLockPressed;
}

void QtQuick2ApplicationViewer::_windowsActiveChanged()
{
    if(isActive() == true)
        _setCapsLockPressed();
}

void QtQuick2ApplicationViewer::keyReleaseEvent(QKeyEvent * e)
{
    if(e->key() == Qt::Key_CapsLock)
    {
        _setCapsLockPressed();
    }

    QQuickView::keyPressEvent(e);
}

void QtQuick2ApplicationViewer::lockResize()

{
    qDebug()<<"Lock resize";
    setMaximumSize(QSize(width(), height()));
    setMinimumSize(QSize(width(), height()));
}

void QtQuick2ApplicationViewer::unlockResize()

{
    setMaximumSize(m_defaultMaximumSize);
    setMinimumSize(m_normalMinimumSize);
}


