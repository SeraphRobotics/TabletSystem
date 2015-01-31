#ifndef TESTSCANMANGER_H
#define TESTSCANMANGER_H

#include <QtTest/QtTest>

class ScanManager;

class TestScanManger : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    // tests
    void testIfSampleScanIdFound();
    void testGetScan();

private:
    ScanManager *sm;
};

#endif








