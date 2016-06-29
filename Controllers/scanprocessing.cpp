#include "scanprocessing.h"
#include <QFileInfo>
#include <QDebug>
#include <math.h>
#include <qalgorithms.h>


ScanProcessing::ScanProcessing(float x, QString imagelocation, cv::Mat noisefile, QObject *parent) :
    QObject(parent),  imagelocation_( imagelocation),x_(x)
{
    noise = noisefile;
}



void ScanProcessing::process(){
//    cv::Mat dest;
//    dest = cv::imread(imagelocation_.toStdString());
//    QPixmap m = QPixmap::fromImage(QImage((unsigned char*) dest.data, dest.cols, dest.rows, dest.step, QImage::Format_RGB888));


    cv::Mat img = cv::imread(imagelocation_.toStdString());
    if(!img.data){
        qDebug()<<"faled to read "<<imagelocation_;
        return;
    }
    if(noise.data){
        img = img-noise;
#ifdef DEBUGGING
        QString name = QString("d-") + QString::number(x_) + ".jpeg";
        cv::imwrite(name.toStdString(), img);
#endif
    }
    bool metOne = false;

    /// ////////////////////////////////////////////////////////////////
    //set-up camera parameters
    //TODO - change these to the actual values
    double fu = 1388;                               // (fu,fv) are the focal lengths
    double fv = 1388;                               // expressed in pixel-related units
    double u0 = 970;                                // (u0,v0) the coordinates
    double v0 = 553;                                // of the principal point [pxl]
    double k1 = 0.102;                              // (k1, k2, k3, k4, k5 ,k6)
    double k2 = -0.170;                             // radial distortion coefficients
    double k3 = 0;
    double k4 = 0;
    double k5 = 0;
    double k6 = 0;
    double p1 = 0;                                  //(p1,p2) tangential distortion coefficients
    double p2 = 0;                                  //
    double h = 203.14;                              // the distance the laser and the surface
    double b = 182.54;                              // distance between camera and laserplate

    double uOff = 28;                               //  horizontal Offset
    double vOff = 28;                               //  vertical Offset
    Q_UNUSED(uOff);
    Q_UNUSED(vOff);

    double alpha = 225*CV_PI/180.;
    double beta  = 0*CV_PI/180.;
    double gamma = 180*CV_PI/180.;

    double z;                                       //  Z
    double y;                                       //  Y



    // Camera matrix
    cv::Mat K = (cv::Mat_<double>(4, 4) <<
                 -fu,     0,         0,      u0,
                 0,       fv,        0,      v0,
                 0,       0,         1,      0,
                 0,       0,         0,      1);

    // Distortion matrix
    cv::Mat distCoef = (cv::Mat_<double>(8, 1) <<
                        k1,
                        k2,
                        p1,
                        p2,
                        k3,
                        k4,
                        k5,
                        k6);

    // Rotation matrices around the X, Y and Z axis
    cv::Mat RX = (cv::Mat_<double>(4, 4) <<
                  1,          0,           0,   0,
                  0, cos(alpha), -sin(alpha),   0,
                  0, sin(alpha),  cos(alpha),   0,
                  0,          0,           0,   1);

    cv::Mat RY = (cv::Mat_<double>(4, 4) <<
                  cos(beta), 0, -sin(beta), 0,
                  0, 1,          0, 0,
                  sin(beta), 0,  cos(beta), 0,
                  0, 0,          0, 1);

    cv::Mat RZ = (cv::Mat_<double>(4, 4) <<
                  cos(gamma), -sin(gamma), 0, 0,
                  sin(gamma),  cos(gamma), 0, 0,
                  0,          0,           1, 0,
                  0,          0,           0, 1);

    // translation matrix
    cv::Mat T = (cv::Mat_<double>(4, 4) <<
                 1,    0,  0,   b,
                 0,    1,  0,   0,
                 0,    0,  1,   -h,
                 0,    0,  0,   1);




        QVector< FAHVector3 >* row = new QVector< FAHVector3 >();


//        img = QPixmapToCvMat(image_,true);


        /// ////////////////////////////////////////////
        cv::medianBlur ( img, img, 7 );

        // Transform the color to GrayScale
        cv::cvtColor(img, img, CV_BGR2GRAY);

        // Binary transform
        int thresholdValue = 45;
        cv::threshold(img, img, thresholdValue, 255, 0);

        /// ///////////////////////////////
        for (int u = 0; u < img.cols; u++){
            for (int v = 0; v < img.rows; v++ ){
                if (img.at<uchar>(v, u) > 0) {
                    if(!metOne){
                        // Undistort
                        double x_dist = (u - u0) / fu;
                        double y_dist = (v - v0) / fv;

                        double r = sqrt(pow(x_dist,2) + pow(y_dist,2));


                        double x_undist = x_dist*(1 + k1*pow(r,2) + k2*pow(r,4))/(1);

//                        double x_undist = x_dist*(1 + k1*pow(r,2) + k2*pow(r,4)+ k3*pow(r,6))/(1 + k4*pow(r,2) + k5*pow(r,4) + + k6*pow(r,6))
//                                + 2*p1*x_dist*y_dist + p2*(pow(r,2) + 2*pow(x_dist,2));

                        double y_undist = y_dist*(1 + k1*pow(r,2) + k2*pow(r,4))/(1);

//                        double y_undist = y_dist*(1 + k1*pow(r,2) + k2*pow(r,4)+ k3*pow(r,6))/(1 + k4*pow(r,2) + k5*pow(r,4) + + k6*pow(r,6))
//                                + p1*(pow(r,2)+2*pow(y_dist,2)) + 2*p2*x_dist*y_dist;

                        double u_undist = fu*x_undist + u0;
                        double v_undist = fv*y_undist + v0;

                        // Calculate the world coordinates
                        z = h-((fv*h-(v0-v_undist)*b)/(fv*b+h*(v0-v_undist)))*b ;
                        y = (u_undist-u0)*sqrt(h*h+b*b)/fu;


                        FAHVector3 v;
                        v.x=x_;
                        v.y=y;
                        v.z=z;
                        row->push_back(v);

                        metOne = true;
                    }
                }
            }
            metOne = false;
        }
        qDebug()<<x_<<": "<<row->size();
        emit processed(x_,row);
//        image_.save(QString::number(x_)+QString(".JPEG"));
        emit finished();
}



cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true ){
      switch ( inImage.format() )
      {
         // 8-bit, 4 channel
         case QImage::Format_RGB32:
         {
            cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

            return (inCloneImageData ? mat.clone() : mat);
         }

         // 8-bit, 3 channel
         case QImage::Format_RGB888:
         {
            if ( !inCloneImageData )
               qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";

            QImage   swapped = inImage.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
         }

         // 8-bit, 1 channel
         case QImage::Format_Indexed8:
         {
            cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

            return (inCloneImageData ? mat.clone() : mat);
         }

         default:
            qDebug() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
      }

      return cv::Mat();
   }

   // If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
   // with the cv::Mat directly
   //    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true ){
      return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
   }

