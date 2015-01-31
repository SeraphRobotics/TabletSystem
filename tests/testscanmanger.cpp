// qt
#include <QDebug>

// local
#include "testscanmanger.h"
#include "globals.h"
#include "DataStructures/scanmanager.h"

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




