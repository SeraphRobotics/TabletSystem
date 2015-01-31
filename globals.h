#ifndef GLOBALS_H
#define GLOBALS_H

#define DEBUG_FUNCTION_NAME qPrintable(QString("%1").arg(__PRETTY_FUNCTION__, -70, QLatin1Char(' ')))
#define DEBUG_FUNCTION_NAME_SHORT  qPrintable(QString("%1").arg(__func__, -70, QLatin1Char(' ')))
#include "libraries/shared/fah-constants.h"


void writeLoopToXDFL(FAHLoopInXYPlane* loop, QString file);
void printPoint(FAHVector3 pt);
FAHLoopInXYPlane* circle(float x, float y,float r);


/*
void printPoint(FAHVector3 pt){
    printf("\n %.4f\t%.4f\t%.4f",pt.x,pt.y,pt.z);
}

FAHLoopInXYPlane* circle(float x, float y,float r){
    FAHLoopInXYPlane* loop = new FAHLoopInXYPlane();
    for( int i=0; i<100; i++){
        float theta = i*Math::k2Pi/100.0;
        FAHVector3 pt(0.5*r*sin(theta)+x,r*cos(theta)+y,0);
        loop->add(pt);
    }
    return loop;
}
*/

#endif // GLOBALS_H

