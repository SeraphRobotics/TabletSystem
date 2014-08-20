#include "orthoticscontroller.h"
#include "processingfunctions.h"
#include "stlgeneration.h"

OrthoticController::OrthoticController(OrthoticManager* om, QObject *parent) :
    QObject(parent),om_(om)
{
}

void OrthoticController::setScan(QString scan_id){
    if(!om_->sm_->hasScan(scan_id)){return;}
    orth_=om_->newOrthotic(scan_id);
    emit scanImageGenerated(makeHeightMap (orth_->getScan()->getProcessedXYGrid()));
}
void OrthoticController::setOrthotic(QString orthotic_id){
    if(!om_->hasOrthotic(orthotic_id)){return;}
    orth_=om_->getOrthotic(orthotic_id);
    emit scanImageGenerated(makeHeightMap (orth_->getScan()->getProcessedXYGrid()));

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
    STLMesh* m = makeSTLfromScanSection(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),QList<FAHLoopInXYPlane*>());
}

void OrthoticController::processBoundary(){
    FAHVector3 minpt1 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints()[0],orth_->getHealPoints()[-1]);
    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints()[0],orth_->getForePoints()[-1]);
    FAHVector3 planeVec = normFrom3Pts(minpt1,minpt2,orth_->getForePoints()[-1]);
    projectGridOntoPlane(planeVec,orth_->getForePoints()[-1],orth_->getScan()->getProcessedXYGrid());
    thresholdWithLoop(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop());
}

void OrthoticController::setTopCoat(Top_Coat tc){
    orth_->setTopCoat(tc);
}

void OrthoticController::addManipulation(Manipulation m){}
void OrthoticController::undo(){}
void OrthoticController::redo(){}

void OrthoticController::setPosting(Posting p){
    orth_->setPosting(p);
    XYGrid<float>*  posted = new XYGrid<float>(orth_->getScan()->getProcessedXYGrid());
    QVector<FAHVector3> planeAndCent = makePostingPlane(orth_->getHealPoints()[0],
                                                        orth_->getHealPoints()[-1],
                                                        orth_->getForePoints()[0],
                                                        orth_->getForePoints()[-1]);


    projectGridOntoPlane(planeAndCent[0],planeAndCent[1], posted);
    orth_->getScan()->setPostedGrid(posted);


}

Border OrthoticController::borderFromLoop(FAHLoopInXYPlane* loop){}
QList<QPointF> OrthoticController::qpointfListFromFAHVector3(QVector<FAHVector3> points){}
