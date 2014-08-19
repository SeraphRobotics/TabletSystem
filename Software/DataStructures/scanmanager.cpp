#include "scanmanager.h"
#include <QDebug>

ScanManager::ScanManager(QObject *parent) :
    QObject(parent)
{

    QSettings settings;

    extension_ = settings.value("scan-extension","scan").toString();
    dir_= settings.value("scan-directory",QDir::currentPath()).toString();

    QStringList name_filters;
    name_filters<< "*"+extension_; //get from QSettings

    dir_.setFilter(QDir::Files|QDir::NoDotAndDotDot);
    dir_.setNameFilters(name_filters);

    updateScanList();
}

Scan* ScanManager::getScan(QString id){
    if(hasScan(id)){
        QString filename = id+QString(".")+extension_; // not the best form. Should be from QSettings
        return new Scan(filename);
    }
    return new Scan();
}

bool ScanManager::hasScan(QString id){
    if (idlist_.contains(id)){return true;}
    return false;
}

void ScanManager::addScan(Scan* s){
    idlist_.append(s->getID());
    qDebug()<<"writing";
    s->writeToDisk();
}

void ScanManager::updateScanList(){
    QStringList files = dir_.entryList();
    idlist_.clear();
    for(int i=0;i<files.size();i++){
        idlist_.append(files[i].split(".")[0]);
    }
//    qDebug()<<idlist_;
}
