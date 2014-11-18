#ifndef STLGENERATION_H
#define STLGENERATION_H


#include "scannerfunctions.h"





/// STL GENERATING FUNCTIONS

/**
 *returns a mesh of the grid.
 *It makes and STL with the heights inside the loop specified.
 *heights are specfied by the grid
 *All walls are straight
 **/
template <class T>
STLMesh* makeSTLfromScanSection(XYGrid<T>* grid, FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops);


/**
 *returns a mesh of the grid. It meshes all points in the grid
 **/
template <class T>
STLMesh* makeSTLfromScan(XYGrid<T>* grid );


/**
 * returns the loop translated proprotional to the height of the loop ds = z*tan(theta)
 * theta is measured in degrees
 * if z<threshold no translation
 **/
FAHLoopInXYPlane* angledBase(float theta, float min_threshold, float max_threshold, FAHLoopInXYPlane *loop);



void STLFromSection(STLMesh *mesh, XYGrid<float>* grid, FAHLoopInXYPlane *angled,  FAHLoopInXYPlane *OuterLoop, QList<FAHLoopInXYPlane *> innerLoops);
/// STL Helper functions

/**
 * Adds 2+ triangles to STL. it connect hte points inside the outerloop and
 **/
template <class T>
void addSquareToSTL(int i, int j, XYGrid<T>* grid,STLMesh* mesh,
                    FAHLoopInXYPlane* OuterLoop,
                    QList<FAHLoopInXYPlane*> innerLoops, float setz, bool top=true);


enum kChannelType{cleft,cright,ctop,cbottom,
                  ctlc,ctrc,cbrc,cblc,
                 ctb,clr,cltb,crtb};
void addChannelToSTL(int i, int j, XYGrid<float>* grid, STLMesh* mesh,
                     FAHLoopInXYPlane* OuterLoop,
                     QList<FAHLoopInXYPlane*> innerLoops,
                     kChannelType type, float depth);

/**
 * adds a linear extrusion of the loop to the mesh
 **/
template <class T>
void addLoopToSTL(const FAHLoopInXYPlane& loop,XYGrid<T>* grid,STLMesh* mesh,bool inner );

void addLoopAtHeight(STLMesh* mesh, FAHLoopInXYPlane* Loop, float z,bool outside);

void addBetweenTwoLoopsToSTL(STLMesh* mesh,
                    FAHLoopInXYPlane* OuterLoop,
                    FAHLoopInXYPlane* innerLoops,
                    bool outside = true);


/**
 * Adds a facet with the points listed as the corners with either a normal facing up (true) or down (false)
 **/
void addFacetWithNormal(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh, bool zpositive);

/**
 * Adds a facet with the points in the direction specified
 **/
FAHVector3 addFacetWithDirection(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh, FAHVector3 direction);


/**
 * adds a facet to the STL with the veriicies lised in the order given. Uses implied normals
 **/
void addFacetToSTL(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh);

/**
 * writes a mesh to the file name
 **/
bool stlToFile(STLMesh* m, QString file);


#endif // STLGENERATION_H
