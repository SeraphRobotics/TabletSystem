#ifndef UI_STRUCTS_H
#define UI_STRUCTS_H
#include <QString>
#include <QPixmap>
#include <QDateTime>
#include "libraries/shared/stl/stlmesh.h"
#include "basicstructures.h"
#include <QMetaType>

//struct Name{
//    QString title;
//    QString first;
//    QString last;
//    explicit Name();
//    explicit Name(QString t,QString f, QString l);
//    QString toQString();
//};


struct UI_User{
    QString id;
    Name name;
    QPixmap icon;
};
Q_DECLARE_METATYPE(UI_User)


struct UI_USB_Item{
    enum Type{kScan,kRx};
    Type type;
    QDateTime datetime;
    QString id;
    QString comment;
    Name patient;//if none, then not assigned, if exists, thenstatus is "[Name]'s orthotic is ready for transfer to printer"
};

Q_DECLARE_METATYPE(UI_USB_Item)

struct UI_Patient{
    QString id;
    QString doctorid;
    Name name;
    QList< UI_USB_Item > item_list;
};

Q_DECLARE_METATYPE(UI_Patient)

enum Foot_Type{kRight,kLeft,kBoth};
Q_DECLARE_METATYPE(Foot_Type)


struct Front_Edge{
    QPointF p1;
    QPointF p2;
    QPointF p3;
    QPointF p4;
};
Q_DECLARE_METATYPE(Front_Edge)



typedef QVector < QPointF > Border;


struct UI_Shell_Modification{
    QString name;
    Border outer_border;  // This is a closed loop where point order matters
    QList < Border > inner_borders;
    float height;
    float depth;
    float stiffness;

};
Q_DECLARE_METATYPE(UI_Shell_Modification)

//struct Posting{
//    enum side{kForFoot,kRearFoot};
//    enum direction {kVargus,kValgus};

//    float angle; //angle
//    direction varus_valgus;
//    side for_rear;
//    float verticle; //mm
//};


struct View_3D_Item{
    QColor color;
    STLMesh mesh;
};

#endif // UI_STRUCTS_H
