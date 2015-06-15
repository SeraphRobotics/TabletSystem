// qt
#include <QDebug>

// local
#include "testscanmanger.h"
#include "globals.h"
#include "DataStructures/scanmanager.h"
#include "DataStructures/scan.h"

const QString ScanId("{f3338f73-e3a0-4f36-9cfb-e00bbdf83e93}");
//const QString ScanId("{73-e3a0-4f36-9cfb-e00bbdf83e93}");

void TestScanManger::initTestCase()
{
    sm = new ScanManager();
}

void TestScanManger::cleanupTestCase()
{
    delete sm;
}

void TestScanManger::init()
{
}

void TestScanManger::cleanup()
{
}

// tests
void TestScanManger::testIfSampleScanIdFound()
{
    QVERIFY(sm->hasScan(ScanId) == true);
}

void TestScanManger::testGetScan()
{
    QCOMPARE(sm->getScan(ScanId)->getID(), ScanId);
}
