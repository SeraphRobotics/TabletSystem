#include "scandataprocesser.h"
#include <QSettings>
#include "scanprocessing.h"
#include <QThread>
#include <QUuid>
#include <QSettings>
#include <QDebug>

ScanDataProcesser::ScanDataProcesser(QObject *parent) :
    QObject(parent),numFilesToProcess(0),numFilesProcessed(0),is_foambox_(true)
{
    id_=QUuid::createUuid().toString();
    dir_=QDir::current();
    QSettings s;
    img_format_ = s.value("scanner/imageformat",".jpeg").toString().toLower();

}

ScanDataProcesser::~ScanDataProcesser(){
    QList<float> xs = pointCloud.keys();
    for (int i=0;i<xs.size(); i++){
        delete pointCloud[xs.at(i)];
    }

}

ScanDataProcesser::ScanDataProcesser(QString folder):
    QObject(),numFilesToProcess(0),numFilesProcessed(0),is_foambox_(true){
    id_=QUuid::createUuid().toString();
    dir_ = QDir(folder);
    QSettings s;
    img_format_ = s.value("scanner/imageformat",".jpeg").toString().toLower();
}


ScanDataProcesser::ScanDataProcesser(QString id, QString folder):
    QObject(),numFilesToProcess(0),numFilesProcessed(0),is_foambox_(true)
{
    id_=id;
    dir_ = QDir(folder);
    QSettings s;
    img_format_ = s.value("scanner/imageformat",".jpeg").toString().toLower();
}

void ScanDataProcesser::isFoamBox(bool box){
    is_foambox_=box;
}

void ScanDataProcesser::processScan(){
    processScan(dir_.absolutePath());
}

void ScanDataProcesser::calibrateWithScan(QString folder){
    QDir d(folder);

    qDebug()<<"Scan calibratation Folder: "<<d.absolutePath();
    QStringList filters;
    filters<<img_format_;
    QStringList files = d.entryList(filters);

    qDebug()<<"Files: \n"<<files;
    qDebug()<<"Filter:"<<filters;
    QString first = folder+"//"+files.takeFirst();
    cv::Mat baseImg = cv::imread( first.toStdString() );

    foreach(QString filename, files){
        QString file = folder+"//"+filename;
        cv::Mat nextimg = cv::imread(file.toStdString() );
        baseImg = baseImg+nextimg;
    }


    QSettings settings;
    QString default_path = QFileInfo(settings.fileName()).absolutePath();
    std::vector<int> params;
    params.push_back(CV_IMWRITE_JPEG_QUALITY);
    params.push_back(99);

    QString writelocation = default_path+"//"+"summed"+img_format_;
    cv::imwrite(writelocation.toStdString(),baseImg,params);
    settings.setValue("scanner/noisefile",writelocation);

}

void ScanDataProcesser::clearScanDir(){
    QDir dir(dir_);
    dir.setNameFilters(QStringList() << img_format_<<img_format_.toUpper());
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

}
void  ScanDataProcesser::processScan(QString folder){

    cv::Mat noisesum;
    QSettings settings;
    QString summed_noise_file = settings.value("scanner/noisefile","").toString();
    if(!summed_noise_file.isEmpty()){
        noisesum = cv::imread(summed_noise_file.toStdString());
    }
    if (!noisesum.data){
        qDebug()<<"scanner not calibrated, no noise file";
        emit scannerNotCalibrated();
//        return;
    }


    dir_ = QDir(folder);
    QFileInfoList list = dir_.entryInfoList();
    numFilesToProcess = list.size()-2;
    numFilesProcessed = 0;

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        processImage(fileInfo.fileName(),noisesum);


//        QImage img( fileInfo.fileName());
//        img = img.convertToFormat(QImage::Format_RGB888);
//        QPixmap m = QPixmap::fromImage(img);
//        m.load( fileInfo.fileName() );
//        addImage(x,m);
    }
}


void ScanDataProcesser::processImage(QString file, cv::Mat noise){

    //QString name = QString(file).toLower().replace(img_format_,"");
    float x = QString(file).toLower().replace(img_format_,"").toFloat();//file.split(".")[0].toFloat();
    //qDebug()<<"x: "<<x<<"File: "<<file;

    //qDebug()<<"Processing: "<<dir_.absoluteFilePath(file);
    ScanProcessing* worker = new ScanProcessing(x,dir_.absoluteFilePath(file),noise);
//    QThread* thread = new QThread;
//    worker->moveToThread(thread);

    connect(worker,SIGNAL(processed(float,QVector<FAHVector3>*)),this,SLOT(processedImage(float,QVector<FAHVector3>*)));

//    connect(thread, SIGNAL(started()), worker, SLOT(process()));
//    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
//    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
//    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
//    thread->start();
    worker->process();
}

void ScanDataProcesser::processedImage(float x, QVector < FAHVector3 >* row ){
    pointCloud[x]=row;
    numFilesProcessed++;
    qDebug()<<"Processed: "<<numFilesProcessed <<" of"<<numFilesToProcess;
    if(numFilesProcessed == numFilesToProcess){
        Scan* s = new Scan();
        s->setInitialData( makeGrid());
#ifdef DEBUGGING
        //clearScanDir();
#else
        clearScanDir();
#endif
        qDebug()<<"Made Scan";
        emit scanProcessed(s);
    }
}


XYGrid<float>* ScanDataProcesser::makeGrid(){
    QSettings settings;

    float Grid_Size_X = settings.value("scanner/x_step",2).toFloat(); /// need to save from elsewhere
    float Grid_Size_Y = settings.value("scanner/y_step",1).toFloat();
    float Tolerance = 0.3;
    float max_x=0;
    float min_x=0;
    float max_y=0;
    float min_y=0;
    float min_z=0;

    QList<float> xs= pointCloud.keys();
    for (int i=0;i<xs.size();i++){
        if (xs[i]<min_x){min_x = xs[i];}
        if (xs[i]>max_x){max_x = xs[i];}

        QVector< FAHVector3 >* row = pointCloud.value(xs[i]);
        for(int j=0;j<row->size();j++){
            if(max_y<row->at(j).y){max_y=row->at(j).y;}
            if(min_y>row->at(j).y){min_y=row->at(j).y;}
            if(min_z>row->at(j).z){min_z=row->at(j).z;}
        }
    }

    int nx = ceil((max_x-min_x)/Grid_Size_X);
    int ny = ceil((max_y-min_y)/Grid_Size_Y);

    qDebug()<<"NX: "<<nx<<" NY: "<<ny;
    qDebug()<<"minX: "<<min_x<<" minY: "<<min_y;
    qDebug()<<"maxX: "<<max_x<<" maxY: "<<max_y;
    qDebug()<<"minZ: "<<min_z;

    XYGrid<float>* grid = new XYGrid<float>(nx,ny,Grid_Size_X,Grid_Size_Y);

    for(int i=0; i<nx;i++){

        // Sort through each row and find the closest row to this grid.
//        qDebug()<<"i:"<<i;
        float x=-1;
        for(int j=0;j<xs.size();j++){
            if(   (xs.at(j)<(i*Grid_Size_X+Tolerance)) && (xs.at(j)>(i*Grid_Size_X-Tolerance) )){
                x=xs.at(j);
//                qDebug()<<"x:"<<i*GRID_SIZE <<"\tx'"<<xs.at(j);
            }
        }

        if(x<0){continue;} /// If no row continue to next row
        QVector< FAHVector3 >* row = pointCloud.value(x);

        // For that row find the point closest to the Y point
        for(int j=0; j<ny; j++){
//            float y = j*GRID_SIZE;// target point

            grid->operator ()(i,j)=0;

            for(int k=0;k<row->size();k++){

                float y_p = row->at(k).y-min_y; //compensate for negative min_y

                if(   (y_p<(j*Grid_Size_Y+Tolerance)) && (y_p>(j*Grid_Size_Y-Tolerance) )){
                    grid->operator ()(i,j)=row->at(k).z-min_z;
//                    qDebug()<<i<<","<<j<<":"<<row->at(k).z-min_z;
                    k=row->size();
                }

            }
        }


    }

    if(!is_foambox_){
        QVector<float> bounds = grid->getValueRange();
        float min = bounds.first();
        float max = bounds.last();
        for(int i=0; i<nx;i++){
            for(int j=0; j<ny; j++){
                grid->operator ()(i,j) = (min+max)-grid->at(i,j);
            }
        }
    }

    return grid;
}



