#ifndef DEBUGFUNCTIONS_H
#define DEBUGFUNCTIONS_H
#include "../libraries/shared/fah-constants.h"

/// DEGUB FUNCTIONS
void writeLoopToXDFL(FAHLoopInXYPlane* loop, QString file);
void printPoint(FAHVector3 pt);
FAHLoopInXYPlane* circle(float x, float y,float r);
#endif // DEBUGFUNCTIONS_H
