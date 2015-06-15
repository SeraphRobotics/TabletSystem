#include "usbminder.h"
#include <QDebug>

USBMinder::USBMinder(QObject *parent) //:
    //QThread(parent)
{
    Q_UNUSED(parent);
    //start();
    //moveToThread(this);
    watcher = new QDeviceWatcher();
    //watcher->moveToThread(this);
    connect(watcher, SIGNAL(deviceAdded(QString)), this, SLOT(slotDeviceAdded(QString)), Qt::DirectConnection);
    connect(watcher, SIGNAL(deviceChanged(QString)), this, SLOT(slotDeviceChanged(QString)), Qt::DirectConnection);
    connect(watcher, SIGNAL(deviceRemoved(QString)), this, SLOT(slotDeviceRemoved(QString)), Qt::DirectConnection);
    watcher->start();
}



void USBMinder::slotDeviceAdded(const QString& dev){
    qDebug()<<"Added: "<<dev;
    emit usbadded(dev);
}
void USBMinder::slotDeviceRemoved(const QString& dev){
    qDebug()<<"removed: "<<dev;
    emit usbRemoved(dev);
}
void USBMinder::slotDeviceChanged(const QString& dev){qDebug()<<"changed: "<<dev;}

//bool USBMinder::event(QEvent *e){
//    qDebug()<<"event Called"<<e->type();

//    if (e->type() == QDeviceChangeEvent::registeredType()) {
//        qDebug()<<"event registered";
//        QDeviceChangeEvent *event = (QDeviceChangeEvent*)e;
//        QString action("Change");
//        if (event->action() == QDeviceChangeEvent::Add)
//            action = "Add";
//        else if (event->action() == QDeviceChangeEvent::Remove)
//            action = "Remove";
//        qDebug()<<"USBMinder: "<<action;
////        qDebug("tid=%#x event=%d %s: %s %s", (unsigned int)QThread::currentThreadId(), e->type(), __PRETTY_FUNCTION__, qPrintable(action), qPrintable(event->device()));
//        event->accept();
//        return true;
//    }else{
//        qDebug()<<"event unregistered";
//    }
//    return QObject::event(e);
//}
