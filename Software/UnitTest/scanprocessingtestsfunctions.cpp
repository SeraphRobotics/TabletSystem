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
    rearpost.verticle=0;
    rearpost.varus_valgus=Posting::kVargus;
    rearpost.for_rear=Posting::kRearFoot;

    QVector<FAHVector3> heals = transformPointsWithPosting(hp1,hp2,rearpost);
    QVector<FAHVector3> fors = transformPointsWithPosting(fp1,fp2,forpost);
    FAHVector3 plane = makePostingPlane(heals[0],heals[1],fors[0],fors[1]);
    printPoint(plane);

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
