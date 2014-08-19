#include <QApplication>
#include <QSettings>

#include "UnitTest/scanprocessingtestsfunctions.h"
#include <QTimer>


#include "Controllers/processingfunctions.h"
#include "UnitTest/usbtester.h"

#include "DataStructures/usbminder.h"
#include "DataStructures/usbmanager.h"

#include "UnitTest/scansystemtester.h"
#include "Controllers/processingfunctions.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Seraph");
    QApplication::setOrganizationDomain("seraphrobotics.com");
    QApplication::setApplicationName("TabletCoreSoftware");
    QSettings::setDefaultFormat(QSettings::IniFormat);

//   USBMinderTest();

//   USBToScanProcessingTest();

//   loopTest();

//   curveTests();


    TestMakePostingPlane();

    return a.exec();
}
