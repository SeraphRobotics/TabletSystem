#include "scannerwatcher.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>

ScannerWatcher::ScannerWatcher(QObject *parent) :
    QObject(parent),portName_("")
{
    //QextSerialEnumerator* enumer = new QextSerialEnumerator();

    QTimer* timer_ = new QTimer();
    timer_->setInterval(100);
    connect(timer_,SIGNAL(timeout()),this,SLOT(updatePorts()));

    timer_->start();


}

void ScannerWatcher::updatePorts(){

    //QList<QextPortInfo> newports = enumer->getPorts();
     QList<QSerialPortInfo> newports = QSerialPortInfo::availablePorts();
    if (newports.size()!=ports_.size()){
        // UPDATE THE PORTS LIST
        qDebug() << "Update ports list";

        if ( ports_.size()>1 ){
            //CHECK IF DISCONNECCTED
            qDebug() << "ports_.size()>1";
            bool hasport = false;
            foreach(QSerialPortInfo port,newports){
                hasport = (port.portName()==portName_) || hasport ;
                qDebug()<<"detected port: "<<port.portName();
            }
            if (!hasport){
                portName_="";
                emit disconnected();
                qDebug()<<"disconnected";
            }

        }else{
            //INITIALIZATION:
            qDebug() << "initialization";
            bool hasport = false;
            foreach(QSerialPortInfo port,newports){
                qDebug()<<"PORT:"<<port.description();
                if(port.description().contains("arduino uno",Qt::CaseInsensitive) && !hasport ){
                    qDebug()<<"port is now: "<<port.portName();
                    portName_ = port.portName();
                    hasport = true;
                    emit scannerPort(portName_);
                }else{
                    //qDebug()<<"found: "<<port.description();
                }
            }
        }
        ports_=newports;
    }
}
