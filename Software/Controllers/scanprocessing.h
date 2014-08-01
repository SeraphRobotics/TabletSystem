#ifndef SCANPROCESSING_H
#define SCANPROCESSING_H

#include <QObject>
#include <QDir>
#include <QVector>
#include <float.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "libraries/shared/fah-constants.h"
#include <QPixmap>


class ScanProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ScanProcessing(float x, QPixmap image, QObject *parent = 0);

signals:
    void processed(float x, QVector < FAHVector3 >* );
    void finished();

public slots:
    void process();


private:
    QPixmap image_;
    float x_;

};


cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData);
cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData);

#endif // SCANPROCESSING_H
