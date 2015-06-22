#include "orthoticscontroller.h"
#include "processingfunctions.h"
#include "stlgeneration.h"
//#include "UnitTest/debugfunctions.h"
#include "padgenerator.h"
#include "globals.h"

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
    for(int i=0;i<healPts.size();i++){
        healPts[i].z=orth_->getScan()->getProcessedXYGrid()->at(healPts[i].x,healPts[i].y);
    }
    for(int i=0;i<forePts.size();i++){
        forePts[i].z=orth_->getScan()->getProcessedXYGrid()->at(forePts[i].x,forePts[i].y);
    }
    orth_->setBorderPoints(healPts,forePts);
    FAHLoopInXYPlane* loop = loopFromPoints(healPts,forePts);
    orth_->setBoundary(loop);
//    writeLoopToXDFL(loop,"LOOPINSETBORDER.XDFL");
    emit boundaryLoopUpdated(orth_->getLoop());
//    emit borderGenerated(borderFromLoop(orth_->getLoop()));



}

void OrthoticController::processBoundary(){

    //orth_->getScan()->reset();
    //    FAHVector3 minpt1 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints().first(),orth_->getHealPoints().last());
    //    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints().last(),orth_->getForePoints().first());
    //    FAHVector3 minpt3 = minpt2.copy()+0.05*(minpt2.copy()-orth_->getForePoints().last().copy());
    //        minpt3.z=orth_->getScan()->getProcessedXYGrid()->at(minpt3.x, minpt3.y );

    // generates a plane between the heal's minimum and the fore foot
    FAHVector3 minpt1 = findHeal(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints(),orth_->getLoop());
//    FAHVector3 minpt2=orth_->getForePoints().first();
//    FAHVector3 minpt3=orth_->getForePoints().last();
    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints().last(),orth_->getForePoints().first());
    FAHVector3 minpt3 = orth_->getForePoints().last();//minpt2.copy()+0.05*(minpt2.copy()-orth_->getForePoints().last().copy());


    FAHVector3 planeVec = normFrom3Pts(minpt1,minpt2,minpt3);
    FAHVector3 d= minpt1.copy();


    //ofset the plane by the thickness of the heal you want

    QSettings settings;
    float slope = settings.value("Generating/healthickness",10).toFloat();
    d.z = d.z-slope;
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

void OrthoticController::setBottomType(Orthotic::bottom_type type){
    orth_->setBottomType(type);
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


    QSettings settings;
    int bordertimes = settings.value("Generating/bordertimes",75).toInt();//2;
    float slope = settings.value("Generating/slope",1).toFloat();
    float heightoffset =settings.value("Generating/offset",0.5).toFloat();//0.5;/// CANT BE ZERO OR A BAD STL IS MADE
    int blurs = settings.value("Generating/blurtimes",10).toInt();//2;

    normalizeBorder(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),bordertimes);

    anchorFront(orth_->getScan()->getPostedXYGrid(),orth_->getForePoints());
    blurInLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),blurs);
    thresholdWithLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop());


    scaleAndOffset(orth_->getScan()->getPostedXYGrid(),slope,heightoffset);


//    QFile f("posted.csv");
//    f.open(QFile::WriteOnly);
//    QTextStream fs(&f);
//    fs<<orth_->getScan()->getPostedXYGrid()->toCSV();
//    f.close();
//    makeSTLs();
    qDebug()<<"Posting made";

}

void OrthoticController::makeSTLs(){



    printjobinputs pji;
    QList<View_3D_Item> toEmitList;
    qDebug()<<"starting to make;";
    delete orth_->shellgrid;
    delete orth_->topcoatgrid;
    orth_->shellgrid = new XYGrid<float>(orth_->getScan()->getPostedXYGrid());
    orth_->topcoatgrid = new XYGrid<float>(orth_->getScan()->getPostedXYGrid());
//    blurGrid(orth_->shellgrid,2);
    STLMesh* shell = new STLMesh();

    bool make_thickness = (Orthotic::kCurved == orth_->getBottomType());
    float thickness = 10.0;

    /// GENERATE PADS
    int i=0;
    QList<FAHLoopInXYPlane*> inners;
    foreach(Manipulation* m, orth_->getManipulations()){
        qDebug()<<"Making Pad";
        STLMesh* m_mesh = new STLMesh();
        float min_z = GeneratePad(m,orth_->topcoatgrid,orth_->shellgrid,m_mesh,shell,thickness,make_thickness);
        inners.append(m->outerloop);
        //// EXTRA_SCALE m_mesh->scale(2,1,1);
        View_3D_Item v3d;
        v3d.mesh =  m_mesh;
        v3d.color = QColor(Qt::black);
        toEmitList.append(v3d);
        manipulationpair pair;
        pair.mesh = m_mesh;
        pair.stiffness = m->stiffness;
        pair.z_height = min_z;
        FAHVector3 cent = m->outerloop->min()-orth_->getLoop()->min();
//        printPoint( m->outerloop->min());
//         printPoint( orth_->getLoop()->min());
//        writeLoopToXDFL(m->outerloop,"PAD.XDFL");
        pair.x_center = cent.x;
        pair.y_center = cent.y;
//        qDebug()<<"CENTER: "<<cent.x<<","<<cent.y;
        pair.id=QString::number(i);
        i++;
        pji.manipulationpairs.append(pair);
        //stlToFile(m_mesh,QString::number(cent.x)+QString(".stl"));
    };


    FAHLoopInXYPlane* bottomloop = bottomLoopFromPoints(orth_->getHealPoints(),orth_->getForePoints());

    qDebug()<<"Making Shell";
#ifdef FULLSCAN
//    if (make_thickness){shell=makeSTLfromScan(orth_->shellgrid);}
#else
    if (make_thickness){
        FixedThicknessSTL(shell,orth_->shellgrid,orth_->getLoop(),inners,thickness);
    }
#endif
    else {STLFromSection(shell,orth_->shellgrid,bottomloop,orth_->getLoop(),inners);}
    //// EXTRA_SCALE shell->scale(2,1,1);

    //Generate Printjob outputs
    manipulationpair pair;
    pair.mesh = shell;
    pair.z_height=0;
    pair.stiffness=0;
    FAHVector3 cent = orth_->getLoop()->center();
    pair.x_center = cent.x;
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

Border OrthoticController::borderFromLoop(FAHLoopInXYPlane* loop)
{
    Q_UNUSED(loop);
    return Border();
}

QList<QPointF> OrthoticController::qpointfListFromFAHVector3(QVector<FAHVector3> points){
    Q_UNUSED(points);
    return QList<QPointF>();
}



