//#include <QtGui/QGuiApplication>
#include <QCoreApplication>
#include <QtTest/QtTest>
#include <QEventLoop>
#include <QDebug>


int main(int argc, char** argv) {
    QCoreApplication app(argc, argv);

    qDebug() << "running tests";

    //EthLinkStatusTest linkTest;

    //QTest::qExec(&linkTest,  argc, argv);


    //return 0;
    return app.exec();
}

