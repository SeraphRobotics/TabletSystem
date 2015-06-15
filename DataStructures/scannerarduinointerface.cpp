#include "scannerarduinointerface.h"
#include <QDebug>



ScannerArduinoInterface::ScannerArduinoInterface()
{
    port_ = new QSerialPort();
    connect(port_, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}

ScannerArduinoInterface::ScannerArduinoInterface(QString port, BaudRateType baudrate, QObject *parent) :
    QObject(parent)
{
    port_ = new QSerialPort(port);//,QextSerialPort::Polling
    port_->setBaudRate( baudrate);
    port_->setFlowControl(QSerialPort::NoFlowControl);
    port_->setParity(QSerialPort::NoParity);
    port_->open(QIODevice::ReadWrite);
    //    port_->set
    connect(port_, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
}


bool ScannerArduinoInterface::connectPort(QString port, BaudRateType baudrate){
    if (port_->isOpen()){port_->close();}
    delete port_;
    port_ = new QSerialPort(port);
    port_->setBaudRate(baudrate);
    port_->setFlowControl(QSerialPort::NoFlowControl);
    port_->setParity(QSerialPort::NoParity);
    return port_->isOpen();
}

bool ScannerArduinoInterface::isReady(){
    return port_->isOpen();
}
void ScannerArduinoInterface::startScan(){
    _write("s");
}
void ScannerArduinoInterface::scanStep(){
    _write("i");
}
void ScannerArduinoInterface::endScan(){
    _write("x");
}

void ScannerArduinoInterface::laserOn(){
    _write("l");
}
void ScannerArduinoInterface::laserOff(){
    _write("k");
}
void ScannerArduinoInterface::ledOn(){
    _write("o");
}
void ScannerArduinoInterface::ledOff(){
    _write("f");
}
void ScannerArduinoInterface::home(){
    qDebug()<<"homing";
    _write("h");
}

void ScannerArduinoInterface::disconnect(){
    port_->close();
}


void ScannerArduinoInterface::_write(QString s){

    if (!isReady() ){return;}
//    qDebug()<<"writing: "<<s.toStdString().c_str();

    QChar c = s.at(0);
//    QChar c(100);
    char fill= c.cell();
    QByteArray ba(1,fill);
//    ba.push_front(c.row());
    qint64 l = ba.length();
    qDebug()<<"writing: "<<ba.toHex();
    qint64 received  = port_->write(ba,l);
//    qDebug()<<"wrote:"<<received;
}


void ScannerArduinoInterface::onDataAvailable(){
    QByteArray data = port_->readAll();
//    qDebug()<<"received: "<<QString(data);
    QString c = QString(data);
    if ("B"==c){
        emit buttonPressed();
    }else if ("D"==c){
        emit scanMovementCompleted();
    }else if ("E"==c){
        emit errored();
    }else if("I"==c){
        emit scanIncremented();
    }else{
        qDebug()<<"received: "+c;
    }
}
