#include "orthotic.h"
#include <QFile>
#include <QTextStream>

Orthotic::Orthotic(QObject *parent) :
    QObject(parent),foot_(Orthotic::kRight),bottom_(Orthotic::kFlat)
{
    id_=QUuid::createUuid();
    filename_ = id_.toString()+QString(".ortho");
    scan_=new Scan();
    scanid_=scan_->getID();

}

Orthotic::Orthotic(QString filename):foot_(Orthotic::kRight),bottom_(Orthotic::kFlat){
    filename_=filename;
    id_=QUuid::createUuid();
    scan_ = new Scan();
    QDomDocument d("OrthoticFile");

    QFile file(filename_);
    if (!file.open(QIODevice::ReadOnly)){return;}

    if (!d.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement ortho = d.documentElement();
    if(!("orthotic"==ortho.nodeName().toLower())){return;}
    QString id = ortho.attribute("id",id_.toString());
    id_=QUuid(id);

    //ADD SCANID HERE
    scanid_ = ortho.attribute("scanid","");
//    QTimer::singleShot(0,this,SLOT(requestScanData()));

    QDomNodeList mchildren = ortho.childNodes();
    for(int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName().toLower();

        if("foot"==name){
            foot_= static_cast<Orthotic::foot_type>(el.text().toUInt());
        }else if("bottom"==name){
            bottom_= static_cast<Orthotic::bottom_type>(el.text().toUInt());
        }else if("posting"==name){
            Posting p = nodeToPosting(mchild);
            if(Posting::kForFoot==p.for_rear){
                forfoot_=p;
            }else{
                rearfoot_=p;
            }
        }else if("boundaryloop"==name){
            QDomNode loop = mchild.namedItem("loop");
            boundaryloop_=loopFromNode(loop);
        }else if("manipulations"==name){
            QDomNodeList manips = mchild.childNodes();
            for(int k=0;k<manips.size();k++){
                QDomNode manip = manips.at(k);
                if("manipulation" == manip.nodeName().toLower()){
                    manipulations_.append(Manipulation(manip));
                }
            }
        }
    }
}

void Orthotic::requestScanData(){
    emit needScan(scanid_);
}

Scan *Orthotic::getScan(){//placeholder
    return scan_;
}

QString Orthotic::getScanID(){
    return scanid_;
}

QString Orthotic::getId(){
    return id_.toString();
}

void Orthotic::addManipulation(Manipulation m){
    manipulations_.append(m);
    emit manipulated();
}
void Orthotic::undo(){
    undo_stack_.push(manipulations_.last());
    manipulations_.pop_back();
    emit manipulated();
}
void Orthotic::redo(){
    addManipulation(undo_stack_.pop());
}

void Orthotic::writeToDisk(){//writes XML and makes Scan write to disk


    QDomDocument d("OrthoticFile");


    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly)){return;}

    scan_->writeToDisk();

    QDomElement node = d.createElement("Orthotic");
    node.setAttribute(QString("id"),id_.toString());
    //SET SCAN ID HERE
    node.setAttribute(QString("scanid"),scan_->getID());

    //Foot type
    QDomElement footTypeEl = d.createElement("foot");
    footTypeEl.appendChild(d.createTextNode(QString::number(foot_)));
    node.appendChild(footTypeEl);

    //Bottom Type
    QDomElement bottomTypeEl = d.createElement("bottom");
    bottomTypeEl.appendChild(d.createTextNode(QString::number(bottom_)));
    node.appendChild(bottomTypeEl);

    //posting
    QDomNode fpostEl = postingToNode(forfoot_);
    node.appendChild(fpostEl);

    QDomNode rpostEl = postingToNode(rearfoot_);
    node.appendChild(rpostEl);

    QDomElement borderEl = d.createElement("boundaryloop");
    QDomNode bl=nodeFromLoop(boundaryloop_);
    borderEl.appendChild(bl);
    node.appendChild(borderEl);

    for(int i=0;i<manipulations_.size();i++){
        node.appendChild(manipulations_[i].toNode());
    }

    d.appendChild(node);
    QTextStream f(&file);
    f<<d.toString();
    file.close();

}
void Orthotic::setScan(Scan* scan){///placeholder for scan class
    scan_=scan;
    scanid_=scan->getID();
}

void Orthotic::setBottomType(bottom_type b){
    bottom_=b;
}
Orthotic::bottom_type Orthotic::getBottomType(){
    return bottom_;
}


Posting Orthotic::getForFootPosting(){
    return forfoot_;
}
Posting Orthotic::getRearFootPosting(){
    return rearfoot_;
}

void Orthotic::setPosting(Posting p){
    if(Posting::kForFoot==p.for_rear){
        forfoot_=p;
    }else{
        rearfoot_=p;
    }
}

void Orthotic::setBoundary(FAHLoopInXYPlane* loop){boundaryloop_=loop;}
FAHLoopInXYPlane* Orthotic::getLoop(){return boundaryloop_;}

void Orthotic::setFootType(foot_type t){foot_=t;}
Orthotic::foot_type Orthotic::getFootType(){return foot_;}

QDomNode postingToNode(Posting p){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("posting");

    QDomElement sideEl = d.createElement("side");
    sideEl.appendChild(d.createTextNode(QString::number(p.for_rear)));
    node.appendChild(sideEl);

    QDomElement angleEl = d.createElement("angle");
    angleEl.appendChild(d.createTextNode(QString::number(p.angle)));
    node.appendChild(angleEl);

    QDomElement verticleEl = d.createElement("verticle");
    verticleEl.appendChild(d.createTextNode(QString::number(p.verticle)));
    node.appendChild(verticleEl);

    QDomElement directionEl = d.createElement("direction");
    directionEl.appendChild(d.createTextNode(QString::number(p.varus_valgus)));
    node.appendChild(directionEl);

    return node;
}

Posting nodeToPosting(QDomNode node){
    Posting p;
    p.angle=0;
    p.varus_valgus=Posting::kValgus;
    p.for_rear=Posting::kForFoot;
    p.verticle=0;

    if(!("posting"==node.nodeName().toLower())){return p;}


    QDomNodeList mchildren = node.childNodes();
    for(int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName();

        if("side"==name){
            p.for_rear= static_cast<Posting::side>(el.text().toUInt());
        }else if("angle"==name){
            p.angle=el.text().toFloat();
        }else if("verticle"==name){
            p.verticle=el.text().toFloat();
        }else if("direction"==name){
            p.varus_valgus=static_cast<Posting::direction>(el.text().toUInt());
        }
    }
    return p;
}
