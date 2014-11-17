#include "orthoticscontroller.h"
#include "processingfunctions.h"
#include "stlgeneration.h"
#include "UnitTest/debugfunctions.h"
#include "padgenerator.h"

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

    emit scanImageGenerated(makeHeightMap(orth_->getScan()->getProcessedXYGrid()));

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


Orthotic* OrthoticController::getOrthotic(){
    return orth_;
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

    orth_->getScan()->reset();
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
//    anchorFront(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints());
//    normalizeBorder(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop(),75);
//    blurInLoop(orth_->getScan()->getProcessedXYGrid(),orth_->getLoop(),7);
//    blurGrid(orth_->getScan()->getProcessedXYGrid(),1);


//    QFile f("processed.csv");
//    f.open(QFile::WriteOnly);
//    QTextStream fs(&f);
//    fs<<orth_->getScan()->getProcessedXYGrid()->toCSV();
//    f.close();
}

void OrthoticController::setTopCoat(Top_Coat tc){
    orth_->setTopCoat(tc);
}

void OrthoticController::addManipulation(Manipulation *m){
    orth_->addManipulation(m);
//    makeSTLs();
}

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

    orth_->getScan()->setPostedGrid(posted);

    anchorFront(orth_->getScan()->getPostedXYGrid(),orth_->getForePoints());
    normalizeBorder(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),75);
//    blurByBorder(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),10);
//    normalizeBorder(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),15);
    blurInLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),10);
//    blurGrid(orth_->getScan()->getPostedXYGrid(),1);
    thresholdWithLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop());

    float slope = 63.5/101;
    float heightoffset =0.5;/// CANT BE ZERO OR A BAD STL IS MADE
    scaleAndOffset(orth_->getScan()->getPostedXYGrid(),slope,heightoffset);

//    QFile f("posted.csv");
//    f.open(QFile::WriteOnly);
//    QTextStream fs(&f);
//    fs<<orth_->getScan()->getPostedXYGrid()->toCSV();
//    f.close();
//    makeSTLs();
    qDebug()<<"STLs made";

}

void OrthoticController::makeSTLs(){



    printjobinputs pji;
    QList<View_3D_Item> toEmitList;
    qDebug()<<"starting to make;";
    delete orth_->shellgrid;
    delete orth_->topcoatgrid;
    orth_->shellgrid = new XYGrid<float>(orth_->getScan()->getPostedXYGrid());
    orth_->topcoatgrid = new XYGrid<float>(orth_->getScan()->getPostedXYGrid());
    blurGrid(orth_->shellgrid,2);
    STLMesh* shell = new STLMesh();

    /// GENERATE PADS
    int i=0;
    QList<FAHLoopInXYPlane*> inners;
    foreach(Manipulation* m, orth_->getManipulations()){

        STLMesh* m_mesh = new STLMesh();
        float min_z = GeneratePad(m,orth_->topcoatgrid,orth_->shellgrid,m_mesh,shell,min_z);
        inners.append(m->outerloop);
        m_mesh->scale(2,1,1);
        View_3D_Item v3d;
        v3d.mesh =  m_mesh;
        v3d.color = QColor(Qt::black);
        toEmitList.append(v3d);
        manipulationpair pair;
        pair.mesh = m_mesh;
        pair.stiffness = m->stiffness;
        pair.z_height = min_z;
        FAHVector3 cent = m->outerloop->center();
        pair.x_center = 2*cent.x;
        pair.y_center = cent.y;
        pair.id=QString::number(i);
        i++;
        pji.manipulationpairs.append(pair);
        //stlToFile(m_mesh,QString::number(cent.x)+QString(".stl"));
    };


    FAHLoopInXYPlane* bottomloop = bottomLoopFromPoints(orth_->getHealPoints(),orth_->getForePoints());
//    STLMesh* angleMesh =
    STLFromSection(shell,orth_->shellgrid,bottomloop,orth_->getLoop(),inners);
    shell->scale(2,1,1);

    //Generate Printjob outputs
    manipulationpair pair;
    pair.mesh = shell;
    pair.z_height=0;
    pair.stiffness=0;
    FAHVector3 cent = orth_->getLoop()->center();
    pair.x_center = 2*cent.x;
    pair.y_center = cent.y;
    pji.shellpair = pair;
    orth_->printjob=pji;


    View_3D_Item v3d;

    v3d.mesh = shell;
    v3d.color = QColor(Qt::gray);

    toEmitList.append(v3d);
    emit stlsGenerated(toEmitList);
//    writeLoopToXDFL(orth_->getLoop(),"FINALLOOP.XDFL");
    qDebug()<<"made all stls";
}


void OrthoticController::save(){
    orth_->writeToDisk();
}

Border OrthoticController::borderFromLoop(FAHLoopInXYPlane* loop){return Border();}
QList<QPointF> OrthoticController::qpointfListFromFAHVector3(QVector<FAHVector3> points){
    return QList<QPointF>();
}
