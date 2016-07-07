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
        //TODO:: btl improve this path parsing
        QString filename = dir_.absolutePath()+"/"+id+QString(".")+extension_; // not the best form. Should be from QSettings
        Scan *scan = new Scan(filename);
        scan->setId(id);
        return scan;
    }
    return new Scan();
}

bool ScanManager::hasScan(QString id){
    if (idlist_.contains(id)){return true;}
    return false;
}

void ScanManager::addScan(Scan* s){
    idlist_.append(s->getID());
    s->writeToDisk();
}

void ScanManager::updateScanList(){
    dir_.refresh();
    QStringList files = dir_.entryList();
    idlist_.clear();
    for(int i=0;i<files.size();i++){
        idlist_.append(files[i].split(".")[0]);
    }
    //qDebug()<<idlist_;
}

QStringList ScanManager::scanIds(){return idlist_;}
