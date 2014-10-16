#include "gcodetests.h"
#include "libraries/shared/stl/stlfile.h"
#include "Controllers/gcodecontroller.h"
#include <QDebug>
#include <QSettings>

void TestGCodeSystem(){
    QSettings s;
    s.setValue("printing/dir",QFileInfo(s.fileName()).absolutePath());
    s.setValue("printing/slicer","C:\\Program Files\\Repetier-Host\\Slic3r\\Slic3r-console.exe");
    s.setValue("printing/valving-python-script","toValve.py");
    s.setValue("printing/merge-python-script","merge.py");
    s.setValue("printing/output-name","merged.gcode");

    GcodeController gc;
    QStringList stlfiles;
    stlfiles<<"Ball.stl";
    stlfiles<<"box.stl";

    QStringList inis;
    inis<<"p.ini";
    inis<<"ms.ini";

    qDebug()<<"RUNNING";
    for(int i=0; i<stlfiles.size(); i++){
        QString filepath = stlfiles.at(i);
        qDebug()<<"STL: "<<filepath;
        gc.addSTLMeshINIPair(filepath,inis.at(i),false);
    }
    qDebug()<<"ready to generate";
    gc.generateGcode();
    qDebug()<<"generated";

}
