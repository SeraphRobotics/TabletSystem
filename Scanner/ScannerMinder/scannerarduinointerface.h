#ifndef SCANNERARDUINOINTERFACE_H
#define SCANNERARDUINOINTERFACE_H

#include <QObject>
#include "qextserialport.h"

class ScannerArduinoInterface : public QObject
{
    Q_OBJECT
public:
    explicit ScannerArduinoInterface();
    ScannerArduinoInterface(QString port, BaudRateType baudrate, QObject *parent = 0);

signals:
    void buttonPressed();
    void scanMovementCompleted();
    void scanIncremented();
    void errored();

public slots:
    bool connectPort(QString port, BaudRateType baudrate);
    bool isReady();
    void startScan();
    void scanStep();
    void endScan();
    void laserOn();
    void laserOff();
    void ledOn();
    void ledOff();
    void home();
    void disconnect();

private slots:
    void _write(QString s);
    void onDataAvailable();

private:
    QextSerialPort* port_;
//    bool Pressed
};

#endif // SCANNERARDUINOINTERFACE_H
