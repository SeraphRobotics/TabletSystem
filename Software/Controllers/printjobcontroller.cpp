#include "printjobcontroller.h"
#include <QSettings>
#include <QDebug>
#include "stlgeneration.h"

PrintJobController::PrintJobController(QObject *parent) :
    QObject(parent)
{
    gc_ = new GcodeController();
    QSettings s;
    dir_ = s.value("printing/directory",QDir::currentPath()).toString();
    gc_->setDir(dir_);
    connect(gc_,SIGNAL(processingStarted()),this,SLOT(processingStarted()));
    connect(gc_,SIGNAL(processingFailed()),this,SLOT(processingFailed()));
    connect(gc_,SIGNAL(processingComplete()),this,SLOT(processingComplete();));
    connect(gc_,SIGNAL(gcodeGenerated(QString gcode)),this,SLOT(GcodeGenerated(QString gcode)));

}

PrintJobController::~PrintJobController(){
    delete gc_;
}

void PrintJobController::RunPrintJob(printjobinputs pji){
    QSettings s;
    QString plasticIni = s.value("printing/plastic_ini","p.ini").toString();
    QString shellfilename = "0.stl";
    stlToFile(pji.shell,dir_+"//"+shellfilename);
    gc_->addSTLMeshINIPair(shellfilename,plasticIni,false);


    int i=1;
    foreach(manipulationpair mp, pji.manipulationpairs ){
        QString stlfilename = QString::number(i)+'.stl';
        stlToFile(mp.mesh,dir_+"//"+stlfilename);

        QStringList inifilenames = makeIniFiles(mp.manipulation);
        foreach(QString ini,inifilenames){
            gc_->addSTLMeshINIPair(stlfilename,ini,true);
        }
        i++;
    }
    gc_->generateGcode();

}

QStringList PrintJobController::makeIniFiles(Manipulation m){
    //These are not the way to do it We will need to make composites.
    // this will mean making multiple inifiles for some materials and stitching them together
    QStringList inifiles;
    if(m.stiffness>25){
        inifiles<<"hs.ini";
    }else if (m.stiffness>12){
        inifiles<<"ms.ini";
    }else{
        inifiles<<"ls.ini";
    }
    return inifiles;
}

void PrintJobController::processingStarted(){
    qDebug()<<"Started Gcode process";
}
void PrintJobController::processingFailed(){
    qDebug()<<"Started Gcode process";
    emit PrintJobFailed("Unknown reasons");
}
void PrintJobController::processingComplete(){
    qDebug()<<"Finished Gcode process";
    emit PrintJobComplete();
}

void PrintJobController::gcodeGenerated(QString gcode){
    emit GcodeGenerated(gcode);
}
