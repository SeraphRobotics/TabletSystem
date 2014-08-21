#include "orthoticscontroller.h"
#include "processingfunctions.h"
#include "stlgeneration.h"
#include "UnitTest/debugfunctions.h"

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
    FAHLoopInXYPlane* loop = loopFromPoints(healPts,forePts,orth_->getScan()->getProcessedXYGrid()->stepSize());
    orth_->setBoundary(loop);
    writeLoopToXDFL(loop,"LOOPINSETBORDER.XDFL");
    emit boundaryLoopUpdated(orth_->getLoop());
//    emit borderGenerated(borderFromLoop(orth_->getLoop()));


}

void OrthoticController::processBoundary(){
    float slope = 63.5/101;
    float heightoffset =0;
    scaleAndOffset(orth_->getScan()->getProcessedXYGrid(),slope,heightoffset);

    FAHVector3 minpt1 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints().first(),orth_->getHealPoints().last());
    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints().first(),orth_->getForePoints().last());
    FAHVector3 planeVec = normFrom3Pts(minpt1,minpt2,orth_->getForePoints().last());
    qDebug()<<"\nplane vec";
    printPoint(planeVec);

    qDebug()<<"\nminpt1";
    printPoint(minpt1);

    qDebug()<<"\nminpt2";
    printPoint(minpt2);

    FAHVector3 cent = orth_->getForePoints().last();
    FAHVector3 d(cent.y,cent.x,cent.z);
    projectGridOntoPlane(planeVec,d,orth_->getScan()->getProcessedXYGrid());
    thresholdWithLoop(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop());
    QFile f("processed.csv");
    f.open(QFile::WriteOnly);
    QTextStream fs(&f);
    fs<<orth_->getScan()->getProcessedXYGrid()->toCSV();
    f.close();
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
    QVector<FAHVector3> planeAndCent = makePostingPlane(orth_->getHealPoints().first(),
                                                        orth_->getHealPoints().last(),
                                                        orth_->getForePoints().first(),
                                                        orth_->getForePoints().last());


    projectGridOntoPlane(planeAndCent[0],planeAndCent[1], posted);
    printPoint(planeAndCent[0]);
    orth_->getScan()->setPostedGrid(posted);
    thresholdWithLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop());
    QFile f("posted.csv");
    f.open(QFile::WriteOnly);
    QTextStream fs(&f);
    fs<<orth_->getScan()->getPostedXYGrid()->toCSV();
    f.close();
    makeSTLs();

}

void OrthoticController::makeSTLs(){
    QList<FAHLoopInXYPlane*> inners;
    STLMesh* m = makeSTLfromScanSection(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),inners);
    m->scale(2,1,1.0);
    stlToFile(m,"original.stl");
    QList<View_3D_Item> toEmitList;
    View_3D_Item v3d;

    v3d.mesh = m;
    v3d.color = QColor(Qt::gray);

    toEmitList.append(v3d);
    emit stlsGenerated(toEmitList);
}

Border OrthoticController::borderFromLoop(FAHLoopInXYPlane* loop){}
QList<QPointF> OrthoticController::qpointfListFromFAHVector3(QVector<FAHVector3> points){}
