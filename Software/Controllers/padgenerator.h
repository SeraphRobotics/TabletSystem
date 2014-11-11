#ifndef PADGENERATOR_H
#define PADGENERATOR_H

#include "stlgeneration.h"
#include "DataStructures/manipulations.h"

struct ijd{
    int i;
    int j;
    float d;
};

STLMesh* GeneratePad(Manipulation m,XYGrid<float>* pad_grid, XYGrid<float>* shell_grid,STLMesh* shell_mesh);

#endif // PADGENERATOR_H
