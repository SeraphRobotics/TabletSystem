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
    explicit PrintJobController(QObject *parent = 0);
    ~PrintJobController();

signals:
    void PrintJobComplete();
    void PrintJobFailed(QString error);
    void GcodeGenerated(QString gcode);

public slots:
    void RunPrintJob(printjobinputs pji);

private slots:
    void processingStarted();
    void processingFailed();
    void processingComplete();
    void gcodeGenerated(QString gcode);

private:
    QStringList makeIniFiles(Manipulation m);

private:
    GcodeController* gc_;
    QString dir_;
};

#endif // PRINTJOBCONTROLLER_H
