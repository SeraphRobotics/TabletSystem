#include "mergecontroller.h"
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QFile>

MergeController::MergeController(QStringList files, QObject *parent) :
    QObject(parent),files_(files)
{
}

void MergeController::mergeFiles(){
    qDebug()<<"merging";
    // Merge files
    QSettings settings;
    QString mergecommand = "python";
    QProcess* merger = new QProcess(this);
    QStringList merge_args;

    merge_args<<settings.value("printing/merge-python-script","merge.py").toString(); //dir_.absolutePath()+"/"+
    QString outputname = settings.value("printing/output-name","merged.gcode").toString(); //dir_.absolutePath()+"/"+

    merge_args<<outputname;
    merge_args<<files_;

    qDebug()<<"\n"<<merge_args;
    merger->start(mergecommand,merge_args);
    if (!merger->waitForStarted(-1)){
        qDebug()<<"failed to start "<<mergecommand<<" with "<<merge_args;
        emit Failed("Failed to start "+mergecommand);
        return;
    }
    if (!merger->waitForFinished(-1)){
        qDebug()<<"failed to finish "<<mergecommand<<" with "<<merge_args;
        emit Failed("Failed to finish"+mergecommand);
        return;
    }

    qDebug()<<merger->readAll();

    qDebug()<<"GCode reading";
    QFile gcodefile(outputname);
    if(!gcodefile.open(QIODevice::ReadOnly)){
        emit Failed("could not read merged gcode");
        qDebug()<<"could not read gcode from "<<gcodefile.fileName();
        return;
    }
    QString gcode = QString(gcodefile.readAll());
    gcodefile.close();
    qDebug()<<"GCode done";
    emit GCodeMerged(gcode);
    emit Success();
}
