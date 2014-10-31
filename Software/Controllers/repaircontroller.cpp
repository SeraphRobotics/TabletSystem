#include "repaircontroller.h"
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include "stlgeneration.h"

RepairController::RepairController(STLMesh *mesh, QString name, QObject *parent) :
    QObject(parent),name_(name)
{
    mesh_=mesh;
}

void RepairController::repairMesh(){
    QSettings s;
    QString shellfilename = name_;
    stlToFile(mesh_,shellfilename);

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
        emit Failed(msg);
        return;
    }
    if (!repair->waitForFinished(-1)){
        QString msg = "failed to start "+slicer+" for "+shellfilename;
        qDebug()<<msg;
        emit Failed(msg);
        return;
    }
    qDebug()<<repair->readAll();
    repair->close();
    emit Success();
}
