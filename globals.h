#ifndef GLOBALS_H
#define GLOBALS_H

#define DEBUG_FUNCTION_NAME qPrintable(QString("%1").arg(__PRETTY_FUNCTION__, -70, QLatin1Char(' ')))
#define DEBUG_FUNCTION_NAME_SHORT  qPrintable(QString("%1").arg(__func__, -70, QLatin1Char(' ')))
#include "libraries/shared/fah-constants.h"


void dumpToFile(QString filename, QString content);
void writeLoopToXDFL(FAHLoopInXYPlane* loop, QString file);
void printPoint(FAHVector3 pt);
FAHLoopInXYPlane* circle(float x, float y,float r);

#endif // GLOBALS_H

