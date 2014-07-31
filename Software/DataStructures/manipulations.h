#ifndef MANIPULATIONS_H
#define MANIPULATIONS_H

#include "../libraries/shared/fah-constants.h"
#include <QDomNode>
#include <QUuid>



enum ManipulationType {Ray_cut_out,
                       met_cut_out,
                       U_Pad,Heal_pad,
                       Heal_spur_hole,
                       Met_Pad,Met_Bar_Pad,
                       Dancer_Pad,
                       Mortons_Neroma_Pad,
                       Scaphoid_Pad,
                       custom
                      };



struct Manipulation {
    ManipulationType type;
    FAHLoopInXYPlane* outerloop;
    QList<FAHLoopInXYPlane*> innerloops;
    float depth;
    float stiffness;
    FAHVector3 location;


    explicit Manipulation();

    explicit Manipulation(QDomNode node);

    QDomNode toNode();
};


FAHLoopInXYPlane* loopFromNode(QDomNode node);

QDomNode nodeFromLoop(FAHLoopInXYPlane* loop);

#endif // MANIPULATIONS_H
