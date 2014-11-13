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
    forfoot_ = Posting();
    rearfoot_=Posting();
    topcoatgrid = new XYGrid<float>();
    shellgrid = new XYGrid<float>();


}

Orthotic::Orthotic(QString filename):foot_(Orthotic::kRight),bottom_(Orthotic::kFlat){
    filename_=filename;
    id_=QUuid::createUuid();
    scan_ = new Scan();
    topcoatgrid = new XYGrid<float>();
    shellgrid = new XYGrid<float>();
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
    for(unsigned int i=0;i<mchildren.length();i++){
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
        }else if("healpoints"==name){
            healPts_ = pointsFromNode(mchild.childNodes());
        }else if ("forepoints" == name){
            forePts_ = pointsFromNode(mchild.childNodes());
        }else if ("topcoat" == name){
            tc_ = topCoatFromNode(mchild);
        }
    }
}

void Orthotic::writeToDisk(){//writes XML and makes Scan write to disk


    QDomDocument d("OrthoticFile");



    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly)){return;}

    //scan_->writeToDisk();

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


//    Boarder loop
    QDomElement borderEl = d.createElement("boundaryloop");
    QDomNode bl=nodeFromLoop(boundaryloop_);
    borderEl.appendChild(bl);
    node.appendChild(borderEl);


    // Manipulations added
    for(int i=0;i<manipulations_.size();i++){
//        Manipulation m=manipulations_.at(i);
//        QDomElement mnode = d.createElement("Manipulation");
//        QDomElement typeEl = d.createElement("type");
//        typeEl.appendChild(d.createTextNode(QString::number(m.type)));
//        mnode.appendChild(typeEl);

//        QDomElement stiffEl = d.createElement("stiffness");
//        stiffEl.appendChild(d.createTextNode(QString::number(m.stiffness)));
//        mnode.appendChild(stiffEl);

//        QDomElement depthEl = d.createElement("depth");
//        depthEl.appendChild(d.createTextNode(QString::number(m.depth)));
//        mnode.appendChild(depthEl);

//        QDomElement thicknessEl = d.createElement("thickness");
//        thicknessEl.appendChild(d.createTextNode(QString::number(m.thickness)));
//        mnode.appendChild(thicknessEl);

//        QDomElement locationEl = d.createElement("location");
//        QDomElement xEl = d.createElement("x");
//        QDomElement yEl = d.createElement("y");
//        xEl.appendChild(d.createTextNode(QString::number(m.location.x)));
//        yEl.appendChild(d.createTextNode(QString::number(m.location.y)));
//        locationEl.appendChild(xEl);
//        locationEl.appendChild(yEl);
//        mnode.appendChild(locationEl);


        //        QDomElement outerEl = d.createElement("outerloop");
        //        QDomNode ol=nodeFromLoop(m.outerloop);
        //        outerEl.appendChild(ol);
        //        mnode.appendChild(outerEl);

//        QDomElement innerEl = d.createElement("innerloops");
//        for(int j=0; j<m.innerloops.size();j++){
//            innerEl.appendChild(nodeFromLoop(m.innerloops.at(j)));
//        }
//        mnode.appendChild(innerEl);
//        node.appendChild(mnode);
        node.appendChild(manipulations_[i].toNode());
    }

    //  QVector< FAHVector3 > healPts_;
    QVector< QDomNode > heal_nodes = nodeListFromVector(healPts_);
    QDomElement healnode = d.createElement("healpoints");
    for(int i=0; i<heal_nodes.size();i++){
        healnode.appendChild(heal_nodes.at(i));
    }
    node.appendChild(healnode);

    //  QVector< FAHVector3 > forePts_;
    QVector< QDomNode > fore_nodes = nodeListFromVector(forePts_);
    QDomElement forenode = d.createElement("forepoints");
    for(int i=0; i<fore_nodes.size();i++){
        forenode.appendChild(fore_nodes.at(i));
    }
    node.appendChild(forenode);

    //  Top_Coat tc_;
    QDomNode tc_node = nodeFromTopCoat(tc_);
    node.appendChild(tc_node);


    d.appendChild(node);
    QTextStream f(&file);
    f<<d.toString();
    file.close();

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
    return id_;
}


void Orthotic::setTopCoat(Top_Coat tc){
    tc_=tc;
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

void Orthotic::setBorderPoints(QVector< FAHVector3 > healPts, QVector< FAHVector3 > forePts){
    healPts_=healPts;
    forePts_ = forePts;
//    foreach(FAHVector3 pt,healPts){
//        healPts_.append(FAHVector3(pt));
//    }

//    foreach(FAHVector3 pt,forePts){
//        forePts_.append(FAHVector3(pt));
//    }
}

QVector< FAHVector3 > Orthotic::getHealPoints(){return healPts_;}
QVector< FAHVector3 > Orthotic::getForePoints(){return forePts_;}
Top_Coat Orthotic::getTopCoat(){return tc_;}
QVector<Manipulation> Orthotic::getManipulations(){return manipulations_;}


void Orthotic::setBoundary(FAHLoopInXYPlane* loop){boundaryloop_=loop;}
FAHLoopInXYPlane* Orthotic::getLoop(){return boundaryloop_;}

void Orthotic::setFootType(foot_type t){foot_=t;}
Orthotic::foot_type Orthotic::getFootType(){return foot_;}


