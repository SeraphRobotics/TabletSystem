#ifndef SCANTOSTLMCU_H
#define SCANTOSTLMCU_H

#include <QObject>
#include <QDebug>
#include <QSettings>

// local
#include "DataStructures/scanmanager.h"
#include "DataStructures/orthoticmanager.h"
#include "Controllers/orthoticscontroller.h"
#include "Controllers/printjobcontroller.h"
#include "DataStructures/orthotic.h"
#include "globals.h"

class ScanToSTLMCU : public QObject
{
    Q_OBJECT
public:
    explicit ScanToSTLMCU(QObject *parent = 0);



signals:

public slots:
    void processScan();

private slots:
    void boundaryLoopUpdated(FAHLoopInXYPlane* loop);
    void scanImageGenerated(QImage img);

private:
    FAHVector3 pointFromValues(QString ny,int nx,float xscale,float yscale);

private:
    ScanManager *sm;
    OrthoticManager *om;
    OrthoticController *oc;

};

#endif // SCANTOSTLMCU_H
