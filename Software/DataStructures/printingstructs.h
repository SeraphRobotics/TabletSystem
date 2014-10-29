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


struct printjobinputs{
    STLMesh* shell;
    QMap<Manipulation, STLMesh*> manipulations;
};

Q_DECLARE_METATYPE(printjobinputs)
#endif // PRINTINGSTRUCTS_H
