#include "orthoticmanager.h"
#include <QSettings>

OrthoticManager::OrthoticManager(ScanManager* sm, QObject *parent) :
    QObject(parent),sm_(sm)
{
    QSettings settings;

    extension_ = settings.value("ortho-extension","ortho").toString();
    dir_= settings.value("ortho-directory",QDir::currentPath()).toString();

    QStringList name_filters;
    name_filters<< "*"+extension_; //get from QSettings

    dir_.setFilter(QDir::Files|QDir::NoDotAndDotDot);
    dir_.setNameFilters(name_filters);

    updateList();
}

QStringList OrthoticManager::getList(){
    return idlist_;
}


Orthotic* OrthoticManager::getOrthotic(QString id){
    if(!hasOrthotic(id)){
        return new Orthotic();
    }
    QString filename = dir_.absolutePath()+"/"+id+QString(".")+extension_;
    //qDebug()<<filename;
    Orthotic* Orth = new Orthotic(filename);
    QString sid = Orth->getScanID();
    Scan* s = sm_->getScan(sid);
    Orth->setScan(s);

    return Orth;
}

Orthotic* OrthoticManager::newOrthotic(QString scan_id){
    Scan* s = sm_->getScan(scan_id);
    Orthotic* orth = new Orthotic();
    orth->setScan(s);
    return orth;
}

bool OrthoticManager::hasOrthotic(QString id){
    updateList();
    if (idlist_.contains(id)){return true;}
    return false;
}


void OrthoticManager::updateList(){
    QStringList files = dir_.entryList();
    idlist_.clear();
    for(int i=0;i<files.size();i++){
        idlist_.append(files[i].split(".")[0]);
    }
//    qDebug()<<idlist_;
}

