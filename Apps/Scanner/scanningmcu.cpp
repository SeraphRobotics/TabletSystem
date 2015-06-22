#include "scanningmcu.h"

ScanningMCU::ScanningMCU(QObject *parent) : QObject(parent)
{
    QSettings s;
    s.setValue("scanner/directory",QDir::currentPath()+"/ScanRaw");

    int cn = s.value("scanner/camNumber",-1).toInt();
    if(cn ==-1){
        s.setValue("scanner/camNumber",1);
    }
    sw_ = new ScannerWatcher(this);
    sc_ = new ScannerController(this);
    sp_ = new ScanDataProcesser(s.value("scanner/directory",QDir::currentPath()).toString());
    sm_ = new ScanManager(this);


    connect(sw_,SIGNAL(scannerPort(QString)),sc_,SLOT(portSelected(QString)));
    connect(sw_,SIGNAL(disconnected()),sc_,SLOT(disconnected()));

    connect(sc_,SIGNAL(ScanComplete()),this,SLOT(scanComplete()));
    connect(this,SIGNAL(processScanFolder(QString)),sp_,SLOT(processScan(QString)));
    connect(sp_,SIGNAL(scanProcessed(Scan*)),sm_,SLOT(addScan(Scan*)));
    //sp_->isFoamBox(false);

}


void ScanningMCU::processNow(){
    scanComplete();
}

void ScanningMCU::scanComplete(){
    QSettings s;
    emit processScanFolder(s.value("scanner/directory",QDir::currentPath()).toString());
}
