#include "patientmanager.h"
#include <QTextStream>
#include <QDebug>
#include <QSettings>

PatientManager::PatientManager(QObject *parent) :
    QObject(parent)
{
    QSettings settings;

    dir_= settings.value("patient-directory",QDir::currentPath()).toString();
    filename_ = settings.value("patient-file","patients.xml").toString();
    rescan();
}


Patient PatientManager::getByID(QString id){
    Patient original = getOriginalByID(id);
    return Patient(original);
}

Patient PatientManager::getByOrthoticsID(QString id){
    Patient p;
    for(int i=0;i<patients_.size();i++){
        p = patients_[i];
        for(int j=0; j<p.rxs.size();j++){
            if(id ==p.rxs.at(j).rightOrthoticId){
                return Patient(p);
            }else if(id==p.rxs.at(j).leftOrthoticId){
                return Patient(p);
            }
        }
    }
    return Patient(p);
}


void PatientManager::updateRx(QString PatientID, Rx r){
    Patient p = getOriginalByID(PatientID);
    int index = patients_.indexOf(p);
    patients_[index].rxs.removeAll(r);
    patients_[index].rxs.append(r);
    writeToDisk();

}

Patient PatientManager::getOriginalByID(QString id){
    Patient p;
    for(int i=0;i<patients_.size();i++){
        p=patients_.at(i);
        if(p.id==id){
            return patients_[i];
        }
        qDebug()<<"ID: "<<p.id;
    }
    return Patient();
}

bool PatientManager::hasPatientID(QString id){
    for(int i=0;i<patients_.size();i++){
        if(patients_.at(i).id==id){
            return true;
        }
    }
    return false;
}

void PatientManager::addPatient(Patient P){
    if(!hasPatientID(P.id)){
        patients_.append(P);
        writeToDisk();
    }
}

void PatientManager::removePatient(QString PatientID){
//    qDebug()<<"Patients: "<<patients_.size();
    Patient p = getOriginalByID(PatientID);
    patients_.removeOne(p);
//    qDebug()<<"Patients: "<<patients_.size();
    writeToDisk();
}

void PatientManager::rescan(){
    QDomDocument d("PatientData");

    QFile file(filename_); //TODO: implement QDir dir_.path()+
    if (!file.open(QIODevice::ReadOnly)){return;}

    if (!d.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement usersEl = d.documentElement();
    if(!("patients"==usersEl.nodeName().toLower())){return;}

    QDomNodeList userEls = usersEl.childNodes();
    for(int i=0;i<userEls.size();i++){
        QDomNode mchild = userEls.at(i);
        if(!mchild.isElement()){continue;}
        if(("patient"==mchild.nodeName().toLower())){
            patients_.append(patientFromNode(mchild));
            qDebug()<<"LOADED ID: "<<patientFromNode(mchild).id;
        }
    }

}


void PatientManager::writeToDisk(){
    QDomDocument d("PatientFile");

    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly)){return;}

    QDomElement node = d.createElement("Patients");

    for(int i=0; i<patients_.size(); i++){
        node.appendChild(nodeFromPatient(patients_.at(i)));
    }
    d.appendChild(node);

    QTextStream f(&file);
    f<<d.toString();
    file.close();
}



QList<PatientSearch> PatientManager::getPatients(){
    QList<PatientSearch> PSList;
    for(int i=0; i<patients_.size(); i++){
        Patient p = patients_.at(i);
        PatientSearch ps;
        ps.id=p.id;
        ps.name = p.name;
        QList<QDate> dates;
        QStringList docIds;

        for(int j=0; j<p.rxs.size(); j++){
            Rx r = p.rxs.at(j);
            dates.append(r.date);
            if(!docIds.contains(r.docId)){docIds.append(r.docId);}
        }
        ps.docIds=docIds;
        ps.dates = dates;
        PSList.append(ps);
    }
    return PSList;
}
