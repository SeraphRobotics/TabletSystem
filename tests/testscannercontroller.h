#ifndef TESTSCANNERCONTROLLER_H
#define TESTSCANNERCONTROLLER_H

#include <QObject>
#include <QtTest/QtTest>

#include "Controllers/scannercontroller.h"
#include "Controllers/scannerwatcher.h"

class TestScannerController : public QObject
{
    Q_OBJECT
public:
    explicit TestScannerController(QObject *parent = 0);
public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private:
    ScannerWatcher* sw;
    ScannerController* sc;
};
#endif //TESTSCANNERCONTROLLER_H
