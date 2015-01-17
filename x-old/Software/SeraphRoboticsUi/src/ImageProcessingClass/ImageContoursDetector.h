#ifndef IMAGECONTOURSDETECTOR_H
#define IMAGECONTOURSDETECTOR_H

#include <QObject>

#include "opencv2/imgproc/imgproc.hpp"

#include "UI_structs.h"

/** @brief Class which takes Image data, detects borders (inner and outer)
 * and returns as List of (x,y) points for borders in image parent system coordinates. */
class ImageContoursDetector : public QObject
{
    Q_OBJECT
public:
    explicit ImageContoursDetector(QObject *parent = 0);

public slots:
    /**
     * @brief detectContours Function used to detect borders using Canny edge detector.
     * @param data
     * @param innerBorder
     * @param outerBorder
     * @param startingXPos
     * @param startingYPos
     */
    void detectContours(const QImage &data
                        ,QList<std::vector<cv::Point> > &innerBorder
                        ,QVariantList &outerBorder,
                        const int &startingXPos,
                        const int &startingYPos);

private:
    /**
     * @brief _qImageToOpenCvMat Function used for converting Between cv::Mat and QImage
     * @param qimage
     * @return
     */
    cv::Mat _qImageToOpenCvMat(const QImage& qimage);
};

#endif // IMAGECONTOURSDETECTOR_H
