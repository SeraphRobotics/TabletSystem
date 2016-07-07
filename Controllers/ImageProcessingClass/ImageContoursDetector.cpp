#include "ImageContoursDetector.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <QDebug>
#include <QImage>
#include <QPainter>

using namespace cv; //TODO get rid of it
using namespace std; //TODO get rid of it

cv::Mat qImageToOpenCvMat(const QImage& qimage)
{
    cv::Mat mat = cv::Mat(qimage.height(),
                          qimage.width(),
                          CV_8UC4, (uchar*)qimage.bits(),
                          qimage.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows,
                           mat.cols,
                           CV_8UC3 );

    int from_to[] = { 0,0,  1,1,  2,2 };
    cv::mixChannels( &mat,
                     1, &mat2,
                     1, from_to,
                     3 );
    return mat2;
}

ImageContoursDetector::ImageContoursDetector(QObject *parent) :
    QObject(parent)
{
}

void ImageContoursDetector::detectContours(const QImage &image
                                           , QVector<QVector<QPoint> > &innerBorder
                                           , QVariantList &outerBorder,
                                           const int &startingXPos,
                                           const int &startingYPos)
{
    Mat src;
    Mat src_gray;
    int thresh = 50;

    qDebug()<<"Image size"<<image.size();

    src = qImageToOpenCvMat(image);

    /// Convert image to gray and blur it
    cvtColor( src, src_gray, CV_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );

    auto thresh_callback = [&] ()
    {
        Mat canny_output;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        vector<Point> outer;

        /// Detect edges using canny
        Canny(src_gray, canny_output, thresh, thresh*2, 3);
        /// Find contours
        ///
        /// @note find only external border.
        findContours(canny_output, contours, hierarchy, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

        /// Draw contours
        Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);

        qDebug()<<"Detect only outer border ,find Contours: "<<contours.size();

        for( unsigned int i = 0; i<contours.size(); i++ )
        {
            qDebug()<<"Hierarhy is "<<hierarchy[i][0];
            CvScalar blue = CV_RGB(0,0,250);
            vector<Point> &element = contours[i];

            for(Point point : element)
            {
                int newX = point.x+startingXPos;
                int newY = point.y+startingYPos;
                outer.push_back(point);
                outerBorder.append(QPointF(newX, newY));
                qDebug()<<"Added Point to outer border:"<<newX<<", "<<newY;
            }
            drawContours(drawing, contours, i, blue, 2, 8, hierarchy, 0, Point());
        }

        /// @note find all borders.
        findContours(canny_output,
                     contours,
                     hierarchy,
                     RETR_CCOMP,
                     CV_CHAIN_APPROX_SIMPLE,
                     Point(0, 0));

        for( unsigned int i = 0; i< contours.size(); i++ )
        {
            qDebug()<<"Hierarhy level is "<<hierarchy[i][0];

            if(hierarchy[i][0] > 1)
            {
                CvScalar red = CV_RGB(250,0,0);

                vector<Point> &element = contours[i];

                drawContours(drawing, contours, i, red, 2, 8, hierarchy, 0, Point());

                for(Point &point : element)
                {
                    point.x = point.x+startingXPos;
                    point.y = point.y+startingYPos;
                    qDebug()<<"Change Point in inner border:"<<point.x<<", "<<point.y;
                }

                if((std::find(outer.begin(), outer.end(), element[0]) != outer.end()))
                    continue;

                QVector<QPoint> qelement;
                for (Point &point : element) {
                    QPoint p(point.x, point.y);
                    qelement.append(p);
                }

                innerBorder.append(qelement);
            }
        }
    };

    thresh_callback();
}
