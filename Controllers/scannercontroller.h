#ifndef SCANNERCONTROLLER_H
#define SCANNERCONTROLLER_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "DataStructures/scannerarduinointerface.h"
#include <QTimer>

class ScannerController : public QObject
{
    Q_OBJECT
public:
    explicit ScannerController(QObject *parent = 0);

signals:
    void ScanComplete();
    void ScanError();

    void scanStarted() const;
    void progressUpdated(int progress) const;

public slots:
    void portSelected(QString port);
    void disconnected();
    void startScan();


private slots:
    void buttonPress();
    void scannerError();
    void scanComplete();
    void setupCamera();

    void ScanStep();

private:
    int camNumber_;
    float scandistance_;
    float stepsize_;
    float framerate_;
    int width_;
    int height_;
    float dist;
    QTimer* timer_;
    ScannerArduinoInterface* sai_;
    cv::VideoCapture capwebcam;
    cv::Mat matOriginal;
    QDir dir_;

    bool m_started = false;
};

#endif // SCANNERCONTROLLER_H
