#ifndef SCANNERFUNCTIONS_H
#define SCANNERFUNCTIONS_H

#include "../libraries/shared/stl/stlmesh.h"
#include "xygrid.h"


typedef QMap<int,QVector<int> > BoundaryMap;


/// BOUNDARY LOOP DETECTION

/**
 * returns a maping betwen the j(y index) values that have features inside of them,
 * and the boundary values of i for the left and right.
 **/
BoundaryMap boundaryGridDetection(XYGrid<float>* grid);

/**
 * Generates the boundary loop from the grid data based ona a trigger event.
 **/
FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid, int offsetFromBorder, float slopetrigger, float sensitivity);

/**
 * Generates the boundary loop from the grid data based ona a trigger event.
 **/
FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid);




/// LOOP Manipulation system

/**
 * smooths out a loop running an XY averaging algorithm.
 **/
FAHLoopInXYPlane smoothLoop(FAHLoopInXYPlane *loop, int times, float ylimit);

/**
 * Maps the Z value of the loop onto the grid
 **/
FAHLoopInXYPlane* mapOntoGrid(FAHLoopInXYPlane* loop, XYGrid<float>* grid, bool withheights=true);


void sortLoop(FAHLoopInXYPlane* loop);

/// System for getting height data

/**
 * returns the height at point x,y in the grid
 **/
template <class T>
float getHeightAt(float x, float y,XYGrid<T>* grid);


/**
 * returns the height at XYZ defined by the planes defined by p1-p4
 **/
float getHeightAt(float x, float y, FAHVector3 p1, FAHVector3 p2, FAHVector3 p3, FAHVector3 p4);

/**
 * returns true if the point is inside the outerloop and outside the innerloop
 **/
bool loopsContain(FAHVector3 pt, const FAHLoopInXYPlane *OuterLoop, QList<FAHLoopInXYPlane *> innerLoops);

/**
 * returns in pointsInGrid all of the points in the grid ij->i+1,j+1 in an ordered list
 **/
bool getPointsInGrid(int i, int j, float stepsize, FAHLoopInXYPlane *outerloop , QList<FAHLoopInXYPlane*> loops, QVector<FAHVector3>* pointsInGrid);


QVector<FAHVector3> intersection_points(QList<FAHLine> lines, const FAHLoopInXYPlane* outerloop, QList<FAHLoopInXYPlane*> loops);

float ptOnLine(FAHLine line, FAHVector3 pt);

/// Cleanup helper functions
void removedDuplicatePoints(QVector<FAHVector3>* vector);

void removePointsIfInList(QVector<FAHVector3>* vector, QVector<FAHVector3> *newvector, QList<FAHVector3> points);


QVector<FAHVector3> sortByIndex(QVector<FAHVector3> vec,int index, bool reverse = true);



/// DEGUB FUNCTIONS
//void writeLoopToXDFL(const FAHLoopInXYPlane& loop, QString file);
//void printPoint(FAHVector3 pt);

#endif // SCANNERFUNCTIONS_H
