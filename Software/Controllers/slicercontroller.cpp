#include "slicercontroller.h"
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QFile>



SlicerController::SlicerController(QString stlfile, QString inifile, bool isValved, QObject *parent) :
    QObject(parent),objfile_(stlfile),inifile_(inifile),isvalved_(isValved)
{
}

void SlicerController::slice(){
    QSettings settings;
    QString slicer = settings.value("printing/slicer","slic3r").toString();
    //for each pair, generate a gcode file using the command  slicer+" \""+p.stlfilename+"\""+"--load \""+p.inifilename+"\""+" --output \""+gcodename+"\"";

    QString output = QString(objfile_).remove(".stl").remove(".obj")+".gcode";

    QStringList args;
    args << objfile_ << "--load" <<inifile_ << "--output" << output;
    QProcess* slicing = new QProcess(this);
    qDebug()<< slicer;
    qDebug()<< args;
    qDebug()<<"Starting slicer";
    slicing->start(slicer,args);

    if (!slicing->waitForStarted(-1)){
        qDebug()<<"failed to start "<<slicer<<" for "<<objfile_;
        emit Failed("failed to start " + slicer);
        return;
    }
    if (!slicing->waitForFinished(-1)){
        qDebug()<<"failed to finish"<<slicer<<" for "<<objfile_;
        emit Failed("failed to finish " + slicer);
        return;
    }
    qDebug()<<"Done slicing";
    qDebug()<<slicing->readAll();
    slicing->close();

    if(isvalved_){ //if a valve tool, convert to valve tool gcode
        QProcess* makevalved = new QProcess(this);
        QString to_valve_gcode = "python";
        QStringList valve_args;
        valve_args << settings.value("printing/valving-python-script","toValve.py").toString();
        valve_args << output;

        qDebug()<<"Starting valving";

        makevalved->start(to_valve_gcode,valve_args);
        if (!makevalved->waitForStarted(-1)){
            qDebug()<<"failed to start "<<to_valve_gcode<<" for "<<output;
            emit Failed("failed to start valving");
            return;
        }
        if (!makevalved->waitForFinished(-1)){
            qDebug()<<"failed to finish"<<to_valve_gcode<<" for "<<output;
            emit Failed("failed to finish valving");
            return;
        }
        qDebug()<<makevalved->readAll();
        qDebug()<<"Done valving";
    }
    emit Success();
}
