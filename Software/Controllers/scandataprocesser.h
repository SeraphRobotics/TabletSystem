#ifndef SCANDATAPROCESSER_H
#define SCANDATAPROCESSER_H

#include <QObject>
#include <QPixmap>
#include <QUuid>
#include <QtCore>
#include <float.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QMap>
#include <QVector>
#include "xygrid.h"
#include "DataStructures/scan.h"

class ScanDataProcesser : public QObject
{
    Q_OBJECT
public:
    explicit ScanDataProcesser(QObject *parent = 0);
    ~ScanDataProcesser();
    ScanDataProcesser(QString id, QString folder);


signals:
    /**
     * @brief scanProcessed is emited when a scan item is created
     * @param s
     */
    void scanProcessed(Scan* s);
    
public slots:


    void processScan();
    /**
     * @brief processScan loads the folater and makes a Scan object
     * @param folder
     */
    void processScan(QString folder);

    void calibrateWithScan(QString folder);

    void scannerNotCalibrated();

private slots:



    void processImage(QString file);

    /**
     * @brief processedImage uses scanprocessing objects to process them in multi threaded fashion
     * @param x
     * @param row
     */
    void processedImage(float x, QVector < FAHVector3 >* row);

    /**
     * @brief makeGrids from pointCloud
     * @return XYGrid
     */
    XYGrid<float>* makeGrid();

public:
    QMap<float, QVector < FAHVector3 >* > pointCloud;
    QString id_;
    QDir dir_;
    int numFilesToProcess;
    int numFilesProcessed;

};






#endif // SCANDATAPROCESSER_H
