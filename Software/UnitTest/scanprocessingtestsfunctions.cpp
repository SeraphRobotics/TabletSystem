#include "scanprocessingtestsfunctions.h"

#include "DataStructures/usbminder.h"
#include "DataStructures/usbmanager.h"
#include "DataStructures/orthoticmanager.h"

#include "UnitTest/scansystemtester.h"
#include "UnitTest/debugfunctions.h"
#include "UnitTest/usbtester.h"
#include "UnitTest/orthoticcontrollertester.h"
#include "UnitTest/printjobtester.h"


#include "Controllers/processingfunctions.h"
#include "Controllers/orthoticscontroller.h"
#include "Controllers/printjobcontroller.h"


void curveTests(){
    qDebug()<<"result factorial: "<<factorial(5);
    qDebug()<<"result comb: "<<comb(10,3);
    qDebug()<<"result b:"<<bernstein_poly(3,4,0.5);
    QVector< FAHVector3 > points;
    points.append(FAHVector3(60.0,105.0,0));
    points.append(FAHVector3(90.0,120.0,0));
    points.append(FAHVector3(125.0,115.0,0));
    points.append(FAHVector3(140.0,85.0,0));

    qDebug()<<bezier_curve(points);

    QVector< FAHVector3 > points2;
    points2.append(FAHVector3(50.0,35.0,0));
    points2.append(FAHVector3(70.0,19.0,0));
    points2.append(FAHVector3(103.0,26.0,0));

    secondOrder(points2);
    qDebug()<<secondOrder(points2);
}

void loopTest(){
    QVector< FAHVector3 > points;
    points.append(FAHVector3(60.0,105.0,0));
    points.append(FAHVector3(90.0,120.0,0));
    points.append(FAHVector3(125.0,115.0,0));
    points.append(FAHVector3(140.0,85.0,0));


    QVector< FAHVector3 > points2;
    points2.append(FAHVector3(50.0,35.0,0));
    points2.append(FAHVector3(70.0,19.0,0));
    points2.append(FAHVector3(103.0,26.0,0));

    FAHLoopInXYPlane plane = loopFromPoints(points2,points);

    writeLoopToXDFL(&plane,"test.xdfl");
}



void TestMakePostingPlane(){
    FAHVector3 hp1(50.0,35.0,0);
    FAHVector3 hp2(103.0,26.0,0);
    FAHVector3 fp1(60.0,105.0,0);
    FAHVector3 fp2(140.0,85.0,0);

    Posting forpost;
    forpost.angle=0*M_PI/180.0;
    forpost.verticle=0;
    forpost.varus_valgus=Posting::kVargus;
    forpost.for_rear=Posting::kForFoot;
    Posting rearpost;
    rearpost.angle=0*M_PI/180.0;
    rearpost.verticle=10;
    rearpost.varus_valgus=Posting::kVargus;
    rearpost.for_rear=Posting::kRearFoot;

    QVector<FAHVector3> heals = transformPointsWithPosting(hp1,hp2,rearpost);
    QVector<FAHVector3> fors = transformPointsWithPosting(fp1,fp2,forpost);
    QVector<FAHVector3> planeAndCent = makePostingPlane(heals[0],heals[1],fors[0],fors[1]);
    printPoint(planeAndCent[0]);
    printPoint(planeAndCent[1]);


}
void USBMinderTest(){
    /*USBMinder um;
    USBManager manager;
    manager.connect(&um,SIGNAL(usbadded(QString)),&manager,SLOT(usbAdded(QString)));
    manager.connect(&um,SIGNAL(usbRemoved(QString)),&manager,SLOT(usbDisconnected(QString)));
    USBTester ut;

    manager.connect(&manager,SIGNAL(uiUSBItemsUpdated(QList<UI_USB_Item>)),&ut,SLOT(uiUSBItemsUpdated(QList<UI_USB_Item>)));
    manager.connect(&ut,SIGNAL(deleteItem(QString)),&manager,SLOT(deleteItem(QString)));
*/

    USBMinder* um = new USBMinder();
    USBManager* manager = new USBManager();
    manager->connect(um,SIGNAL(usbadded(QString)),manager,SLOT(usbAdded(QString)));
    manager->connect(um,SIGNAL(usbRemoved(QString)),manager,SLOT(usbDisconnected(QString)));
    USBTester* ut = new USBTester();

    manager->connect(manager,SIGNAL(uiUSBItemsUpdated(QList<UI_USB_Item>)),ut,SLOT(uiUSBItemsUpdated(QList<UI_USB_Item>)));
    manager->connect(ut,SIGNAL(deleteItem(QString)),manager,SLOT(deleteItem(QString)));


}

void USBToScanProcessingTest(){
    ScanSystemTester* st = new ScanSystemTester();
}

void TestMinAlongLine(){
    QString csvfileLocation = "scan.csv";
    QFile f(csvfileLocation);

    if (!f.open(QIODevice::ReadOnly)){
     qDebug()<<"didnt open file";
     return;
    }
    QString data;
    QTextStream in(&f);
    //file opened successfully
    while(!in.atEnd()){
     data.append(in.readLine()+"\n");
    }
    f.close();

    //qDebug()<<data;
    XYGrid<float> x(data);
    FAHVector3 hp1(35.0+8,50.0,0);
    FAHVector3 hp2(26.0+8,103.0,0);
    FAHVector3 fp1(60.0,105.0,0);
    FAHVector3 fp2(140.0,85.0,0);
    //qDebug()<<x.toCSV();
    printPoint(minAlongLine(&x,hp1,hp2));
//    printPoint(minAlongLine(&x,fp1,fp2));
}

void TestprojectGridOntoPlane(){
    QString csvfileLocation = "blanktest.csv";
    QFile f(csvfileLocation);

    if (!f.open(QIODevice::ReadOnly)){
     qDebug()<<"didnt open file";
     return;
    }
    QString data;
    QTextStream in(&f);
    //file opened successfully
    while(!in.atEnd()){
     data.append(in.readLine()+"\n");
    }
    f.close();

//    qDebug()<<data.split("\n")[0];
    XYGrid<float> x(data);
    FAHVector3 hp1(0.0 ,    0.0,    0.0);
    FAHVector3 hp2(0.0 ,    50.0,    10);
    FAHVector3 fp1(50.0,    0.0,    0.0);
    FAHVector3 fp2(50.0 ,   50.0,   10.0);
    QVector<FAHVector3> planeAndCent = makePostingPlane(hp1,hp2,fp1,fp2);
    printPoint(planeAndCent[0]);
    printPoint(planeAndCent[1]);
    projectGridOntoPlane(planeAndCent[0],planeAndCent[1], &x);


    QFile f2("mapped.csv");
    if (!f2.open(QIODevice::WriteOnly)){
     qDebug()<<"didnt open file";
     return;
    }
    QTextStream out(&f2);
    out<<x.toCSV();
    f2.close();
}

void TestmakeHeightMap(){

    QString csvfileLocation = "scan.csv";
    QFile f(csvfileLocation);

    if (!f.open(QIODevice::ReadOnly)){
     qDebug()<<"didnt open file";
     return;
    }
    QString data;
    QTextStream in(&f);
    //file opened successfully
    while(!in.atEnd()){
     data.append(in.readLine()+"\n");
    }
    f.close();

    XYGrid<float> x(data);

    QImage img = makeHeightMap(&x);
    img.save("heightmap.jpeg");
}


void TestOrthoticsRx(){
    ScanManager* sm = new ScanManager();
    OrthoticManager* om = new OrthoticManager(sm);
    OrthoticController* oc = new OrthoticController(om);
    OrthoticControllerTester* oct = new OrthoticControllerTester();

    float offset = 3.0;

    QVector< FAHVector3 > forePts;
    forePts.append(FAHVector3(105.0+offset,60.0,0));
    forePts.append(FAHVector3(120.0+offset,90.0,0));
    forePts.append(FAHVector3(115.0+offset,125.0,0));
    forePts.append(FAHVector3(85.0+offset,130.0,0));

    QVector< FAHVector3 > healPts;
    healPts.append(FAHVector3(35.0+offset,50.0,0));
    healPts.append(FAHVector3(19.0+offset,70.0,0));
    healPts.append(FAHVector3(26.0+offset,103.0,0));

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
    tc.density = Top_Coat::kLow;
    tc.depth=10;
    tc.style=Top_Coat::kCloth;
    tc.thickness=11;

    QString scanid = sm->scanIds()[0];
    qDebug()<<"using scan: "<<scanid;

    oct->connect(oc,SIGNAL(boundaryLoopUpdated(FAHLoopInXYPlane*)),oct,SLOT(boundaryLoopUpdated(FAHLoopInXYPlane*)));
    oct->connect(oc,SIGNAL(scanImageGenerated(QImage)),oct,SLOT(scanImageGenerated(QImage)));
    oct->connect(oc,SIGNAL(stlsGenerated(QList<View_3D_Item>)),oct,SLOT(stlsGenerated(QList<View_3D_Item>)));


    oc->setScan(scanid);
    oc->setBorderPoints(healPts, forePts);

    oc->setTopCoat(tc);

    oc->processBoundary();

    oc->setPosting(forpost);
    oc->setPosting(rearpost);

}

void TestOrthoticsRxThroughGCode(){
    QSettings s;
    s.setValue("printing/slicer","C:\\Program Files\\Repetier-Host\\Slic3r\\Slic3r-console.exe");
    s.setValue("printing/valving-python-script","toValve.py");
    s.setValue("printing/merge-python-script","merge.py");


    ScanManager* sm = new ScanManager();
    OrthoticManager* om = new OrthoticManager(sm);
    OrthoticController* oc = new OrthoticController(om);
    OrthoticControllerTester* oct = new OrthoticControllerTester();



    /// LOAD WHAT HTE USER WOULD SET
    float offset = 3.0;
    QVector< FAHVector3 > forePts;
    forePts.append(FAHVector3(105.0+offset,60.0,0));
    forePts.append(FAHVector3(120.0+offset,90.0,0));
    forePts.append(FAHVector3(115.0+offset,125.0,0));
    forePts.append(FAHVector3(85.0+offset,130.0,0));

    QVector< FAHVector3 > healPts;
    healPts.append(FAHVector3(35.0+offset,50.0,0));
    healPts.append(FAHVector3(19.0+offset,70.0,0));
    healPts.append(FAHVector3(26.0+offset,103.0,0));

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

    QString scanid = sm->scanIds()[0];
    qDebug()<<"using scan: "<<scanid;

    oct->connect(oc,SIGNAL(boundaryLoopUpdated(FAHLoopInXYPlane*)),oct,SLOT(boundaryLoopUpdated(FAHLoopInXYPlane*)));
    oct->connect(oc,SIGNAL(scanImageGenerated(QImage)),oct,SLOT(scanImageGenerated(QImage)));
    oct->connect(oc,SIGNAL(stlsGenerated(QList<View_3D_Item>)),oct,SLOT(stlsGenerated(QList<View_3D_Item>)));


    oc->setScan(scanid);
    oc->setBorderPoints(healPts, forePts);

    oc->setTopCoat(tc);

    oc->processBoundary();

    oc->setPosting(forpost);

//    pjt->connect(oc,SIGNAL(printJobInputs(printjobinputs)),pjt,SLOT(printJobInputs(printjobinputs)));
    oc->setPosting(rearpost);

    FAHLoopInXYPlane* c = circle(90.0,90.0,10.0);
    writeLoopToXDFL(c,"circle.xdfl");
    Manipulation m;
    m.stiffness=10;
    m.depth=0;
    m.thickness = 3;
    m.outerloop = c;
    oc->addManipulation(m);

    PrintJobController* pjc = new PrintJobController(oc->getOrthotic());
//    PrintJobTester* pjt = new PrintJobTester(pjc);
    pjc->RunPrintJob();

}
