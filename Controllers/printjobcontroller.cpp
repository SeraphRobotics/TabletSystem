#include "printjobcontroller.h"
#include <QSettings>
#include <QDebug>
#include "stlgeneration.h"
#include <QProcess>
#include "topcoatcontroller.h"
#include "slicercontroller.h"
#include "repaircontroller.h"
#include "mergecontroller.h"
#include <QThread>
#include "globals.h"


PrintJobController::PrintJobController(Orthotic *orth, QObject *parent) :
    QObject(parent),numSTLsRepaired(0),numSTLsToRepair(0), numSTLsSliced(0), numSTLToSlice(0), topcoatdone(true)/// DISABLE TOPCOAT
{
    orth_ = orth;
    QSettings s;
    dir_ = s.value("printing/directory",QDir::currentPath()).toString();
    workthread = new QThread;
    timer_ = new QTimer();
    timer_->setInterval(10);
    connect(timer_,SIGNAL(timeout()),workthread,SLOT(start()));
    timer_->start();

//    workthread->start();
}

PrintJobController::~PrintJobController(){
    timer_->stop();
    delete timer_;
    delete workthread;

}

void PrintJobController::RunPrintJob(){


    QString shellfilename = dir_ + "/shell.stl";



    numSTLsToRepair = 1+orth_->printjob.manipulationpairs.size();

    FAHVector3 p1, p2, d;
    float rotation=0;
    if(orth_->getFootType()==Orthotic::kLeft){
        p1 = orth_->getForePoints().first().copy();
        p2 = orth_->getHealPoints().first().copy();
        //// EXTRA_SCALE p1.x=p1.x*2;
        p1.y = p1.y+1.1;
        p1.z=0;
       //// EXTRA_SCALE  p2.x=p2.x*2;
        p2.z=0;
        rotation = Math::kPi/2.0;
    }else{
        p1 = orth_->getForePoints().last().copy();
        p2 = orth_->getHealPoints().last().copy();
        //// EXTRA_SCALE p1.x=p1.x*2;
        p1.y = p1.y-1.1;
        p1.z=0;
        //// EXTRA_SCALE p2.x=p2.x*2;
        p2.z=0;
        rotation = -Math::kPi/2.0;
    }

    d= p2-p1;
    d.normalize();
    FAHMatrix4x4 m;
    FAHMatrix4x4 n;
    FAHMatrix4x4 p;
    n.rotationZ(Math::kPi/2.0);
#ifdef DEBUGGING
    m.identity();
    m.mul(p.xyswap());
#else
    m.rotationPointAxisAngle(p2,d,rotation); //(-Math::kPi)
    m = n.mul(m);
    m = p.xyswap().mul(m);
#endif


    orth_->printjob.shellpair.mesh->transform(m);
    RepairController* rs = new RepairController(orth_->printjob.shellpair.mesh,shellfilename);
    connect(rs,SIGNAL(Success()),this,SLOT(repairSucessful()));
    connect(workthread,SIGNAL(finished()),rs,SLOT(deleteLater()));
    connect(rs,SIGNAL(Failed(QString)),this,SLOT(stepFailed(QString)));
    rs->moveToThread(workthread);
    workthread->start();
    rs->repairMesh();
    shell_.stlfile = shellfilename ;
    shell_.gcode_file = shellfilename.replace(".stl","_fixed.gcode");
    shell_.z_offset = 0;
    shell_.z_translate = 0;
    shell_.x_center = orth_->printjob.shellpair.x_center;
    shell_.y_center = orth_->printjob.shellpair.y_center;

    for(int i=0; i<orth_->printjob.manipulationpairs.size();i++){
        QString fn = dir_ + "/"+ orth_->printjob.manipulationpairs.at(i).id;
        fn.append(".stl");
        orth_->printjob.manipulationpairs[i].mesh->transform(m);
        RepairController* r = new RepairController(orth_->printjob.manipulationpairs.at(i).mesh,fn);
        connect(r,SIGNAL(Success()),this,SLOT(repairSucessful()));
        connect(workthread,SIGNAL(finished()),r,SLOT(deleteLater()));
        connect(r,SIGNAL(Failed(QString)),this,SLOT(stepFailed(QString)));
        r->moveToThread(workthread);
        r->repairMesh();

    }


    ///Start topcoat
    //TopCoatController* tcc = new TopCoatController(orth_,"");
//    TopCoatController* tcc = new TopCoatController(orth_, dir_, m);
//    tcc->moveToThread(workthread);
//    connect(tcc,SIGNAL(generatedCoatingFile(QString)),this,SLOT(topcoatMade(QString)));
//    connect(tcc,SIGNAL(Failed(QString)),this,SLOT(stepFailed(QString)));
//    tcc->generateTopCoat();




}

void PrintJobController::makeIniFiles(QString stlfilename, manipulationpair pair){
    //These are not the way to do it We will need to make composites.
    // this will mean making multiple inifiles for some materials and stitching them together
    file_z_pair p;
    p.stlfile = stlfilename;

    p.z_offset = 10.0;
    p.z_translate = pair.z_height;
    p.x_center = pair.x_center;
    p.y_center = pair.y_center;

    if(pair.stiffness>76){
        p.inifile = "p.ini";
        p.gcode_file = stlfilename.replace(".obj",".gcode");
    }else if(pair.stiffness>50){
        p.inifile = "pattern29.ini";
        p.gcode_file = stlfilename.replace(".obj",".extrude.gcode");
        p.z_offset = 3.0;
    }else if(pair.stiffness>25){
        p.inifile = "pattern39.ini";
        p.gcode_file = stlfilename.replace(".obj",".extrude.gcode");
        p.z_offset = 3.0;
    }else if (pair.stiffness>12){
        p.inifile="pattern60.ini";
        p.gcode_file = stlfilename.replace(".obj",".extrude.gcode");
        p.z_offset = 4.0;
    }else{
        p.gcode_file = stlfilename.replace(".obj",".extrude.gcode");
        p.inifile="p.ini";

    }
    QSettings s;
    QString dir = s.value("printing/inis",QDir::currentPath()).toString();

    p.inifile=dir+"/"+p.inifile;
    //p.stlfile=dir+"/"+p.stlfile;
    qDebug()<<"added "<<p.stlfile;
    pad_files_.append(p);
}


void PrintJobController::stepFailed(QString s){
    qDebug()<<s;
    emit PrintJobFailed(s);

}

void PrintJobController::repairSucessful(){
    numSTLsRepaired++;
    if(numSTLsRepaired >=numSTLsToRepair){
        //Start Slicing
        QSettings s;
        QString plasticIni = s.value("printing/plastic_ini","p.ini").toString();
        SlicerController* sc = new SlicerController("shell_fixed.obj",plasticIni,shell_.x_center,shell_.y_center,false);
        numSTLToSlice++;
        sc->moveToThread(workthread);
        connect(sc,SIGNAL(Success()),this,SLOT(slicingSucessful()));
        connect(sc,SIGNAL(Failed(QString)),this,SLOT(slicingFailure(QString)));
        connect(workthread,SIGNAL(finished()),sc,SLOT(deleteLater()));
        workthread->start();
        //sc->slice();


//        int i=0;
//        foreach(manipulationpair mp, orth_->printjob.manipulationpairs ){
        qDebug()<<"\n\nManipulations: "<<orth_->printjob.manipulationpairs.size();
        for(int i=0; i<orth_->printjob.manipulationpairs.size();i++){
            manipulationpair mp = orth_->printjob.manipulationpairs.at(i);
            QString stlfilename = orth_->printjob.manipulationpairs.at(i).id;
            stlfilename.append("_fixed.obj");
//            qDebug()<<"\n\n\n\n"<<i;
            qDebug()<<"filename: "<<stlfilename;
//            qDebug()<<"stiffness: "<<mp.stiffness;
            makeIniFiles(stlfilename,mp);

            numSTLToSlice+=pad_files_.size();
            foreach(file_z_pair p,pad_files_){
                SlicerController* sci = new SlicerController(p.stlfile,p.inifile,p.x_center,p.y_center,true);
                connect(sci,SIGNAL(Success()),this,SLOT(slicingSucessful()));
                connect(workthread,SIGNAL(finished()),sci,SLOT(deleteLater()));
                sci->slice();
            }
        }

    }
}
void PrintJobController::slicingSucessful(){
    qDebug() << DEBUG_FUNCTION_NAME << "entered";
    numSTLsSliced++;
    if(numSTLsSliced>=numSTLToSlice && topcoatdone){
        startMerge();
    }
}

void PrintJobController::slicingFailure(QString error)
{
    qDebug() << DEBUG_FUNCTION_NAME << error;
}

void PrintJobController::topcoatMade(QString file){
    qDebug()<<"Top coat at" << file;
    topcoatdone=true;
    topcoat_file_.stlfile = file;
    topcoat_file_.gcode_file = file;
    topcoat_file_.z_offset=0;
    topcoat_file_.z_translate=0;
    if(numSTLsSliced>=numSTLToSlice && topcoatdone){
        startMerge();
    }
}

void PrintJobController::startMerge(){
    qDebug()<<numSTLsSliced<<":"<<numSTLToSlice<<" "<<topcoatdone;

    MergeController* mc = new MergeController(shell_,pad_files_,topcoat_file_);
    connect(mc,SIGNAL(GCodeMerged(QString)),this,SLOT(mergeSucessful(QString)));
    connect(workthread,SIGNAL(finished()),mc,SLOT(deleteLater()));
    mc->moveToThread(workthread);
//    workthread->start();
    mc->mergeFiles();


}

void PrintJobController::mergeSucessful(QString gcode){
    emit GcodeGenerated(gcode);
}


