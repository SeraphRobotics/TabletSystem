#include <QCoreApplication>
#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>

#include "testscanmanger.h"
#include "testorthoticmanager.h"
#include "testorthoticcontroller.h"

const QString SampleDataLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/"));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/output"));

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("SeraphData");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setValue("scan-extension","scan");
    settings.setValue("scan-directory", SampleDataLocation);

    settings.setValue("ortho-extension","ortho");
    settings.setValue("ortho-directory", SampleDataLocation);

    //settings.setValue("data-output-directory", SampleDataOutputLocation);
    settings.setValue("printing/directory", SampleDataOutputLocation);

    settings.setValue("printing/slicer","/usr/bin/slic3r");
    settings.setValue("printing/plastic_ini", QString(SampleDataLocation) + "p.ini");

    settings.setValue("printing/valving-python-script",QString(SampleDataLocation + "toValve.py"));
    settings.setValue("printing/merge-python-script",QString(SampleDataLocation +"merge.py"));



    settings.setValue("patient-file","patients.xml");
    settings.setValue("patient-directory",QDir::currentPath());

    settings.setValue("users-file","users.xml");
    settings.setValue("users-directory",QDir::currentPath());

    settings.sync();
    qDebug() << "running tests";

    // test cases
    //TestScanManger testScanMananger;
    //QTest::qExec(&testScanMananger,  argc, argv);

    //TestOrthoticManager testOrthoticManager;
    //QTest::qExec(&testOrthoticManager,  argc, argv);

    TestOrthoticController testOrthoticController;
    QTest::qExec(&testOrthoticController,  argc, argv);


    return 0;
    return app.exec();
}




















