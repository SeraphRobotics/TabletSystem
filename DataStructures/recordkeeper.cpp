#include "recordkeeper.h"
#include <QDateTime>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include <QDir>

recordkeeper::recordkeeper(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    int updateTime = settings.value("time/update").toInt();
    if (updateTime == 0){
        updateTime = 10000;
        settings.setValue("time/update",updateTime);
    }
    timer_ = new QTimer();
    timer_->setInterval(updateTime);
    connect(timer_,SIGNAL(timeout()),this,SLOT(updateTime()));
    timer_->start();
    xmlrecord_ = QDomDocument("times");

    if(!settings.contains("time/file")){
            settings.setValue("time/file","records.xml");
    }

    appendTime(kOff,settings.value("time/currentTime",QDateTime::currentMSecsSinceEpoch()).toLongLong());
    appendTime(kOn, QDateTime::currentMSecsSinceEpoch());
    qDebug()<<"Time "<<QDateTime::currentMSecsSinceEpoch();
}

void recordkeeper::appendTime(kTimeType type, qint64 timeInMSSinceEpoch){

    qDebug()<<"update:"<<type<<" at "<<timeInMSSinceEpoch;

    //////////// Open the File////////////////
    // If the file exists load it into XML records
    QSettings settings;
    QFile file_r(settings.value("time/file","records.xml").toString());
    bool loaded = true;
    if (file_r.open(QIODevice::ReadOnly)){
        if (!xmlrecord_.setContent(&file_r)) {
            file_r.close();
            loaded = false;
        }
    }else{
            loaded = false;
            file_r.close();
    }
    // if it doesnt exist, make a new record
    if(!loaded){
        QDomElement root = xmlrecord_.createElement("times");
        xmlrecord_.appendChild(root);
    }



    ///////////////////////Create entry//////////////////////////
    QDomElement time = xmlrecord_.createElement("time");
    if(kOn==type){
        time.setAttribute("type","on");
    }else if (kOff==type){
        time.setAttribute("type","off");
    }else if (kScan==type){
        time.setAttribute("type","scan");
    }
    QDomText txt= xmlrecord_.createTextNode(QString::number(timeInMSSinceEpoch));
    time.appendChild(txt);
    xmlrecord_.documentElement().appendChild(time);



    ///Write updated time
    QFile file(settings.value("time/file","records.xml").toString());
    if(!file.open(QIODevice::WriteOnly)){
        return;
    }
    QTextStream ss(&file);
    ss<<xmlrecord_.toString();
    file.close();

}

void recordkeeper::updateTime(){
    QSettings settings;
    qDebug()<<"time updated";
    settings.setValue("time/currentTime",QDateTime::currentMSecsSinceEpoch());
}

void recordkeeper::USBAdded(QString addr){
    QSettings settings;

    QString path = settings.value("time/file","records.xml").toString();

    QFile* command = new QFile(path);

    if(command->open(QIODevice::ReadWrite)){

        QString str = command->fileName();
        QStringList parts = str.split("/");
        QString simpleName = parts.at(parts.size()-1);

        QString newPath = addr+ QDir::separator() + simpleName;

        qDebug() << newPath;
        bool write_bool = true;

        if(QFile::exists(newPath)){
            write_bool = false;
            //there are records on the USB already
            xmlrecord_.setContent(command);
            QDomDocument usbRecord;
            QFile* usbRecordFile = new QFile(newPath);
            if(!usbRecordFile->open(QIODevice::ReadOnly)){
                write_bool = true;
            }else{
                usbRecord.setContent(usbRecordFile);
                usbRecordFile->close();
                usbRecordFile->open(QIODevice::WriteOnly);

                QDomElement root = usbRecord.documentElement();
                QDomElement element = xmlrecord_.documentElement();
                int i=0;
                for(QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling())
                {
                    i++;
                    qDebug()<<"Node: "<<i;
                    root.appendChild(n.cloneNode(true));
                }

                QTextStream ss(usbRecordFile);
                ss<<usbRecord.toString();
                usbRecordFile->close();
                command->remove();
            }

        }

        if(true==write_bool){
            if(!command->copy(newPath)){
                qDebug()<<"Error copying the code.";
            }else{
                command->remove();
            }
        }

    }
    else{
        qDebug()<<"Error opening the file.";
    }


}


/**
<times>
    <time type="off">123124003</time>
    <time type="on">1231241313</time>
    <time type="scan">123124133</time>
</times>
**/
