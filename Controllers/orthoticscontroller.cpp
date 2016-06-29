#include "orthoticscontroller.h"
#include "processingfunctions.h"
#include "stlgeneration.h"
//#include "UnitTest/debugfunctions.h"
#include "padgenerator.h"
#include "globals.h"
#include <QDateTime>

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

    //blur pt
//    int ntimes = 1;
//    for(int i=0;i<ntimes;i++){
//        foreach(FAHVector3 pt, orth_->getLoop()->points){
//            float z = medianNoiseFilteringAtPt(orth_->getScan()->getPostedXYGrid(),pt);
//            int a=pt.x/orth_->getScan()->getPostedXYGrid()->stepSizeX();
//            int b=pt.y/orth_->getScan()->getPostedXYGrid()->stepSizeY();
//            orth_->getScan()->getPostedXYGrid()->operator ()(a,b)=z;
//        }
//    }
    int ntimes = 10;
    for(int i=0;i<ntimes;i++){
        blurAlongLine(orth_->getScan()->getPostedXYGrid(),
                      orth_->getHealPoints().first(),
                      orth_->getForePoints().first());
        blurAlongLine(orth_->getScan()->getPostedXYGrid(),
                      orth_->getHealPoints().last(),
                      orth_->getForePoints().last());
    }
    anchorFront(orth_->getScan()->getPostedXYGrid(),orth_->getForePoints());

    //dumpToFile("posted-blured.csv",orth_->getScan()->getPostedXYGrid()->toCSV());
    //return;


    ////////////////////////////Make heal cup
    QVector< FAHVector3 > curve =  secondOrder(orth_->getHealPoints(),orth_->getForePoints(), 100);
    /// GET ARCH MAX
    FAHVector3 pheal,pfront, p_heal_other, p_front_other;
    int plus_y=0, minus_y=0;
    if(Orthotic::kRight==orth_->getFootType()){
        pheal = orth_->getHealPoints().first();
        pfront = orth_->getForePoints().first();
        p_heal_other = orth_->getHealPoints().last();
        p_front_other = orth_->getForePoints().last();
        plus_y = 3;
        minus_y = -4;
    }else{ //left
        pheal = orth_->getHealPoints().last();
        pfront = orth_->getForePoints().last();
        p_heal_other = orth_->getHealPoints().first();
        p_front_other = orth_->getForePoints().first();
        plus_y = 4;
        minus_y = -3;
    }
    FAHVector3 max_arch= maxAlongLine( orth_->getScan()->getPostedXYGrid(),pheal,pfront);

    /////Determine s height
    float setz=0;
    if(orth_->getHealType()==Orthotic::kShallow){
        int n=0;
        for(int i=0;i<curve.size();i++){
            setz+= orth_->getScan()->getPostedXYGrid()->at(curve.at(i).x,curve.at(i).y);
            n++;
        }
        setz = setz/n;

    }else{// Deep cup
        setz = max_arch.z;
    }

    /////////Set Points
    /// set curve points
    for(int i=0;i<curve.size();i++){
        int a=curve.at(i).x/orth_->getScan()->getPostedXYGrid()->stepSizeX()+0.5;
        int b=curve.at(i).y/orth_->getScan()->getPostedXYGrid()->stepSizeY()+0.5;
        //qDebug()<<"("<<a<<","<<b<<")";
        for(int k=-2;k<3;k++){
            for(int L=-2;L<2;L++){
                orth_->getScan()->getPostedXYGrid()->operator ()(a+k,b+L)=setz;
            }
        }
    }
    FAHVector3 v = max_arch-pheal;
    FAHVector3 testp;
    int numpts=1000;
    for(int i=0; i<numpts;i++){
        float t = 1.0/numpts*i;
        testp = pheal+t*v;
        testp.z=orth_->getScan()->getPostedXYGrid()->operator ()(testp.x,testp.y);
        if (testp.z<setz){
            int a=testp.x/orth_->getScan()->getPostedXYGrid()->stepSizeX() +0.5;
            int b=testp.y/orth_->getScan()->getPostedXYGrid()->stepSizeY() +0.5;
            for(int k=-1;k<1;k++){
                for(int L=minus_y;L<plus_y;L++){
                    orth_->getScan()->getPostedXYGrid()->operator ()(a+k,b+L)=setz;
                }
            }
        }
    }
    if(orth_->getHealType()==Orthotic::kDeep){
        v.z=0;
        QSettings settings;
        float percent = settings.value("Generating/healpercent",0.75).toFloat();
        float distance = v.magnitude()*percent;
        FAHVector3 delta = p_front_other - p_heal_other;
        FAHVector3 norm = delta.copy();
        norm.normalize();
        FAHVector3 other = distance*norm + p_heal_other;
        delta = other-p_heal_other;

        FAHVector3 test_other;
        float z_other = orth_->getScan()->getPostedXYGrid()->at(other.x,other.y);
        float z=0;
        int numpts2=500;
        for(int q=0; q<numpts2;q++){
            float t = 1.0/numpts2*q;
            test_other = p_heal_other+t*delta;

            int a=test_other.x/orth_->getScan()->getPostedXYGrid()->stepSizeX() +0.5;
            int b=test_other.y/orth_->getScan()->getPostedXYGrid()->stepSizeY() +0.5;
            z = (z_other-setz)/numpts2*q+setz;
    //        qDebug()<<test_other.x<<","<<test_other.y;
    //        orth_->getScan()->getPostedXYGrid()->operator ()(test_other.x,test_other.y) = z;

            for(int k=-2;k<2;k++){
                for(int L=-2;L<2;L++){
                    orth_->getScan()->getPostedXYGrid()->operator ()(a+k,b+L)=z;
                }
            }

        }

        int ntimes = 10;
        for(int i=0;i<ntimes;i++){
            blurAlongLine(orth_->getScan()->getPostedXYGrid(),
                          orth_->getHealPoints().first(),
                          orth_->getForePoints().first());
            blurAlongLine(orth_->getScan()->getPostedXYGrid(),
                          orth_->getHealPoints().last(),
                          orth_->getForePoints().last());
        }

    }



    //dumpToFile("posted-blured.csv",orth_->getScan()->getPostedXYGrid()->toCSV());

}
void OrthoticController::normalizeByBoundary(){

    {//orth_->getScan()->reset();
    //    FAHVector3 minpt1 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints().first(),orth_->getHealPoints().last());
    //    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints().last(),orth_->getForePoints().first());
    //    FAHVector3 minpt3 = minpt2.copy()+0.05*(minpt2.copy()-orth_->getForePoints().last().copy());
    //        minpt3.z=orth_->getScan()->getProcessedXYGrid()->at(minpt3.x, minpt3.y );
    //    FAHVector3 minpt2 = minAlongLine(orth_->getScan()->getProcessedXYGrid(),orth_->getForePoints().last(),orth_->getForePoints().first());
    //    FAHVector3 minpt3 = orth_->getForePoints().last();//minpt2.copy()+0.05*(minpt2.copy()-orth_->getForePoints().last().copy());
    }
    // generates a plane between the heal's minimum and the fore foot
    FAHVector3 minpt1 = findHeal(orth_->getScan()->getProcessedXYGrid(),orth_->getHealPoints(),orth_->getLoop());
    FAHVector3 minpt2=orth_->getForePoints().first();
    FAHVector3 minpt3=orth_->getForePoints().last();



    FAHVector3 planeVec = normFrom3Pts(minpt1,minpt2,minpt3);
    FAHVector3 d= minpt1.copy();


    //offset the plane by the thickness of the heal you want

    QSettings settings;
    float slope = settings.value("Generating/healthickness",10).toFloat();
    d.z = d.z-slope;
    projectGridOntoPlane(planeVec.scale(1.0),d,orth_->getScan()->getProcessedXYGrid());


}

void OrthoticController::setTopCoat(Top_Coat tc){
    orth_->setTopCoat(tc);
}

void OrthoticController::setBottomType(Orthotic::bottom_type type){
    orth_->setBottomType(type);
}

void OrthoticController::setHealType(Orthotic::heal_type t){
    orth_->setHealType(t);
}

void OrthoticController::addManipulation(Manipulation *m){
    orth_->addManipulation(m);
}
void OrthoticController::offset(float mm){
    XYGrid<float>* grid = orth_->getScan()->getPostedXYGrid() ;
    for(int i=0;i<grid->nx();i++){
        for(int j=0; j<grid->ny(); j++){
            grid->operator ()(i,j)=grid->operator ()(i,j)+mm;
        }
    }
}

void OrthoticController::processFromRaw(){
    QSettings settings;
    orth_->getScan()->reset();
    medianNoiseFiltering(orth_->getScan()->getProcessedXYGrid() );
    float slope = settings.value("Generating/slope",1).toFloat();
    float heightoffset =settings.value("Generating/offset",2).toFloat();//0.5;/// CANT BE ZERO OR A BAD STL IS MADE
    scaleAndOffset(orth_->getScan()->getProcessedXYGrid(),slope,heightoffset);
    orth_->getScan()->setPostedGrid(orth_->getScan()->getProcessedXYGrid());
}

void OrthoticController::processPosting(){
    QSettings settings;
    //int bordertimes = settings.value("Generating/bordertimes",75).toInt();//2;

    int blurs = settings.value("Generating/blurtimes",10).toInt();//2;

    //anchorFront(orth_->getScan()->getPostedXYGrid(),orth_->getForePoints());

    qint64 s = QDateTime::currentMSecsSinceEpoch();
    processBoundary();
    qint64 p = QDateTime::currentMSecsSinceEpoch();
    //normalizeBorder(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),bordertimes);
    qint64 n = QDateTime::currentMSecsSinceEpoch();
    dumpToFile("posted-preblur.csv",orth_->getScan()->getPostedXYGrid()->toCSV());
    blurInLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop(),blurs);
    qint64 b = QDateTime::currentMSecsSinceEpoch();
    thresholdWithLoop(orth_->getScan()->getPostedXYGrid(),orth_->getLoop());
    qint64 t = QDateTime::currentMSecsSinceEpoch();
    dumpToFile("posted-final.csv",orth_->getScan()->getPostedXYGrid()->toCSV());
    qDebug()<<"Time to process boundary: "<< (p-s);
    qDebug()<<"Time to normalize boundary: "<< (n-p);
    qDebug()<<"Time to blur in boundary: "<< (b-n);
    qDebug()<<"Time to threshold inside boundary: "<< (t-b);

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
    dumpToFile("posted-preprocess.csv",posted->toCSV());
    orth_->getScan()->setPostedGrid(posted);
    processPosting();
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
    QSettings settings;
    float thickness = settings.value("Generating/thickness",1).toFloat();;

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
    if (make_thickness){shell=makeSTLfromScan(orth_->shellgrid);}
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



