#include "scannercontroller.h"
#include <QPixmap>
#include <QDebug>
#include <QSettings>

ScannerController::ScannerController(QObject *parent) :
    QObject(parent),camNumber_(1),scandistance_(338),stepsize_(2),framerate_(5),width_(1920),height_(1080),dist(0)
{
    QSettings s;
    dir_ = QDir(s.value("scanner/directory",QDir::currentPath()).toString());
    qDebug()<<"Scan Dir: "<<dir_.absolutePath();
    camNumber_ = s.value("scanner/camNumber",1).toInt();
    sai_=  new ScannerArduinoInterface();
    timer_ = new QTimer();
//    timer_->setInterval(1000.0/framerate_*0.925);
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
    sai_=  new ScannerArduinoInterface(port,QSerialPort::Baud9600);
    connect(sai_,SIGNAL(buttonPressed()),this,SLOT(buttonPress()));
    connect(sai_,SIGNAL(errored()),this,SLOT(scannerError()));
    //connect(sai_,SIGNAL(scanMovementCompleted()),this,SLOT(scanComplete()));
    connect(sai_,SIGNAL(scanIncremented()),this,SLOT(ScanStep()));

    qDebug()<<"selected: "<<port<<" opened:"<<sai_->isReady();

//    QTimer::singleShot(3000,this,SLOT( setupCamera() ));
//    QTimer::singleShot(2000,sai_,SLOT(ledOn()));
    if(sai_->isReady()){QTimer::singleShot(2200,sai_,SLOT(home()));}
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

void ScannerController::startScan(){
    if(sai_->isReady()){buttonPress();}
}

void ScannerController::buttonPress(){
    if (!timer_->isActive() ){
        sai_->startScan();
//        timer_->start();
        dist=0;
        setupCamera();
        QTimer::singleShot(100,this,SLOT(ScanStep()));
    }
}
void ScannerController::scannerError(){
    emit ScanError();
}
void ScannerController::scanComplete(){
    emit ScanComplete();
}

void ScannerController::ScanStep(){
    if(!capwebcam.isOpened()){return;}
    int n=0;
    QImage m;
    cv::Mat dest;

    while (m.isNull() && n<10){
        qDebug()<<"QIMAGE NULL N:"<<n;
        n++;
        capwebcam.read(matOriginal);



        if(matOriginal.empty()==true){
            qDebug()<<QString("Error reading camera at position:")<<QString::number(dist);
        }else{

            cv::cvtColor(matOriginal, dest,CV_BGR2RGB);

            m = QImage((unsigned char*) dest.data,dest.cols,dest.rows,dest.step,QImage::Format_RGB888);
            if (!m.isNull()){
                qDebug()<<"saving to "<<dir_.absoluteFilePath(QString::number(dist)+".jpeg");
                qDebug()<<m.byteCount();
                qDebug()<<m.save(dir_.absoluteFilePath(QString::number(dist)+".jpeg"),"JPEG");
                sai_->scanStep();
                dist += stepsize_;
            }
        }

    }


    if (dist>=scandistance_){
//        timer_->stop();
        capwebcam.release();
        QTimer::singleShot(100.0,sai_,SLOT(endScan()));
        qDebug()<<"ending";
        QTimer::singleShot(110.0,this,SLOT( scanComplete() ));
    }else{
        qDebug()<<"STEP";
//        QTimer::singleShot(1000.0,this,SLOT(ScanStep()));
//1000.0/framerate_*0.925
    }

}
