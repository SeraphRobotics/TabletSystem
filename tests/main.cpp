//#include <QtGui/QGuiApplication>
#include <QCoreApplication>
#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>


#include "testscanmanger.h"

int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    qDebug() << "running tests";

    TestScanManger testScanMananger;




    QTest::qExec(&testScanMananger,  argc, argv);

    //return 0;
    return app.exec();
}

