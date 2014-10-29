#include "scansystemtester.h"
#include <QListIterator>
#include <QDebug>
#include "Controllers/stlgeneration.h"

ScanSystemTester::ScanSystemTester(QObject *parent) :
    QObject(parent)
{
//    um_ = new USBMinder();
//    manager_ = new USBManager();
//    manager_->connect(um_,SIGNAL(usbadded(QString)),manager_,SLOT(usbAdded(QString)));
//    manager_->connect(um_,SIGNAL(usbRemoved(QString)),manager_,SLOT(usbDisconnected(QString)));

//    manager_->connect(manager_,SIGNAL(uiUSBItemsUpdated(QList<UI_USB_Item>)),this,SLOT(USBItems(QList<UI_USB_Item>)));
//    manager_->connect(manager_,SIGNAL(usbConnected()),this,SLOT(USBConnected()));

    sm_ = new ScanManager();

    qDebug()<<"starting";

    QMap < QString, UI_USB_Item > items_;

    QDomDocument d;

    QString filename;
    filename = "ScanData.xml";
    QFile usbdata(filename);

    if (!usbdata.open(QFile::ReadOnly)) {
        qDebug() << "Could not open file."<< filename;
//        qDebug() << usbdata.
    }

    if (!d.setContent(&usbdata)) {
        usbdata.close();
        return;
    }
    usbdata.close();

    QDomElement usbdataEl = d.documentElement();
    if(!("scandata" == usbdataEl.nodeName().toLower())){
        qDebug()<<"NO DATA IN"<<filename;
        qDebug()<<"contained: "<<d.toString();
        return;
    }

    QDomNodeList usbdataEls = usbdataEl.childNodes();
    for(int i=0;i<usbdataEls.size();i++){
        QDomNode mchild = usbdataEls.at(i);
        if(!mchild.isElement()){continue;}
        UI_USB_Item item = makeUSBItemFromNode(mchild);
        items_[item.id]=item;
    }
    qDebug()<<"items: "<<items_.keys();
    USBItems(items_.values());

}

//void ScanSystemTester::USBConnected(){
//    qDebug()<<"connected";
//}

void ScanSystemTester::USBItems(QList<UI_USB_Item> f){
    QListIterator<UI_USB_Item> i(f);
    while(i.hasNext()){
        UI_USB_Item item = i.next();
        if(item.type==UI_USB_Item::kScan){
            qDebug()<<"Processing: "<<item.id;
            processScanItem(item);
        }
    }
}

void ScanSystemTester::processScanItem(UI_USB_Item item){
//    qDebug()<<"item: "<<item.datetime;
    QString dir = item.id.remove("\"");
    qDebug()<<"dir: "<<dir;
    ScanDataProcesser* sdp = new ScanDataProcesser(item.id,dir);
    connect(sdp,SIGNAL(scanProcessed(Scan*)),sm_,SLOT(addScan(Scan*)));
    sdp->processScan();

    Scan* s = sm_->getScan(sm_->scanIds().takeFirst());


    QList<FAHLoopInXYPlane*> innerLoops3;
//    innerLoops3.append(orth_->getLoop());
    FAHLoopInXYPlane* outside = new FAHLoopInXYPlane();
    outside->points.append(FAHVector3(0,0,0));
    outside->points.append(FAHVector3(1000,0,0));
    outside->points.append(FAHVector3(1000,1000,0));
    outside->points.append(FAHVector3(0,1000,0));


    STLMesh* p = makeSTLfromScanSection(s->getProcessedXYGrid(),
                                         outside,
                                         innerLoops3);
    p->scale(2,1,1.0);
    QString outname = item.id+".stl";
    stlToFile(p,outname);
    qDebug()<<"writing";




}

