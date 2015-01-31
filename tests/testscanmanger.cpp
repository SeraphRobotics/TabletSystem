// qt
#include <QDebug>

// local
#include "testscanmanger.h"
#include "globals.h"
#include "DataStructures/scanmanager.h"
#include "DataStructures/scan.h"

const QString ScanId("{f3338f73-e3a0-4f36-9cfb-e00bbdf83e93}");

void TestScanManger::initTestCase()
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
    sm = new ScanManager();
}

void TestScanManger::cleanupTestCase()
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
}

void TestScanManger::init()
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
}


void TestScanManger::cleanup()
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
}

// tests
void TestScanManger::testIfSampleScanIdFound()
{
    QVERIFY(sm->hasScan(ScanId) == true);
}

void TestScanManger::testGetScan()
{
    Scan *scan = sm->getScan(ScanId);
    QCOMPARE(scan->getID(), ScanId);
}
