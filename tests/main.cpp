//#include <QtGui/QGuiApplication>
#include <QCoreApplication>
#include <QtTest/QtTest>
#include <QEventLoop>

//#include "config.h"
//#include "utils/logger.h"


int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);


    //EthLinkStatusTest linkTest;

    //QTest::qExec(&linkTest,  argc, argv);


    //return 0;
    return app.exec();
}

