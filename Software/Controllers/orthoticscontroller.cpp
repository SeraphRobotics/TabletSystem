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
    FAHLoopInXYPlane* loop = loopFromPoints(healPts,forePts);
    orth_->setBoundary(loop);
//    writeLoopToXDFL(loop,"LOOPINSETBORDER.XDFL");
    emit boundaryLoopUpdated(orth_->getLoop());
//    emit borderGenerated(borderFromLoop(orth_->getLoop()));



}

void OrthoticController::processBoundary(){


    FAHVector3 minpt1 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints().first(),orth_->getHealPoints().last());
    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints().last(),orth_->getForePoints().first());
    FAHVector3 minpt3 = orth_->getForePoints().first();
    minpt3.z=orth_->getScan()->getProcessedXYGrid()->at(int(minpt3.x),int(minpt3.y));
    FAHVector3 planeVec = normFrom3Pts(minpt1,minpt2,minpt3);

    FAHVector3 cent = orth_->getForePoints().first();
    FAHVector3 d(cent.x,cent.y,cent.z);
    projectGridOntoPlane(planeVec.scale(1.0),d,orth_->getScan()->getProcessedXYGrid());
    //blurGrid(orth_->getScan()->getProcessedXYGrid(),6);
//    thresholdWithLoop(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop());
    normalizeBorder(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop(),75);
    blurInLoop(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop(),10);
//    blurGrid(orth_->getScan()->getProcessedXYGrid(),10);


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

    QVector<FAHVector3> heals = transformPointsWithPosting(orth_->getHealPoints().first(),
                                                           orth_->getHealPoints().last(),
                                                           orth_->getRearFootPosting());
    QVector<FAHVector3> fors = transformPointsWithPosting(orth_->getForePoints().first(),
                                                          orth_->getForePoints().last(),
                                                          orth_->getForFootPosting());

    QVector<FAHVector3> planeAndCent = makePostingPlane(heals[0],heals[1],fors[0],fors[1]);



    projectGridOntoPlane(planeAndCent[0],planeAndCent[1], posted);
//    printPoint(planeAndCent[0]j);
    orth_->getScan()->setPostedGrid(posted);
    thresholdWithLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop());
    thresholdWithLoop(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop());
    float slope = 63.5/101;
    float heightoffset =0;
    scaleAndOffset(orth_->getScan()->getPostedXYGrid(),slope,heightoffset);

    QFile f("posted.csv");
    f.open(QFile::WriteOnly);
    QTextStream fs(&f);
    fs<<orth_->getScan()->getPostedXYGrid()->toCSV();
    f.close();
    makeSTLs();
    qDebug()<<"Done";

}

void OrthoticController::makeSTLs(){
    QList<FAHLoopInXYPlane*> inners;
//    STLMesh* mp = makeSTLfromScan(orth_->getScan()->getPostedXYGrid());

    QList<FAHLoopInXYPlane*> innerLoops3;
//    innerLoops3.append(orth_->getLoop());
    FAHLoopInXYPlane* outside = new FAHLoopInXYPlane();
    outside->points.append(FAHVector3(0,0,0));
    outside->points.append(FAHVector3(1000,0,0));
    outside->points.append(FAHVector3(1000,1000,0));
    outside->points.append(FAHVector3(0,1000,0));


//    STLMesh* p = makeSTLfromScanSection(orth_->getScan()->getProcessedXYGrid(),
//                                         outside,
//                                         innerLoops3);
//    p->scale(2,1,1.0);
//    stlToFile(p,"prepost.stl");


//    STLMesh* mp = makeSTLfromScanSection(orth_->getScan()->getPostedXYGrid(),
//                                         outside,
//                                         innerLoops3);

//    mp->scale(2,1,1.0);
//    stlToFile(mp,"full.stl");


    FAHLoopInXYPlane* bottomloop = bottomLoopFromPoints(orth_->getHealPoints(),orth_->getForePoints());
    STLMesh* angleMesh = STLFromSection(orth_->getScan()->getPostedXYGrid(),bottomloop,orth_->getLoop(),innerLoops3);
    angleMesh->scale(2,1,1);
    stlToFile(angleMesh,"angled.stl");


//    innerLoops3.append(orth_->getLoop());
//    STLMesh* mn = makeSTLfromScanSection(orth_->getScan()->getPostedXYGrid(),
//                                         outside,
//                                         innerLoops3);
//    mn->scale(2,1,1.0);
//    stlToFile(mn,"cutout.stl");





//    STLMesh* m = makeSTLfromScanSection(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),inners);
//    m->scale(2,1,1.0);
//    stlToFile(m,"original.stl");
    QList<View_3D_Item> toEmitList;
    View_3D_Item v3d;

    v3d.mesh = angleMesh;
    v3d.color = QColor(Qt::gray);

    toEmitList.append(v3d);
    emit stlsGenerated(toEmitList);
//    writeLoopToXDFL(orth_->getLoop(),"FINALLOOP.XDFL");






}

Border OrthoticController::borderFromLoop(FAHLoopInXYPlane* loop){}
QList<QPointF> OrthoticController::qpointfListFromFAHVector3(QVector<FAHVector3> points){}
