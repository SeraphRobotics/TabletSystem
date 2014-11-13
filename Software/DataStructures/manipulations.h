#ifndef MANIPULATIONS_H
#define MANIPULATIONS_H

#include "../libraries/shared/fah-constants.h"
#include <QDomNode>
#include <QUuid>
#include <QMetaType>






struct Manipulation {
    enum ManipulationType {Ray_cut_out,
                           met_cut_out,
                           U_Pad,Heal_pad,
                           Heal_spur_hole,
                           Met_Pad,Met_Bar_Pad,
                           Dancer_Pad,
                           Mortons_Neroma_Pad,
                           Scaphoid_Pad,
                           kCustom
                          };
    ManipulationType type;
    FAHLoopInXYPlane* outerloop;
    QList<FAHLoopInXYPlane*> innerloops;
    float depth;
    float thickness;
    float stiffness;
    FAHVector3 location;


    explicit Manipulation();

    explicit Manipulation(QDomNode node);

    QDomNode toNode();
};

Q_DECLARE_METATYPE(Manipulation)

struct Top_Coat{
    enum Style{ kNone,kAuto,kCloth};
    enum Density {kLow,kMedium,kHigh};
    Style style;
    float thickness;
    float depth;
    Density density;
};
Q_DECLARE_METATYPE(Top_Coat)

QDomNode nodeFromTopCoat(Top_Coat tc);
Top_Coat topCoatFromNode(QDomNode node);


/**
  Posting is the data structure for the basic orthotic angular modifications. Each orthotic has two postings
**/
struct Posting{
    enum side{kForFoot,kRearFoot};
    enum direction {kVargus,kValgus};
    Posting();
    float angle; //angle
    direction varus_valgus;
    side for_rear;
    float verticle; //mm
};

QDomNode postingToNode(Posting p);

Posting nodeToPosting(QDomNode node);


QVector< FAHVector3 > pointsFromNode(QDomNodeList nodes);
QVector< QDomNode>  nodeListFromVector( QVector <FAHVector3> points);

FAHVector3 pointFromNode(QDomNode node);
QDomNode nodeFromPoint(FAHVector3 v);

FAHLoopInXYPlane* loopFromNode(QDomNode node);

QDomNode nodeFromLoop(FAHLoopInXYPlane* loop);

#endif // MANIPULATIONS_H
