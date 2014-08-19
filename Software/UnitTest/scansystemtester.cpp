#include "scansystemtester.h"
#include <QListIterator>
#include <QDebug>

ScanSystemTester::ScanSystemTester(QObject *parent) :
    QObject(parent)
{
    um_ = new USBMinder();
    manager_ = new USBManager();
    manager_->connect(um_,SIGNAL(usbadded(QString)),manager_,SLOT(usbAdded(QString)));
    manager_->connect(um_,SIGNAL(usbRemoved(QString)),manager_,SLOT(usbDisconnected(QString)));

    manager_->connect(manager_,SIGNAL(uiUSBItemsUpdated(QList<UI_USB_Item>)),this,SLOT(USBItems(QList<UI_USB_Item>)));
    manager_->connect(manager_,SIGNAL(usbConnected()),this,SLOT(USBConnected()));

    sm_ = new ScanManager();
}

void ScanSystemTester::USBConnected(){
    qDebug()<<"connected";
}

void ScanSystemTester::USBItems(QList<UI_USB_Item> f){
//    qDebug()<<"Found "<<f.size()<<" items";
    QListIterator<UI_USB_Item> i(f);
    while(i.hasNext()){
        UI_USB_Item item = i.next();
        if(item.type==UI_USB_Item::kScan){
//            qDebug()<<"processing Scan: "<<item.id;
            processScanItem(item);
        }
    }
}

void ScanSystemTester::processScanItem(UI_USB_Item item){
//    qDebug()<<"item: "<<item.datetime;
    QString dir = manager_->getLocation(item.id);
//    qDebug()<<"dir: "<<dir;
    ScanDataProcesser* sdp = new ScanDataProcesser(item.id,dir);
    connect(sdp,SIGNAL(scanProcessed(Scan*)),sm_,SLOT(addScan(Scan*)));
    sdp->processScan();
//    connect(sdp,SIGNAL(scanProcessed(Scan*)),sdp,SLOT(deleteLater()));
}

