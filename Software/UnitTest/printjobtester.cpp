#include "printjobtester.h"
#include <QDebug>

PrintJobTester::PrintJobTester(PrintJobController *pjc, QObject *parent) :
    QObject(parent)
{
    pjc_ = pjc;
    connect(pjc_,SIGNAL(GcodeGenerated(QString)),this,SLOT(GcodeGenerated(QString)));
}

void PrintJobTester::GcodeGenerated(QString gcode){

    QFile f("processed.gcode");
    f.open(QFile::WriteOnly);
    QTextStream fs(&f);
    fs<<gcode;
    f.close();
    qDebug()<<"DONE";
}
void PrintJobTester::printJobInputs(printjobinputs p){
    pjc_->RunPrintJob(p);
}
