#include <QCoreApplication>
#include <QSettings>
#include "recordkeeper.h"
#include "usbminder.h"
#include "scannerarduinointerface.h"

#include "../qdevicewatcher-master/src/qdevicewatcher.h"
#include "../qdevicewatcher-master/src/qdevicechangeevent.h"

#include "scannerwatcher.h"
#include "scannercontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("ScannerMinder");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings s;

    int cn = s.value("camNumber",-1).toInt();
    if(cn ==-1){
        s.setValue("camNumber",1);
    }

//    recordkeeper rk;
    //USBMinder* um = new USBMinder();
//    rk.connect(&um,SIGNAL(usbadded(QString)),&rk,SLOT(USBAdded(QString)));

//    ScannerArduinoInterface sai("COM5",BAUD9600);
//    QTimer::singleShot(2000,&sai,SLOT(laserOn()));
    ScannerWatcher* sw = new ScannerWatcher();
    ScannerController* sc = new ScannerController();


    sc->connect(sw,SIGNAL(scannerPort(QString)),sc,SLOT(portSelected(QString)));
    sc->connect(sw,SIGNAL(disconnected()),sc,SLOT(disconnected()));

    return a.exec();
}
