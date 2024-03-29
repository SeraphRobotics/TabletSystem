#include <QCoreApplication>
#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>

#include "testscanmanger.h"
#include "testorthoticmanager.h"
#include "testorthoticcontroller.h"
#include "testscannercontroller.h"


#ifdef Q_OS_WIN
const QString SampleDataLocation(QString(QDir::currentPath()+ "/SampleData/" ));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/output/" ));
#else
const QString SampleDataLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/"));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/output"));
#endif //Q_OS_WIN

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);
    qDebug()<<"SAMPLE DATA LOCATION: "<<SampleDataLocation;
    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("SeraphData");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setValue("scan-extension","scan");
    settings.setValue("scan-directory", SampleDataLocation);

    settings.setValue("ortho-extension","ortho");
    settings.setValue("ortho-directory", SampleDataLocation);


    settings.setValue("Generating/border",2);
    settings.setValue("Generating/slope",63.5/101);
    settings.setValue("Generating/offset",2.0);
    settings.setValue("Generating/blurtimes",10);
    settings.setValue("printing/topcoat-thickness",2.0);

    //settings.setValue("data-output-directory", SampleDataOutputLocation);
    settings.setValue("printing/directory", SampleDataOutputLocation);

#ifdef Q_OS_WIN
    settings.setValue("printing/slicer","\"C://Program Files//Repetier-Host//Slic3r//slic3r-console.exe\"");
#else
    settings.setValue("printing/slicer","/usr/bin/slic3r");
#endif //Q_OS_WIN

    settings.setValue("printing/plastic_ini", QString(SampleDataLocation) + "p.ini");
    settings.setValue("printing/inis",QString(SampleDataLocation) );

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

//    TestOrthoticController testOrthoticController;
//    QTest::qExec(&testOrthoticController,  argc, argv);

    TestScannerController* testScannerController = new TestScannerController();
    testScannerController->initTestCase();
    return app.exec();


//    return 0;

}




















