#include "slicercontroller.h"
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QFile>


SlicerController::SlicerController(QString stlfile, QString inifile, float x, float y, bool isValved, QObject *parent) :
    QObject(parent),objfile_(stlfile),inifile_(inifile),isvalved_(isValved),x_(x),y_(y)
{
}

void SlicerController::slice(){
    QSettings settings;
    QString slicer = settings.value("printing/slicer","slic3r").toString();
    //for each pair, generate a gcode file using the command  slicer+" \""+p.stlfilename+"\""+"--load \""+p.inifilename+"\""+" --output \""+gcodename+"\"";

    QString dir_ = settings.value("printing/directory").toString();
    QString output = QString(objfile_).remove(".stl").remove(".obj")+".gcode";

    QStringList args;
//    QString cent = QString::number(x_/2.0)+","+QString::number(y_/2.0);
    QString cent = " 100,100";

    args << dir_ + "/" + objfile_ << "--load" <<inifile_ << "--output" << dir_ + "/" + output
         <<"--print-center"<<cent;
    QProcess* slicing = new QProcess(this);
    connect(slicing, SIGNAL(error(QProcess::ProcessError)), this, SLOT(slicerError(QProcess::ProcessError)));
    qDebug()<< "slicer binary: " << slicer;
    qDebug()<< "slicer args: " << args.join(" ");
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
    if(slicing->exitCode() != 0)
        emit Failed("bad exitCode " + slicer);

    qDebug() << "slicer exit code : " << slicing->exitCode();
    //    qDebug()<<slicing->readAll();

//    QFile f(objfile_.append(".txt"));
//    if(f.open(QIODevice::WriteOnly)){
//        QTextStream s(&f);
//        s<<QString(slicing->readAll());
//        f.close();
//    }

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

void SlicerController::slicerError(QProcess::ProcessError error)
{
    switch(error) {
    case QProcess::FailedToStart:
        qDebug() << "slicer failed to start";
        break;
    case QProcess::Crashed:
        qDebug() << "slicer crashed";
        break;
    case QProcess::Timedout:
        qDebug() << "slicer timed out";
        break;
    case QProcess::WriteError:
        qDebug() << "slicer write error";
        break;
    case QProcess::ReadError:
        qDebug() << "slicer read error";
        break;
    case QProcess::UnknownError:
        qDebug() << "slicer unknow error";
        break;
    }
}
