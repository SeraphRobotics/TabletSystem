#ifndef PRINTJOBTESTER_H
#define PRINTJOBTESTER_H

#include <QObject>
#include "DataStructures/printingstructs.h"
#include "Controllers/printjobcontroller.h"

class PrintJobTester : public QObject
{
    Q_OBJECT
public:
    explicit PrintJobTester(PrintJobController* pjc,QObject *parent = 0);

signals:

public slots:
    void GcodeGenerated(QString gcode);
    void printJobInputs(printjobinputs p);

private:
    PrintJobController* pjc_;
};

#endif // PRINTJOBTESTER_H
