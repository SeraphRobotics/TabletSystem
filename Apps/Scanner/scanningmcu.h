#ifndef SCANNINGMCU_H
#define SCANNINGMCU_H

#include <QObject>
#include "Controllers/scannerwatcher.h"
#include "Controllers/scannercontroller.h"
#include "Controllers/scandataprocesser.h"
#include "DataStructures/scanmanager.h"


class ScanningMCU : public QObject
{
    Q_OBJECT
public:
    explicit ScanningMCU(QObject *parent = 0);
    void processNow();

signals:
    void processScanFolder(QString folder);

private slots:
    void scanComplete();


private:
    ScannerWatcher* sw_;
    ScannerController* sc_;
    ScanDataProcesser* sp_;
    ScanManager* sm_;

};

#endif // SCANNINGMCU_H
