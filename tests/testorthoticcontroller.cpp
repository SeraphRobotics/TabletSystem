// qt
#include <QDebug>
#include <QSettings>

// local
#include "testorthoticcontroller.h"
#include "DataStructures/scanmanager.h"
#include "DataStructures/orthoticmanager.h"
#include "Controllers/printjobcontroller.h"
#include "DataStructures/orthotic.h"
#include "globals.h"

#define TEST_PRIVATE_METHODS

#ifdef TEST_PRIVATE_METHODS
// ugly hack to TDD private methods, use carefully, avoid if possible
  #define private public
  #include "Controllers/orthoticscontroller.h"
  #undef private
#else
  #include "Controllers/orthoticscontroller.h"
#endif



const QString OrthoticId("{6ae3fe2d-9d96-4d7d-a58c-8d9ebe015c88}");

// public slots
void TestOrthoticController::boundaryLoopUpdated(FAHLoopInXYPlane *loop)
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
    //img.save("img.jpeg");
    QSettings settings;
    writeLoopToXDFL(loop, settings.value("printing/directory").toString() + "/Loop.xdfl");
}

void TestOrthoticController::scanImageGenerated(QImage img)
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
    //img.save("img.jpeg");
    QSettings settings;
    img.save(settings.value("printing/directory").toString() + "/img.jpeg");
}

void TestOrthoticController::stlsGenerated(QList<View_3D_Item> items)
{
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
    Q_UNUSED(items);
}

// test suite setup
void TestOrthoticController::initTestCase()
{
    sm = new ScanManager();
    om = new OrthoticManager(sm);
    oc = new OrthoticController(om);

    connect(oc,SIGNAL(boundaryLoopUpdated(FAHLoopInXYPlane*)), this, SLOT(boundaryLoopUpdated(FAHLoopInXYPlane*)));
    connect(oc,SIGNAL(scanImageGenerated(QImage)), this, SLOT(scanImageGenerated(QImage)));
    connect(oc,SIGNAL(stlsGenerated(QList<View_3D_Item>)), this, SLOT(stlsGenerated(QList<View_3D_Item>)));

    // LOAD WHAT HTE USER WOULD SET
    float offset = 3.0;
    float scalex =2.0;
    QVector< FAHVector3 > forePts;
    forePts.append(FAHVector3(scalex*(105.0+offset),60.0,0));
    forePts.append(FAHVector3(scalex*(120.0+offset),90.0,0));
    forePts.append(FAHVector3(scalex*(115.0+offset),125.0,0));
    forePts.append(FAHVector3(scalex*(85.0+offset),130.0,0));

    QVector< FAHVector3 > healPts;
    healPts.append(FAHVector3(scalex*(35.0+offset),50.0,0));
    healPts.append(FAHVector3(scalex*(19.0+offset),70.0,0));
    healPts.append(FAHVector3(scalex*(26.0+offset),103.0,0));

    Posting forpost;
    forpost.angle=0*M_PI/180.0;
    forpost.verticle=0;
    forpost.varus_valgus=Posting::kValgus;
    forpost.for_rear=Posting::kForFoot;
    Posting rearpost;
    rearpost.angle=0*M_PI/180.0;
    rearpost.verticle=0;
    rearpost.varus_valgus=Posting::kValgus;
    rearpost.for_rear=Posting::kRearFoot;

    Top_Coat tc;
    tc.density = Top_Coat::kHigh;
    tc.depth=1;
    tc.style=Top_Coat::kAuto;
    tc.thickness=5;

    oc->setScan(sm->scanIds()[0]);
    oc->setBorderPoints(healPts, forePts);
    oc->setTopCoat(tc);
    oc->processBoundary();
    oc->setPosting(forpost);

    oc->setPosting(rearpost);
    oc->setBottomType(Orthotic::kCurved);

    qDebug() << "post setBottom";

    for(int i=0;i<1;i++){
        FAHLoopInXYPlane* c = circle(2*(40.0+i*25),80.0+i*5,15.0);
        Manipulation* m = new Manipulation();
        m->stiffness=25+i*25;
        m->depth=0;
        m->thickness = 3;
        m->outerloop = c;
        m->type = Manipulation::kCustom;
        m->location = FAHVector3(0,0,0);
        m->innerloops = QList<FAHLoopInXYPlane*>();
        oc->addManipulation(m);
    }
//    oc->save();
    qDebug()<<"generated manipulations";
    qDebug() << "pre make stls";

    oc->makeSTLs();

    qDebug() << "post make stls";
    PrintJobController* pjc = new PrintJobController(oc->getOrthotic());
//    PrintJobTester* pjt = new PrintJobTester(pjc);
    pjc->RunPrintJob();
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
void TestOrthoticController::testGetOrthotic()
{
    //QCOMPARE(oc->getOrthotic()->getId(), OrthoticId);
}




















