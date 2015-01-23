#include "patientdata.h"
#include "orthotic.h"

Patient::Patient():id(QUuid::createUuid().toString()),gender(kMale),name(Name()){}


Patient::Patient(const Patient& p):id(p.id),gender(p.gender),name(p.name) {
    for(int i=0;i<p.rxs.size();i++){
        rxs.append(Rx(p.rxs.at(i)));
    }
}

bool Patient::operator==(Patient p){
    if (id==p.id){
        return true;
    }
    return false;
}

Rx::Rx():docId(""),date(QDate::currentDate()),note(""),shoesize(0),
    leftScanId(""),rightScanId(""),
    leftOrthoticId(""),rightOrthoticId(""){}

Rx::Rx(const Rx& r):docId(r.docId),date(r.date),note(r.note),shoesize(r.shoesize),
    leftScanId(r.leftScanId),rightScanId(r.rightScanId),leftOrthoticId(r.leftOrthoticId),
    rightOrthoticId(r.rightOrthoticId){}

bool Rx::operator==(Rx r){
    int count=0;
    if(r.date==date){count++;}
    if(r.docId==docId){count++;}
    if(r.leftScanId==leftScanId){count++;}
    if(r.leftOrthoticId==leftOrthoticId){count++;}
    if(r.rightScanId==rightScanId){count++;}
    if(r.rightOrthoticId==rightOrthoticId){count++;}
    if(count>3){
        return true;
    }
    return false;
}


QDomNode nodeFromRx(Rx r){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("Rx");

    QDomElement note = d.createElement("note");
    note.appendChild(d.createTextNode(r.note));
    node.appendChild(note);

    QDomElement shoe = d.createElement("shoesize");
    shoe.appendChild(d.createTextNode(QString::number(r.shoesize)));
    node.appendChild(shoe);

    node.appendChild(nodeFromDate(r.date));

    QDomElement doctor = d.createElement("doctor");
    doctor.setAttribute("id",r.docId);
    node.appendChild(doctor);

    QDomElement scanr = d.createElement("scan");
    scanr.setAttribute("id",r.rightScanId);
    scanr.setAttribute("foot",QString::number(Orthotic::kRight));
    node.appendChild(scanr);

    QDomElement scanl = d.createElement("scan");
    scanl.setAttribute("id",r.leftScanId);
    scanl.setAttribute("foot",QString::number(Orthotic::kLeft));
    node.appendChild(scanl);

    QDomElement orthor = d.createElement("orthotic");
    orthor.setAttribute("id",r.rightOrthoticId);
    orthor.setAttribute("foot",QString::number(Orthotic::kRight));
    node.appendChild(orthor);

    QDomElement orthol = d.createElement("orthotic");
    orthol.setAttribute("id",r.leftOrthoticId);
    orthol.setAttribute("foot",QString::number(Orthotic::kLeft));
    node.appendChild(orthol);

    return node;
}



Rx rxFromNode(QDomNode node){
    Rx r;
    if(!("rx"==node.nodeName().toLower())){return r;}


    QDomNodeList mchildren = node.childNodes();
    for(int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName();

        if("date"==name){
            r.date =dateFromNode(mchild);
        }else if("note"==name){
            r.note=el.text();
        }else if("shoesize"==name){
            r.shoesize=el.text().toFloat();
        }else if("doctor"==name){
            r.docId = el.attribute("id","");
        }else if("scan"==name){
            QString atrib = el.attribute("foot",QString::number(Orthotic::kNull));
            Orthotic::foot_type t= static_cast<Orthotic::foot_type>(atrib.toUInt());
            QString id = el.attribute("id","");
            if(!id.isNull()){
                if(Orthotic::kRight==t){
                    r.rightScanId=id;
                }else if(Orthotic::kLeft==t){
                    r.leftScanId=id;
                }
            }
        }else if("orthotic"==name){
            Orthotic::foot_type t= static_cast<Orthotic::foot_type>(el.attribute("foot",QString::number(Orthotic::kNull)).toUInt());
            QString id = el.attribute("id","");
            if(!id.isNull()){
                if(Orthotic::kRight==t){
                    r.rightOrthoticId=id;
                }else if(Orthotic::kLeft==t){
                    r.leftOrthoticId=id;
                }
            }
        }

    }
    return r;
}


QDomNode nodeFromPatient(Patient p){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("patient");
    node.setAttribute("id",p.id);

    node.appendChild(nodeFromName(p.name));

    QDomElement gender = d.createElement("gender");
    gender.appendChild(d.createTextNode(QString::number(p.gender)));
    node.appendChild(gender);

    QDomElement rxs = d.createElement("Rxs");
    qDebug()<<"SIZE"<<p.rxs.size();
    for(int i=0; i<p.rxs.size(); i++){
        rxs.appendChild(nodeFromRx(p.rxs[i]));
    }
    node.appendChild(rxs);
    return node;
}

Patient patientFromNode(QDomNode node) {
    Patient p;
    if(!("patient"==node.nodeName().toLower())){return p;}

    p.id=node.toElement().attribute("id","");

    QDomNodeList mchildren = node.childNodes();
    for(int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName().toLower();

        if("name"==name){
            p.name=nameFromNode(mchild);
        }else if("gender"==name){
            p.gender= static_cast<Patient::ShoeGender>(el.text().toUInt());
        }else if("rxs"==name){
            QDomNodeList rxList = mchild.childNodes();
            for(int j=0;j<rxList.size();j++){

                QDomNode rxEl =rxList.at(j);
                if(!rxEl.isElement()){continue;}
                p.rxs.append(rxFromNode(rxEl));
            }
        }
    }
    return p;

}
