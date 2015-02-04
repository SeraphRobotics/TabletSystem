// qt
#include <QDebug>


// local
#include "testorthoticcontroller.h"
#include "DataStructures/scanmanager.h"
#include "DataStructures/orthoticmanager.h"
#include "Controllers/orthoticscontroller.h"


void TestOrthoticController::initTestCase()
{
    sm = new ScanManager();
    om = new OrthoticManager(sm);
    oc = new OrthoticController(om);
}


void TestOrthoticController::cleanupTestCase()
{
}

void TestOrthoticController::init()
{
}

void TestOrthoticController::cleanup()
{
}


// tests
