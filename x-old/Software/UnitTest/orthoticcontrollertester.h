#ifndef ORTHOTICCONTROLLERTESTER_H
#define ORTHOTICCONTROLLERTESTER_H

#include <QObject>
#include "manipulations.h"
#include "scan.h"
#include "View/UI_structs.h"

class OrthoticControllerTester : public QObject
{
    Q_OBJECT
public:
    explicit OrthoticControllerTester(QObject *parent = 0);

public slots:

    void scanImageGenerated(QImage img);

    void boundaryLoopUpdated(FAHLoopInXYPlane* loop);

    void borderGenerated(Border b);

    void frontPoints(QList<QPointF> ps);

    void healPoints(QList<QPointF> ps);

    void stlsGenerated(QList<View_3D_Item> items);

    void topCoat(Top_Coat tc);
};

#endif // ORTHOTICCONTROLLERTESTER_H
