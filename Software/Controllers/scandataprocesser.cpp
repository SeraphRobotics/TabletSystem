#include "scandataprocesser.h"
#include <QSettings>
#include "scanprocessing.h"
#include <QThread>
#include <QUuid>

ScanDataProcesser::ScanDataProcesser(QObject *parent) :
    QObject(parent),numFilesToProcess(0),numFilesProcessed(0)
{
    id_=QUuid::createUuid().toString();
    dir_=QDir::current();
}

ScanDataProcesser::~ScanDataProcesser(){
    QList<float> xs = pointCloud.keys();
    for (int i=0;i<xs.size(); i++){
        delete pointCloud[xs.at(i)];
    }

}

ScanDataProcesser::ScanDataProcesser(QString id, QString folder):
    QObject(),numFilesToProcess(0),numFilesProcessed(0)
{
    id_=id;
    dir_ = QDir(folder);
}

void ScanDataProcesser::processScan(){
    processScan(dir_.absolutePath());
}

void  ScanDataProcesser::processScan(QString folder){

    dir_ = QDir(folder);
    QFileInfoList list = dir_.entryInfoList();
    numFilesToProcess = list.size();
    numFilesProcessed = 0;

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        float x = fileInfo.fileName().split(".")[0].toFloat();

        cv::Mat dest;
        dest = cv::imread(fileInfo.fileName().toStdString());
//        cv::cvtColor(matOriginal, dest,CV_BGR2RGB);
        QPixmap m = QPixmap::fromImage(QImage((unsigned char*) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888));
//        QImage img( fileInfo.fileName());
//        img = img.convertToFormat(QImage::Format_RGB888);
//        QPixmap m = QPixmap::fromImage(img);
//        m.load( fileInfo.fileName() );
        addImage(x,m);
    }
}


void ScanDataProcesser::addImage(float x, QPixmap pxmap){

    QThread* thread = new QThread;
    ScanProcessing* worker = new ScanProcessing(x,pxmap);
    worker->moveToThread(thread);

    connect(worker,SIGNAL(processed(float,QVector<FAHVector3>*)),this,SLOT(processedImage(float,QVector<FAHVector3>*)));

    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();


}

void ScanDataProcesser::processedImage(float x, QVector < FAHVector3 >* row ){
    pointCloud[x]=row;
    numFilesProcessed++;

    if(numFilesProcessed == numFilesToProcess){
        Scan* s = new Scan();
        s->setInitialData( makeGrid());
        emit scanProcessed(s);
    }
}


XYGrid<float>* ScanDataProcesser::makeGrid(){
    float GRID_SIZE = 2; /// need to save from elsewhere
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

    int nx = ceil((max_x-min_x)/GRID_SIZE);
    int ny = ceil((max_y-min_y)/GRID_SIZE);

    qDebug()<<"NX: "<<nx<<" NY: "<<ny;
    qDebug()<<"minX: "<<min_x<<" minY: "<<min_y;
    qDebug()<<"maxX: "<<max_x<<" maxY: "<<max_y;
    qDebug()<<"minZ: "<<min_z;

    XYGrid<float>* grid = new XYGrid<float>(nx,ny,GRID_SIZE);

    for(int i=0; i<nx;i++){

        // Sort through each row and find the closest row to this grid.
//        qDebug()<<"i:"<<i;
        float x=-1;
        for(int j=0;j<xs.size();j++){
            if(   (xs.at(j)<(i*GRID_SIZE+Tolerance)) && (xs.at(j)>(i*GRID_SIZE-Tolerance) )){
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

                if(   (y_p<(j*GRID_SIZE+Tolerance)) && (y_p>(j*GRID_SIZE-Tolerance) )){
                    grid->operator ()(i,j)=row->at(k).z-min_z;
//                    qDebug()<<i<<","<<j<<":"<<row->at(k).z-min_z;
                    k=row->size();
                }

            }
        }


    }

    return grid;
}



