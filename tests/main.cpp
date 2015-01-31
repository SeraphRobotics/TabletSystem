//#include <QtGui/QGuiApplication>
#include <QCoreApplication>
#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>


#include "testscanmanger.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);


    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("SeraphData");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setValue("scan-extension","scan");
    settings.setValue("scan-directory", QString(QDir::currentPath() + "/../seraphLibs/Media/"));

    //settings.setValue("ortho-extension","ortho");
    //settings.setValue("ortho-directory",QDir::currentPath());

    //settings.setValue("patient-file","patients.xml");
    //settings.setValue("patient-directory",QDir::currentPath());

    //settings.setValue("users-file","users.xml");
    //settings.setValue("users-directory",QDir::currentPath());

    settings.sync();

    qDebug() << "running tests";

    TestScanManger testScanMananger;




    QTest::qExec(&testScanMananger,  argc, argv);

    return 0;
    return app.exec();
}

