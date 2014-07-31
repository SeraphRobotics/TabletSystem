#include "usbmanager.h"
#include <QFile>
#include <QDebug>
#include <QDomElement>
#include <QDomNode>

USBManager::USBManager(QObject *parent) :
    QObject(parent),USBDir_("")
{

}


void USBManager::usbAdded(QString f)
{
    if(USBDir_!=""){
        qDebug()<<"looking for: "<<USBDir_;
        return;
    }
    USBDir_ = f;
    proccessUSBDrive();
    emit usbConnected();
    emit uiUSBItemsUpdated(items_);
}

void USBManager::usbDisconnected(QString f)
{
    if (f != USBDir_){
        qDebug()<<"looking for: "<<USBDir_ <<" Got: <<"<<f;
        return;
    }
    USBDir_="";
    items_.clear();
    emit usbDisconnected();
    emit uiUSBItemsUpdated(items_);
}

void USBManager::proccessUSBDrive()
{

    QDomDocument d;

    QString filename;
    filename = USBDir_+"USBData.xml";
    QFile usbdata(filename);

    if (!usbdata.open(QFile::ReadOnly)) {
        qDebug() << "Could not open file.";
    }

    if (!d.setContent(&usbdata)) {
        usbdata.close();
        return;
    }
    usbdata.close();

    QDomElement usbdataEl = d.documentElement();
    if(!("usbdata" == usbdataEl.nodeName().toLower())){
        qDebug()<<"NO DATA IN FILE";
        return;
    }

    QDomNodeList usbdataEls = usbdataEl.childNodes();
    for(int i=0;i<usbdataEls.size();i++){
        QDomNode mchild = usbdataEls.at(i);
        if(!mchild.isElement()){continue;}
        items_.append(makeUSBItemFromNode(mchild));
    }
}


UI_USB_Item makeUSBItemFromNode(QDomNode node){
    UI_USB_Item returnItem;



    QDomNodeList els =  node.childNodes();
    if(("scan"==node.nodeName().toLower()))
    {
        returnItem.type=UI_USB_Item::kScan;
        qDebug()<<"type: scan";
    }else if(("printjob"==node.nodeName().toLower()))
    {
        returnItem.type=UI_USB_Item::kRx;
        qDebug()<<"type: RX";
    }

    for(int i=0;i<els.size();i++){
        QDomNode mchild = els.at(i);
        if(!mchild.isElement()){continue;}
        if( "folder"==mchild.nodeName().toLower() ){
            returnItem.id = mchild.toElement().text();
            qDebug()<<"id: "<< mchild.toElement().text();
        }else if( "datetime"==mchild.nodeName().toLower() ){
            QDateTime time;
            time.setMSecsSinceEpoch(mchild.toElement().text().toLongLong());
            returnItem.datetime = time;
            qDebug()<<"read: "<<mchild.toElement().text();
            qDebug()<<"time: "<< returnItem.datetime.toString();

        }else if( "file"==mchild.nodeName().toLower() ){
            returnItem.id = mchild.toElement().text().split(".")[0];
            qDebug()<<"id: "<< returnItem.id;
        }
    }
    return returnItem;
}
