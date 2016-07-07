#ifndef IMAGECONTOURSDETECTOR_H
#define IMAGECONTOURSDETECTOR_H

#include <QObject>
#include "View/UI_structs.h"

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
                        , QVector<QVector<QPoint> > &innerBorder
                        , QVariantList &outerBorder,
                        const int &startingXPos,
                        const int &startingYPos);
};

#endif // IMAGECONTOURSDETECTOR_H
