#ifndef PRINTJOBCONTROLLER_H
#define PRINTJOBCONTROLLER_H

#include <QObject>
#include "gcodecontroller.h"
#include "libraries/shared/stl/stlfile.h"
#include "DataStructures/printingstructs.h"

class PrintJobController : public QObject
{
    Q_OBJECT
public:
    explicit PrintJobController(Orthotic* orth, QObject *parent = 0);
    ~PrintJobController();

signals:
    void PrintJobComplete();
    void PrintJobFailed(QString error);
    void GcodeGenerated(QString gcode);

public slots:
    void RunPrintJob();

private slots:

    void stepFailed(QString s);

    void repairSucessful();
    void slicingSucessful();

    void topcoatMade(QString file);
    void startMerge();
    void mergeSucessful(QString gcode);




private:
    QStringList makeIniFiles(float stiffness);

private:
    Orthotic* orth_;
    QThread* workthread;
    QString dir_;
    int numSTLsRepaired;
    int numSTLsToRepair;
    int numSTLsSliced;
    int numSTLToSlice;
    bool topcoatdone;


};

#endif // PRINTJOBCONTROLLER_H
