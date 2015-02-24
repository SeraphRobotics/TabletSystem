#include "mergecontroller.h"
#include <QDebug>
#include <QProcess>
#include <QSettings>
#include <QFile>
#include <QDomDocument>


MergeController::MergeController(file_z_pair shell, QList< file_z_pair>  pads, file_z_pair topcoat, QObject *parent):
    QObject(parent), shell_(shell), pad_files_(pads), topcoat_file_(topcoat)
{
}

void MergeController::mergeFiles(){
    qDebug()<<"merging";

    QSettings settings;
    QString dir_ = settings.value("printing/directory").toString();
    /////////////// Make XML file
    //QString xmlfile = "merge.xml";
    QString xmlfile = dir_ + "/" + "merge.xml";
    QDomDocument d("MergeFile");
    QDomElement node = d.createElement("merge");

    //Shell
    QDomElement shellEl = nodeFromPair("shell",shell_);
    node.appendChild(shellEl);

    foreach(file_z_pair p,pad_files_){
        p.gcode_file = dir_+"/"+p.gcode_file;
        QDomElement padEl = nodeFromPair("pad",p);
        node.appendChild(padEl);
    }
    QDomElement topEl = nodeFromPair("topcoat",topcoat_file_);
    node.appendChild(topEl);
    d.appendChild(node);


    QFile file(xmlfile);
    if (!file.open(QIODevice::WriteOnly)){
        emit Failed("Failed to open merge.xml");
        return;
    }
    QTextStream f(&file);
    f<<d.toString();
    file.close();
    qDebug()<<"wrote xml";

    // Merge files using XML
    //QSettings settings;
    QString mergecommand = "python";
    QProcess* merger = new QProcess(this);
    QStringList merge_args;

    merge_args<<settings.value("printing/merge-python-script","merge.py").toString(); //dir_.absolutePath()+"/"+

    merge_args<<xmlfile;

    //QString outputname = settings.value("printing/output-name","merged.gcode").toString(); //dir_.absolutePath()+"/"+
    //QString outputname = settings.value("printing/directory","/merged.gcode").toString(); //dir_.absolutePath()+"/"+
    QString outputname = dir_ + "/" + "merged.gcode"; //).toString(); //dir_.absolutePath()+"/"+
    merge_args<<outputname;


    qDebug()<<"merge args: \n"<<merge_args;
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





QDomElement nodeFromPair(QString name, file_z_pair pair){
    QDomDocument d;
    QDomElement node = d.createElement(name);

    QDomElement fileEl = d.createElement("file");
    fileEl.appendChild(d.createTextNode( pair.gcode_file ));

    QDomElement zoffsetEl = d.createElement("zoffset");
    zoffsetEl.appendChild(d.createTextNode( QString::number(pair.z_offset,'f',5) ));

    QDomElement ztranslateEl = d.createElement("ztranslate");
    ztranslateEl.appendChild(d.createTextNode( QString::number(pair.z_translate,'f',5) ));

    QDomElement xEl = d.createElement("xcenter");
    xEl.appendChild(d.createTextNode( QString::number(pair.x_center,'f',5) ));

    QDomElement yEl = d.createElement("ycenter");
    yEl.appendChild(d.createTextNode( QString::number(pair.y_center,'f',5) ));

    node.appendChild(fileEl);
    node.appendChild(zoffsetEl);
    node.appendChild(ztranslateEl);
    node.appendChild(xEl);

    node.appendChild(yEl);
//    qDebug()<<"Node: "<<name<<" file:"<<pair.file;
    return node;
}
