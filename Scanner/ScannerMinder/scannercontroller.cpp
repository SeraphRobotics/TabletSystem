#include "scannercontroller.h"
#include <QPixmap>
#include <QDebug>
#include <QSettings>

ScannerController::ScannerController(QObject *parent) :
    QObject(parent),camNumber_(1),scandistance_(334),stepsize_(1),framerate_(5),width_(1920),height_(1080),dist(0)
{
    QSettings s;
    camNumber_ = s.value("camNumber",1).toInt();
    sai_=  new ScannerArduinoInterface();
    timer_ = new QTimer();
    timer_->setInterval(1000.0/framerate_*0.925);
    connect(timer_,SIGNAL(timeout()),this,SLOT(ScanStep()));




}

void ScannerController::disconnected(){
    capwebcam.release();
    sai_->disconnect();
    delete sai_;
    sai_=  new ScannerArduinoInterface();
    timer_->stop();
}

void ScannerController::portSelected(QString port){
//    sai_->disconnect();
    delete sai_;
    sai_=  new ScannerArduinoInterface(port,BAUD9600);
    connect(sai_,SIGNAL(buttonPressed()),this,SLOT(buttonPress()));
    connect(sai_,SIGNAL(errored()),this,SLOT(scannerError()));
    connect(sai_,SIGNAL(scanMovementCompleted()),this,SLOT(scanComplete()));

    qDebug()<<"selected: "<<port<<" opened:"<<sai_->isReady();
//    QTimer::singleShot(3000,this,SLOT( setupCamera() ));
//    QTimer::singleShot(2000,sai_,SLOT(ledOn()));
}

void ScannerController::setupCamera(){
    capwebcam.open(camNumber_);
    bool state=true;
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_WIDTH,width_);
    state = state && capwebcam.set(CV_CAP_PROP_FRAME_HEIGHT,height_);

    if(!state){
        qDebug()<< QString("Error setting capture sizes)");
    }

    if ( capwebcam.isOpened() ){
        qDebug()<<"opened camera";
    }else{
        qDebug()<<"is opened";
    }
}

void ScannerController::buttonPress(){
    if (!timer_->isActive() ){
        sai_->startScan();
//        timer_->start();
        dist=0;
        ScanStep();
        setupCamera();
    }
}
void ScannerController::scannerError(){}
void ScannerController::scanComplete(){

}

void ScannerController::ScanStep(){
    if(!capwebcam.isOpened()){return;}
    int n=0;
    QImage m;
    while (m.isNull() && n<10){
        n++;
        capwebcam.read(matOriginal);

        dist += stepsize_;

        if(matOriginal.empty()==true){
            qDebug()<<QString("Error reading camera at position:")<<QString::number(dist);
        }

        cv::Mat dest;
        cv::cvtColor(matOriginal, dest,CV_BGR2RGB);

        m = QImage((unsigned char*) dest.data,dest.cols,dest.rows,dest.step,QImage::Format_RGB888);


    }
    if (!m.isNull()){
        sai_->scanStep();
        m.save(QString::number(dist)+".jpeg","JPEG");
    }


    if (dist>scandistance_){
//        timer_->stop();
        capwebcam.release();
    }else{
        QTimer::singleShot(100,this,SLOT(ScanStep());
    }

}
