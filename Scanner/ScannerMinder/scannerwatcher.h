#ifndef SCANNERWATCHER_H
#define SCANNERWATCHER_H

#include <QObject>

#include "qextserialenumerator.h"
#include <QTimer>


class ScannerWatcher : public QObject
{
    Q_OBJECT
public:
    explicit ScannerWatcher(QObject *parent = 0);

signals:
    void disconnected();
    void scannerPort(QString port);

public slots:

private slots:

    void updatePorts();


private:
    QTimer* timer_;
    QextSerialEnumerator* enumer;
    QString portName_;
    QList<QextPortInfo> ports_;
};

#endif // SCANNERWATCHER_H
