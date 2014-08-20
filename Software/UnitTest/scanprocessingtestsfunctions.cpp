#include "scanprocessingtestsfunctions.h"

#include "Controllers/processingfunctions.h"
#include "UnitTest/usbtester.h"

#include "DataStructures/usbminder.h"
#include "DataStructures/usbmanager.h"

#include "UnitTest/scansystemtester.h"
#include "Controllers/processingfunctions.h"

#include "UnitTest/debugfunctions.h"

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

    writeLoopToXDFL(plane,"test.xdfl");
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
