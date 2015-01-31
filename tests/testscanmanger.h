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



private:
    ScanManager *sm;


};



#endif




