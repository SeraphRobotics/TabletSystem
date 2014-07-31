#include <QApplication>
#include <QSettings>
#include "Controllers/usbminder.h"
#include "Controllers/usbmanager.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Seraph");
    QApplication::setOrganizationDomain("seraphrobotics.com");
    QApplication::setApplicationName("TabletCoreSoftware");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    USBMinder um;
    USBManager manager;
    manager.connect(&um,SIGNAL(usbadded(QString)),&manager,SLOT(usbAdded(QString)));
    manager.connect(&um,SIGNAL(usbRemoved(QString)),&manager,SLOT(usbDisconnected(QString)));

    return a.exec();
}
