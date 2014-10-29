#ifndef GCODECONTROLLER_H
#define GCODECONTROLLER_H

#include <QObject>
#include "libraries/shared/stl/STLMesh.h"
#include "libraries/shared/stl/stlfile.h"
#include "DataStructures/printingstructs.h"
#include <QDir>


class GcodeController : public QObject
{
    Q_OBJECT
public:
    explicit GcodeController(QObject *parent = 0);

signals:
    void processingStarted();
    void processingFailed();
    void processingComplete();
    void gcodeGenerated(QString gcode);

public slots:
    void addSTLMeshINIPair(QString stlfile,QString inifile,bool isValved);
    void generateGcode();
    void setSlicer(QString slicer);
    void setDir(QString dir);

private:
    QList<meshpair> pairs;
    QDir dir_;


};

#endif // GCODECONTROLLER_H
