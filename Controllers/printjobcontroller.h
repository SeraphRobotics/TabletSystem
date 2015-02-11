#ifndef PRINTJOBCONTROLLER_H
#define PRINTJOBCONTROLLER_H

#include <QObject>
#include "gcodecontroller.h"
#include "libraries/shared/stl/stlfile.h"
#include "DataStructures/printingstructs.h"
#include "Controllers/mergecontroller.h"
#include <QTimer>

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
    void slicingFailure(QString error);
    void topcoatMade(QString file);
    void startMerge();
    void mergeSucessful(QString gcode);




private:
    void makeIniFiles(QString stlfilename, manipulationpair pair);

private:
    Orthotic* orth_;
    QThread* workthread;
    QString dir_;
    int numSTLsRepaired;
    int numSTLsToRepair;
    int numSTLsSliced;
    int numSTLToSlice;
    bool topcoatdone;

    file_z_pair shell_;
    QList< file_z_pair> pad_files_;
    file_z_pair topcoat_file_;

    QTimer* timer_;

};

#endif // PRINTJOBCONTROLLER_H
