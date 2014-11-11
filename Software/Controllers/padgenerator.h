#ifndef PADGENERATOR_H
#define PADGENERATOR_H

#include "stlgeneration.h"
#include "DataStructures/manipulations.h"

struct ijd{
    int i;
    int j;
    float d;
};

float GeneratePad(Manipulation *m, XYGrid<float>* pad_grid, XYGrid<float>* shell_grid, STLMesh* mesh, STLMesh* shell_mesh, float min_z);

#endif // PADGENERATOR_H
