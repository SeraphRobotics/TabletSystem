#include "usbmanager.h"

#include <QFile>
#include <QDebug>
#include <QDomElement>
#include <QDomNode>
#include <QDir>
#include <QMapIterator>
#include <QStorageInfo>
#include <QTimer>

#include <stdio.h>
#include <iostream>

#include "usbminder.h"

USBManager::USBManager(QObject *parent) :
    QObject(parent),USBDir_(""),Rx_extension(".gcode")
{
    USBMinder *usbMinder = new USBMinder(this);
    connect(usbMinder, &USBMinder::usbadded,
            this, &USBManager::processUsbAdded);
    connect(usbMinder, &USBMinder::usbRemoved,
            this, &USBManager::usbRemoved);
}

void USBManager::processUsbAdded(const QString &f)
{
    QTimer::singleShot(2000, [this, f]
    {
        usbAdded(f);
    });
}

void USBManager::usbAdded(QString f)
{
    if (USBDir_ != "") {
        qDebug()<<"looking for: "<<USBDir_;
        return;
    }

    USBDir_ = f;
    proccessUSBDrive();
    emit usbConnected(mountDir(f));
    emit uiUSBItemsUpdated(items_.values());
    qDebug()<<"items: "<<items_.size();
}

void USBManager::usbRemoved(QString f)
{
    if (f != USBDir_ ){
        qDebug() << "looking for: " << USBDir_ << " Got: <<" << f;
        return;
    }

    USBDir_="";
    items_.clear();
    emit usbDisconnected(mountDir(f));
    emit uiUSBItemsUpdated(items_.values());
    qDebug()<<"items: "<<items_.size();
}

QString USBManager::getLocation(QString id){
    bool hasid = items_.keys().contains(id);

    if( !hasid ){
        qDebug()<<"doesnt have";
        return QString();
    }

    UI_USB_Item item = items_[id];

    if (item.type== UI_USB_Item::kScan){
        return USBDir_+id;
    }else{
        return USBDir_+id+Rx_extension;
    }
}

void USBManager::addItem(const UI_USB_Item &item)
{
    items_.insert(item.id, item);
    updateUSBData();
}

void USBManager::deleteItem(QString id){
    qDebug()<<"delete: "<<id;

    /// FIND IF IT IS AN ITEM
    bool hasid = items_.keys().contains(id);

    qDebug()<<"exists? "<<hasid;
    if (!hasid){return;}



    /// IF IT EXISTS DELETE IT
    UI_USB_Item item = items_[id];
    bool worked=false;
    QString path = mountDir(USBDir_);
    if (item.type==UI_USB_Item::kScan){
        worked = removeDir(path+item.id);
        qDebug()<<"removed dir" <<item.id;
    }else{
        if (QFile::exists(path + item.id + Rx_extension) ){
            worked = QFile::remove(path + item.id + Rx_extension);
        }else{
            qDebug()<<"didnt exist";
        }

    }
    if(!worked){return;}
    items_.remove(id);
    qDebug()<<"Worked: "<<worked;

    /// REWRITE DATA
    updateUSBData();
}

void USBManager::updateUSBData(){

    QDir usbDir(mountDir(USBDir_));
    QString filename = usbDir.absoluteFilePath("USBData.xml");

    QDomDocument d("USBDataFile");

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)){
        qDebug()<<"could not open file: "<< filename;
        return;
    }

    QDomElement node = d.createElement("USBData");


    QMapIterator<QString,UI_USB_Item  > i(items_);
    while(i.hasNext()){
        i.next();
        node.appendChild(nodeFromUSBItem(i.value()));
    }

    d.appendChild(node);

    QTextStream f(&file);
    f<<d.toString();
    file.close();
}

QString USBManager::mountDir(const QString &device) const
{
#ifdef Q_OS_WIN32
    return device;
#else
    QList<QStorageInfo> volumes = QStorageInfo::mountedVolumes();

    foreach (QStorageInfo storageInfo, volumes) {
        if (QString(storageInfo.device()) == device) {
            return storageInfo.rootPath() + "/";
        }
    }

    qDebug() << "Cannot find appropriate storage for device:" << device;

    return QString();
#endif //Q_OS_WIN32
}

void USBManager::proccessUSBDrive()
{

    QDomDocument d;

    QDir usbDir(mountDir(USBDir_));
    QString filename = usbDir.absoluteFilePath("USBData.xml");
    QFile usbdata(filename);
    qDebug() << filename;

    if (!usbdata.open(QFile::ReadOnly)) {
        qDebug() << "Could not open file" << filename << ":" << usbdata.errorString();
        usbdata.close();
        USBDir_.clear();
        return;
    }

    if (!d.setContent(&usbdata)) {
        usbdata.close();
        USBDir_.clear();
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
        UI_USB_Item item = makeUSBItemFromNode(mchild);
        items_[item.id]=item;
    }
}


QDomNode nodeFromUSBItem(UI_USB_Item i){
    QDomDocument d("dummy");

    QDomElement node;

    if (i.type==UI_USB_Item::kScan){
         node = d.createElement("Scan");
         QDomElement Folder = d.createElement("Folder");
         Folder.appendChild(d.createTextNode(i.id));
         node.appendChild(Folder);
    }else{
        node = d.createElement("PrintJob");
        QDomElement Folder = d.createElement("file");
        Folder.appendChild(d.createTextNode(i.id+".gcode"));
        node.appendChild(Folder);
    }
    QDomElement datetime = d.createElement("DateTime");
    datetime.appendChild(d.createTextNode(QString::number(i.datetime.toMSecsSinceEpoch())));
    node.appendChild(datetime);
    return node;
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


bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }else{
        qDebug()<<"DIR "<<dirName<<" DOESNT EXIST";
    }
    return result;
}
