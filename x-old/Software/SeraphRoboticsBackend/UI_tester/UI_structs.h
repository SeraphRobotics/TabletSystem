#ifndef UI_STRUCTS_H
#define UI_STRUCTS_H
#include <QString>
#include <QPixmap>
#include <QDateTime>

#include "shared/stl/STLMesh.h"
#include "basicstructures.h"
#include "opencv2/imgproc/imgproc.hpp"


struct UI_User{
    QString id;
    Name name;
    QPixmap icon;
};



struct UI_USB_Item{

    enum Type{kScan,kRx};

    Type type;
    QDateTime datetime;
    QString id;
    QString comment;
    Name patient;//if none, then not assigned, if exists, thenstatus is "[Name]'s orthotic is ready for transfer to printer"
};

struct UI_Patient{
    QString id;
    QString doctorid;
    Name name;
    QList< UI_USB_Item > item_list;
};


enum Foot_Type{kRight,kLeft,kBoth};



struct Front_Edge{
    QPointF p1;
    QPointF p2;
    QPointF p3;
    QPointF p4;
};



struct Top_Coat{
    enum Style{ kNone,kAuto,kCloth};
    enum Density {kLow,kMedium,kHigh};
    Style style;
    float thickness;
    float depth;
    Density density;
};


typedef QVector < QPointF > Border;



struct UI_Shell_Modification{
    QString name;
    Border outer_border;  // This is a closed loop where point order matters
    QList< std::vector <cv::Point> > inner_borders;
    double height;
    double depth;
    double stiffness;

};

struct View_3D_Item{
    QColor color;
    STLMesh mesh;
};

#endif // UI_STRUCTS_H
