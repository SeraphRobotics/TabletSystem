#include "printjobcontroller.h"
#include <QSettings>
#include <QDebug>
#include "stlgeneration.h"
#include <QProcess>

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

void PrintJobController::RunPrintJob(Orthotic* orth){


    QSettings s;
    QString plasticIni = s.value("printing/plastic_ini","p.ini").toString();
    QString shellfilename = "0.stl";
    stlToFile(orth->printjob.shell,shellfilename);//,dir_+"//"+

    /// REPAIR SHELL
    QString slicer = s.value("printing/slicer","slic3r").toString();

    QStringList args;
    args << "--repair"<<shellfilename;
    QProcess* repair = new QProcess(this);
    qDebug()<< slicer;
    qDebug()<< args;
    repair->start(slicer,args);

    if (!repair->waitForStarted(-1)){
        QString msg = "failed to start "+slicer+" for "+shellfilename;
        qDebug()<<msg;
        emit PrintJobFailed(msg);
        return;
    }
    if (!repair->waitForFinished(-1)){
        QString msg = "failed to start "+slicer+" for "+shellfilename;
        qDebug()<<msg;
        emit PrintJobFailed(msg);
        return;
    }
    qDebug()<<repair->readAll();
    repair->close();




    gc_->addSTLMeshINIPair(shellfilename.replace(".stl","_fixed.obj"),plasticIni,false);


    int i=1;
    foreach(manipulationpair mp, orth->printjob.manipulationpairs ){
        QString stlfilename = QString::number(i)+QString('.stl');
        stlToFile(mp.mesh,stlfilename);//dir_+"//"+

        QStringList inifilenames = makeIniFiles(mp.stiffness);
        foreach(QString ini,inifilenames){
            gc_->addSTLMeshINIPair(stlfilename,ini,true);
        }
        i++;
    }
    gc_->generateGcode();

}

QStringList PrintJobController::makeIniFiles(float stiffness){
    //These are not the way to do it We will need to make composites.
    // this will mean making multiple inifiles for some materials and stitching them together
    QStringList inifiles;
    if(stiffness>25){
        inifiles<<"hs.ini";
    }else if (stiffness>12){
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
