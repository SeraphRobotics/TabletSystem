#ifndef TESTORTHOTICCONTOLLER_H
#define TESTORTHOTICCONTOLLER_H

#include <QtTest/QtTest>
#include "View/UI_structs.h"
#include "libraries/shared/fah-constants.h"

class ScanManager;
class OrthoticManager;
class OrthoticController;
//class FAHLoopInXYPlane;

class TestOrthoticController : public QObject
{
    Q_OBJECT
public slots:
    void boundaryLoopUpdated(FAHLoopInXYPlane* loop);
    void scanImageGenerated(QImage img);
     void stlsGenerated(QList<View_3D_Item> items);
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    // tests
    void testGetOrthotic();

private:

    ScanManager *sm;
    OrthoticManager *om;
    OrthoticController *oc;
};


#endif



