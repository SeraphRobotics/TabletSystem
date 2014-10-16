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
    stlfiles<<"TestCube.stl";
//    stlfiles<<"Ball.stl";
    stlfiles<<"box.stl";

    QStringList inis;
    inis<<"p.ini";
    inis<<"ms.ini";

    qDebug()<<"RUNNING";
    for(int i=0; i<stlfiles.size(); i++){
        STLFile f;
        QString filepath = QDir::toNativeSeparators(QDir::currentPath())+QDir::separator()+stlfiles.at(i);
        filepath.replace("\\","\\\\");
        qDebug()<<"STL: "<<filepath;

        f.read(filepath);
        qDebug()<<"read ";
        return;
        qDebug()<<"with size:"<<f.GetMesh()->GetFacets().size();
        STLMesh* m = new STLMesh(f.GetMesh());
        qDebug()<<"Loaded file";
        gc.addSTLMeshINIPair(m,inis.at(i),false);
    }
    gc.generateGcode();

}
