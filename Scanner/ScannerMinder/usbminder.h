#ifndef USBMINDER_H
#define USBMINDER_H

#include <QObject>
#include <QThread>


#include "../qdevicewatcher-master/src/qdevicewatcher.h"
#include "../qdevicewatcher-master/src/qdevicechangeevent.h"

class USBMinder : public QThread
{
    Q_OBJECT
public:
    explicit USBMinder(QObject *parent = 0);
    
signals:
    void usbRemoved(QString addr);
    void usbadded(QString addr);
    
public slots:
    void slotDeviceAdded(const QString& dev);
    void slotDeviceRemoved(const QString& dev);
    void slotDeviceChanged(const QString& dev);


protected:
//    virtual bool event(QEvent *e);

private:
    QDeviceWatcher* watcher;
};

#endif // USBMINDER_H
