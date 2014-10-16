#include "gcodecontroller.h"
#include <QProcess>
#include <QDebug>
#include <QSettings>

GcodeController::GcodeController(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    QString appdir= settings.value("printing/dir",".").toString();
    dir_ = QDir(appdir);
}

void GcodeController::addSTLMeshINIPair(STLMesh* mesh, QString inifile, bool isValved){
    QString name = dir_.absolutePath()+dir_.separator()+QString::number(pairs.size())+".stl";
    STLFile stlfile;
    stlfile.SetMesh(mesh);
    stlfile.WriteASCII(name);
    meshpair pair;
    pair.stlfilename=name;
    pair.inifilename = inifile;
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
        QString gcodename = p.stlfilename.remove(".stl")+".gcode";
        gcodes.append(gcodename);
        QStringList args;
        args << p.stlfilename << "--load " << p.inifilename << "--output " << gcodename;
        QProcess* slicing = new QProcess(this);
        slicing->start(slicer,args);
        if (!slicing->waitForStarted()){
            qDebug()<<"failed to start "<<slicer<<" for "<<p.stlfilename;
            emit processingFailed();
            return;
        }
        if (!slicing->waitForFinished()){
            qDebug()<<"failed to finish"<<slicer<<" for "<<p.stlfilename;
            emit processingFailed();
            return;
        }
        qDebug()<<slicing->readAll();


        if(p.isValved){
            QProcess* makevalved = new QProcess(this);
            QString to_valve_gcode = settings.value("printing/valving-python-script","toValve.py");
            QStringList valve_args;
            valve_args << gcodename;
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
        }
    }

    // Merge files
    QString mergecommand = settings.value("printing/merge-python-script","merge.py");;
    QProcess* merger = new QProcess(this);
    merger->start(mergecommand,gcodes);
    if (!merger->waitForStarted()){
        qDebug()<<"failed to start "<<mergecommand<<" with "<<gcodes;
        emit processingFailed();
        return;
    }
    if (!merger->waitForFinished()){
        qDebug()<<"failed to finish "<<mergecommand<<" with "<<gcodes;
        emit processingFailed();
        return;
    }
    qDebug()<<merger->readAll();

    QString outputname = settings.value("printing/output-name","merged.gcode");
    QFile gcodefile(dir_.absolutePath()+dir_.separator()+outputname);
    if(!gcodefile.open(QIODevice::ReadOnly)){
        emit processingFailed();
        qDebug()<<"could not read gcode from "<<gcodefile.fileName();
        return;
    }
    QString gcode = QString(gcodefile.readAll());
    emit processingComplete();
    emit gcodeGenerated(gcode);

}

void GcodeController::setSlicer(QString slicer){
    QSettings settings;
    settings.setvalue("printing/slicer",slicer);
}

void GcodeController::setDir(QString dir){
    QSettings settings;
    settings.setvalue("printing/dir",dir);
    dir_ = QDir(dir);
}
