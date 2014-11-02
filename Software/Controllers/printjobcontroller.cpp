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


PrintJobController::PrintJobController(Orthotic *orth, QObject *parent) :
    QObject(parent),numSTLsRepaired(0),numSTLsToRepair(0), numSTLsSliced(0), numSTLToSlice(0), topcoatdone(false)
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


    QString shellfilename = "shell.stl";



    numSTLsToRepair = 1+orth_->printjob.manipulationpairs.size();

    RepairController* rs = new RepairController(orth_->printjob.shell,shellfilename);
    connect(rs,SIGNAL(Success()),this,SLOT(repairSucessful()));
    connect(workthread,SIGNAL(finished()),rs,SLOT(deleteLater()));
    connect(rs,SIGNAL(Failed(QString)),this,SLOT(stepFailed(QString)));
    rs->moveToThread(workthread);
    workthread->start();
    rs->repairMesh();

    shell_.file = shellfilename.replace(".stl","_fixed.gcode") ;
    shell_.z = 0;

    for(int i=0; i<orth_->printjob.manipulationpairs.size();i++){
        RepairController* r = new RepairController(orth_->printjob.manipulationpairs.at(i).mesh,QString::number(i)+".stl");
        connect(r,SIGNAL(Success()),this,SLOT(repairSucessful()));
        connect(workthread,SIGNAL(finished()),r,SLOT(deleteLater()));
        connect(r,SIGNAL(Failed(QString)),this,SLOT(stepFailed(QString)));
        r->moveToThread(workthread);
        r->repairMesh();

    }


    ///Start topcoat
    TopCoatController* tcc = new TopCoatController(orth_,"");
    tcc->moveToThread(workthread);
    connect(tcc,SIGNAL(generatedCoatingFile(QString)),this,SLOT(topcoatMade(QString)));
    connect(tcc,SIGNAL(Failed(QString)),this,SLOT(stepFailed(QString)));
    tcc->generateTopCoat();




}

QStringList PrintJobController::makeIniFiles(QString stlfilename, float stiffness){
    //These are not the way to do it We will need to make composites.
    // this will mean making multiple inifiles for some materials and stitching them together

    QStringList inifiles;
    if(stiffness>25){
        inifiles<<"hs.ini";
        file_z_pair p;
        p.file = stlfilename;
        p.z = 10.0;
        pad_files_.append(p);
    }else if (stiffness>12){
        inifiles<<"ms.ini";
        file_z_pair p;
        p.file = stlfilename;
        p.z = 10.0;
        pad_files_.append(p);
    }else{
        inifiles<<"ls.ini";
        file_z_pair p;
        p.file = stlfilename;
        p.z = 10.0;
        pad_files_.append(p);
    }

    return inifiles;
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

        SlicerController* sc = new SlicerController("shell_fixed.obj",plasticIni,false);
        numSTLToSlice++;
        sc->moveToThread(workthread);
        connect(sc,SIGNAL(Success()),this,SLOT(slicingSucessful()));
        connect(workthread,SIGNAL(finished()),sc,SLOT(deleteLater()));
        workthread->start();
        sc->slice();


        int i=0;
        foreach(manipulationpair mp, orth_->printjob.manipulationpairs ){
            QString stlfilename = QString::number(i)+QString('_fixed.obj');

            QStringList inifilenames = makeIniFiles(stlfilename,mp.stiffness);
            numSTLToSlice+=inifilenames.size();
            foreach(QString ini,inifilenames){
                SlicerController* sci = new SlicerController(stlfilename,ini,true);
                connect(sci,SIGNAL(Success()),this,SLOT(slicingSucessful()));
                connect(workthread,SIGNAL(finished()),sci,SLOT(deleteLater()));
                sci->slice();
            }
            i++;
        }

    }
}
void PrintJobController::slicingSucessful(){
    numSTLsSliced++;
    if(numSTLsSliced>=numSTLToSlice && topcoatdone){
        startMerge();
    }
}

void PrintJobController::topcoatMade(QString file){
    qDebug()<<"Top coat at" << file;
    topcoatdone=true;
    topcoat_file_.file = file;
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


