#include "gcodecontroller.h"
#include <QProcess>
#include <QDebug>
#include <QSettings>

GcodeController::GcodeController(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    //QString appdir= settings.value("printing/dir",".").toString();
    QString appdir= settings.value("printing/directory",".").toString();
    dir_ = QDir(appdir);
}

void GcodeController::addSTLMeshINIPair(QString stlfile, QString inifile, bool isValved){
    QString name = dir_.absolutePath()+"/"+stlfile; //
//    STLFile stlfile;
//    stlfile.SetMesh(stlfile);
//    stlfile.WriteASCII(name);
    meshpair pair;
    pair.stlfilename=name;
    pair.inifilename = dir_.absolutePath()+"/"+inifile;
    pair.isValved = isValved;
    pairs.append(pair);

}
void GcodeController::generateGcode(){
    emit processingStarted();
    QSettings settings;
    QString slicer = settings.value("printing/slicer","slic3r").toString();
    //for each pair, generate a gcode file using the command  slicer+" \""+p.stlfilename+"\""+"--load \""+p.inifilename+"\""+" --output \""+gcodename+"\"";
    QStringList gcodes;
    foreach(meshpair p,pairs){
        QString output = QString(p.stlfilename).remove(".stl").remove(".obj")+".gcode";
        QString gcodename;
        if(p.isValved){
            gcodes.append(QString(p.stlfilename).remove(".stl").remove(".obj")+".extrude.gcode"); //the toValve sciprt will export a file ending in extrude.gcode
        }else{
            gcodes.append(output);
        }

        QStringList args;
        args << p.stlfilename << "--load" << p.inifilename << "--output" << output;
        QProcess* slicing = new QProcess(this);
        qDebug()<< slicer;
        qDebug()<< args;
        qDebug()<<"Starting slicer";
        slicing->start(slicer,args);

        if (!slicing->waitForStarted(-1)){
            qDebug()<<"failed to start "<<slicer<<" for "<<p.stlfilename;
            emit processingFailed();
            return;
        }
        if (!slicing->waitForFinished(-1)){
            qDebug()<<"failed to finish"<<slicer<<" for "<<p.stlfilename;
            emit processingFailed();
            return;
        }
        qDebug()<<"Done slicing";
        qDebug()<<slicing->readAll();
        slicing->close();

        if(p.isValved){ //if a valve tool, convert to valve tool gcode
            QProcess* makevalved = new QProcess(this);
            QString to_valve_gcode = "python";
            QStringList valve_args;
            valve_args << dir_.absolutePath()+"/"+settings.value("printing/valving-python-script","toValve.py").toString();
            valve_args << gcodename;

            qDebug()<<"Starting valving";

            makevalved->start(to_valve_gcode,valve_args);
            if (!makevalved->waitForStarted()){
                qDebug()<<"failed to start "<<to_valve_gcode<<" for "<<gcodename;
                emit processingFailed();
                return;
            }
            if (!makevalved->waitForFinished()){
                qDebug()<<"failed to finish"<<to_valve_gcode<<" for "<<gcodename;
                emit processingFailed();
                return;
            }
            qDebug()<<makevalved->readAll();
            qDebug()<<"Done valving";
        }
    }

    QString outputname;

    if(pairs.size()>2){
        qDebug()<<"merging";
        // Merge files
        QString mergecommand = "python";
        QProcess* merger = new QProcess(this);
        QStringList merge_args;
        merge_args<<settings.value("printing/merge-python-script","merge.py").toString(); //dir_.absolutePath()+"/"+
        outputname = settings.value("printing/output-name","merged.gcode").toString(); //dir_.absolutePath()+"/"+
        merge_args<<outputname;
        merge_args<<gcodes;
        qDebug()<<"\n"<<merge_args;
        merger->start(mergecommand,merge_args);
        if (!merger->waitForStarted()){
            qDebug()<<"failed to start "<<mergecommand<<" with "<<merge_args;
            emit processingFailed();
            return;
        }
        if (!merger->waitForFinished()){
            qDebug()<<"failed to finish "<<mergecommand<<" with "<<merge_args;
            emit processingFailed();
            return;
        }

        qDebug()<<merger->readAll();
    }else{
        outputname = gcodes.last();
    }

    qDebug()<<"GCode reading";
    QFile gcodefile(outputname);
    if(!gcodefile.open(QIODevice::ReadOnly)){
        emit processingFailed();
        qDebug()<<"could not read gcode from "<<gcodefile.fileName();
        return;
    }
    QString gcode = QString(gcodefile.readAll());
    gcodefile.close();
    qDebug()<<"GCode done";
    emit processingComplete();
    emit gcodeGenerated(gcode);
//    qDebug()<<gcode;

}

void GcodeController::setSlicer(QString slicer){
    QSettings settings;
    settings.setValue("printing/slicer",slicer);
}

void GcodeController::setDir(QString dir){
    QSettings settings;
    settings.setValue("printing/dir",dir);
    dir_ = QDir(dir);
}
