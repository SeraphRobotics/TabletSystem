#include "orthoticcontrollertester.h"
#include "debugfunctions.h"
#include "Controllers/stlgeneration.h"
#include <QDebug>

OrthoticControllerTester::OrthoticControllerTester(QObject *parent) :
    QObject(parent)
{
}

void OrthoticControllerTester::scanImageGenerated(QImage img){
    img.save("img.jpeg");
}

void OrthoticControllerTester::boundaryLoopUpdated(FAHLoopInXYPlane* loop){
    writeLoopToXDFL(loop,"Loop.xdfl");
}

void OrthoticControllerTester::borderGenerated(Border b){
    for(int i=0; i<b.size();i++){
        qDebug()<<b.at(i).x()<<","<<b.at(i).y();
    }
}

void OrthoticControllerTester::frontPoints(QList<QPointF> ps){
    for(int i=0;i<ps.size();i++){
        qDebug()<<ps.at(i).x()<<","<<ps.at(i).y();
    }
}

void OrthoticControllerTester::healPoints(QList<QPointF> ps){
    for(int i=0;i<ps.size();i++){
        qDebug()<<ps.at(i).x()<<","<<ps.at(i).y();
    }
}

void OrthoticControllerTester::stlsGenerated(QList<View_3D_Item> items){
    for(int i=0;i<items.size();i++){
//        stlToFile(items.at(i).mesh, QString::number(i)+".stl");
    }
}

void OrthoticControllerTester::topCoat(Top_Coat tc){
    qDebug()<<"set tc:"<<tc.thickness;
}
