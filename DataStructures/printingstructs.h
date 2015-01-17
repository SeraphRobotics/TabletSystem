#ifndef PRINTINGSTRUCTS_H
#define PRINTINGSTRUCTS_H
#include <QString>
#include "libraries/shared/stl/stlfile.h"
#include "DataStructures/orthotic.h"
#include <QMetaType>

struct meshpair{
    QString stlfilename;
    QString inifilename;
    QString gcodefile;
    bool isValved;
};


#endif // PRINTINGSTRUCTS_H
