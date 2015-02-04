// qt
#include <QDebug>

// local
#include "testorthoticmanager.h"
#include "globals.h"
#include "DataStructures/scanmanager.h"
#include "DataStructures/orthoticmanager.h"

const QString OrthoticId("{6ae3fe2d-9d96-4d7d-a58c-8d9ebe015c88}");
//const QString OrthoticId("{2d-9d96-4d7d-a58c-8d9ebe015c88}");


void TestOrthoticManager::initTestCase()
{
    sm = new ScanManager();
    om = new OrthoticManager(sm);
}

void TestOrthoticManager::cleanupTestCase()
{
}

void TestOrthoticManager::init()
{
}

void TestOrthoticManager::cleanup()
{
}

// tests
void TestOrthoticManager::testIfSampleDataFound()
{
    QVERIFY(om->hasOrthotic(OrthoticId) == true);
}

void TestOrthoticManager::testGetOrthotic()
{
    QCOMPARE(om->getOrthotic(OrthoticId)->getId(), OrthoticId);
}

