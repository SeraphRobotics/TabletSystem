#include "orthoticscontroller.h"
#include "processingfunctions.h"

OrthoticController::OrthoticController(OrthoticManager* om, QObject *parent) :
    QObject(parent),om_(om)
{
}

void OrthoticController::setScan(QString scan_id){
    if(!om_->sm_->hasScan(scan_id)){return;}
    orth_=om_->newOrthotic(scan_id);
    emit scanImageGenerated(makeHeightMap (orth_->getScan()->getXYGrid()));
}
void OrthoticController::setOrthotic(QString orthotic_id){
    if(!om_->hasOrthotic(orthotic_id)){return;}
    orth_=om_->getOrthotic(orthotic_id);
    emit scanImageGenerated(makeHeightMap (orth_->getScan()->getXYGrid()));

    if(orth_->getLoop()->points.size()>4){
        emit boundaryLoopUpdated(orth_->getLoop());
        emit borderGenerated(borderFromLoop(orth_->getLoop()));
    }
    if(orth_->getHealPoints().size()>2){
        emit healPoints(qpointfListFromFAHVector3(orth_->getHealPoints()));
    }
    if(orth_->getForePoints().size()>2){
        emit frontPoints(qpointfListFromFAHVector3(orth_->getForePoints()));
    }
    emit topCoat(orth_->getTopCoat());
}


void OrthoticController::setBorderPoints(QVector< FAHVector3 > healPts, QVector< FAHVector3 > forePts){
    if(!(healPts.size()==3 && forePts.size()==4)){return;}
    orth_->setBorderPoints(healPts,forePts);
    FAHLoopInXYPlane loop = loopFromPoints(healPts,forePts);
    orth_->setBoundary(&loop);
    emit boundaryLoopUpdated(orth_->getLoop());
    emit borderGenerated(borderFromLoop(orth_->getLoop()));
}

void OrthoticController::processBoundary(){
    FAHVector3 minpt1 = minAlongLine(orth_->getScan()->getXYGrid(),orth_->getHealPoints()[0],orth_->getHealPoints()[-1]);
    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getXYGrid(),orth_->getForePoints()[0],orth_->getForePoints()[-1]);
    FAHVector3 planeVec = normFrom3Pts(minpt1,minpt2,orth_->getForePoints()[-1]);
    projectGridOntoPlane(planeVec,forePts[-1],orth_->getScan()->getXYGrid());
    thresholdWithLoop(orth_->getScan()->getXYGrid(),loop);
}

void OrthoticController::setTopCoat(Top_Coat tc){
    orth_->setTopCoat(tc);
}

void OrthoticController::addManipulation(Manipulation m){}
void OrthoticController::undo(){}
void OrthoticController::redo(){}

void OrthoticController::setPosting(Posting p){
    orth_->setPosting(p);

    QVector<FAHVector3> planeAndCent = makePostingPlane(hp1,hp2,fp1,fp2);
    printPoint(planeAndCent[0]);
    printPoint(planeAndCent[1]);
    projectGridOntoPlane(planeAndCent[0],planeAndCent[1], &x);

}

Border OrthoticController::borderFromLoop(FAHLoopInXYPlane* loop){}
QList<QPointF> OrthoticController::qpointfListFromFAHVector3(QVector<FAHVector3> points){}
