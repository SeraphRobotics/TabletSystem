#include <QApplication>
#include <QSettings>
#include "Controllers/usbminder.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Seraph");
    QApplication::setOrganizationDomain("seraphrobotics.com");
    QApplication::setApplicationName("TabletCoreSoftware");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    USBMinder um;


    return a.exec();
}
