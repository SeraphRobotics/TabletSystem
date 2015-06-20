#ifndef PROCESSINGFUNCTIONS_H
#define PROCESSINGFUNCTIONS_H


#include <QImage>
#include "libraries/shared/fah-constants.h"
#include "DataStructures/xygrid.h"
#include "DataStructures/scan.h"
#include "DataStructures/manipulations.h"

/// LOOP MAKING

/**
 * @brief loopFromPoints Makes a loop using Heal points and fore points
 * @param healpts
 * @param forepts
 * @return FAHLoopInXYPlane
 */
FAHLoopInXYPlane* loopFromPoints(QVector< FAHVector3 > healpts, QVector< FAHVector3 > forepts);


FAHLoopInXYPlane* bottomLoopFromPoints(QVector< FAHVector3 > healpts, QVector< FAHVector3 > forepts);

/**
 * @brief secondOrder make a list of points for the heal
 * @param heal_pts
 * @return vector of points
 */
QVector< FAHVector3 > secondOrder(QVector< FAHVector3 >heal_pts, QVector< FAHVector3 > forepts, int nTimes=1000);

/**
 * @brief fourthOrder makes a list of points on a curve between to points with given slopes
 * @return vector of points
 */
QVector< FAHVector3 > fourthOrder(FAHVector3 Start,float startslope, FAHVector3 End,float endslope, int nTimes=1000);


/**
 * @brief bezier_curve used for front curve in loopFromPoints
 * @param points
 * @param nTimes
 * @return vector of points
 */
QVector< FAHVector3 > bezier_curve(QVector< FAHVector3 > points, int nTimes=1000);

/**
 * @brief bernstein_poly used
 * @param i
 * @param n
 * @param t
 * @return
 */
float bernstein_poly(int i, int n, float t);

/**
 * @brief projectGridOntoPlane
 * @param n
 * @param grid
 */
void projectGridOntoPlane(FAHVector3 n,FAHVector3 cent, XYGrid< float >* grid);

/**
 * @brief makePostingPlane
 * @param hp1
 * @param hp2
 * @param fp1
 * @param fp2
 * @return QVector<FAHVector3>
 * make a planes a plane using least squares method returns a vector of the normal and center
 */
QVector<FAHVector3> makePostingPlane(FAHVector3 hp1,FAHVector3 hp2,FAHVector3 fp1, FAHVector3 fp2);

FAHVector3 findHeal(XYGrid< float >* grid,QVector<FAHVector3> healpts);

/**
 * @brief minAlongLine
 * @param grid
 * @param p1
 * @param p2
 * @return minimum pt
 */
FAHVector3 minAlongLine(XYGrid< float >* grid, FAHVector3 p1, FAHVector3 p2);

/**
 * @brief normFrom3Pts
 * @param p1
 * @param p2
 * @param p3
 * @return plane vector
 */
FAHVector3 normFrom3Pts(FAHVector3 p1, FAHVector3 p2,FAHVector3 p3);


/**
 * @brief thresholdWithLoop
 * @param grid
 * @param loop
 * sets the grid to have a minimum of 0 based on the mininmum used before
 */
void thresholdWithLoop(XYGrid< float >* grid, FAHLoopInXYPlane *loop);

void anchorFront(XYGrid<float>* grid,QVector<FAHVector3>forepts);

void blurByBorder(XYGrid<float>* grid,FAHLoopInXYPlane* borderloop, int times);

void normalizeBorder(XYGrid<float>* grid, FAHLoopInXYPlane* borderloop, int times);

void blurInLoop(XYGrid<float>* grid,FAHLoopInXYPlane* borderloop, int times);

///IMAGE MAKING
/**
 * @brief makeHeightMap
 * @param grid
 * @return QImage height map
 */
QImage makeHeightMap(XYGrid< float >* grid);

int falseColorR(float dist);
int falseColorG(float dist);


QVector< FAHVector3> transformPointsWithPosting(FAHVector3 p1,FAHVector3 p2,Posting p);

QDebug operator<< (QDebug d,const FAHVector3 v);

float twoPtSlope(FAHVector3 p1, FAHVector3 p2);

int comb(int n, int k);

int factorial(int n);

#endif // PROCESSINGFUNCTIONS_H
