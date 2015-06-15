#include <QCoreApplication>
#include <stdio.h>

#include <QDebug>
#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("SeraphData");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setValue("scan-extension","scan");
    settings.setValue("scan-directory",QDir::currentPath());

    settings.setValue("ortho-extension","ortho");
    settings.setValue("ortho-directory",QDir::currentPath());

    settings.setValue("patient-file","patients.xml");
    settings.setValue("patient-directory",QDir::currentPath());

    settings.setValue("users-file","users.xml");
    settings.setValue("users-directory",QDir::currentPath());

    settings.sync();

//    manipulationsTest();
//    orthoticTest2();
//    scanTest2();

//    testScanAndOrthotic();
//    patientRxTest();
//    userTest();

//    userManagerTest();
//    patientManagerWriteTest();

//    patientManagerReadTest();
//    patientManagerSecurityTest();
//    patientManagerUpdateTest();

//    patientManagerRemoveTest();

//    patientManagerOrthoTest();

//    ScanManagerReadTest();

//    ScanManagerWriteTest();

//    orthoManagerReadTest();

//    orthoManagerWriteTest();

//    newScanTest();

    newPatientTest();

    return 0;//app.exec();
}
