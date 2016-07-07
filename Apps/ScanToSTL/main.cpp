#include <QCoreApplication>
#include "scantostlmcu.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("SeraphScanToSTL");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    ScanToSTLMCU* sts = new ScanToSTLMCU();
    sts->processScan();

    return a.exec();
}
