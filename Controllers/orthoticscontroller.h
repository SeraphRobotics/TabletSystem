#ifndef ORTHOTICSCONTROLLER_H
#define ORTHOTICSCONTROLLER_H

#include <QObject>
#include "DataStructures/manipulations.h"
#include "DataStructures/scan.h"
#include "View/UI_structs.h"
#include "DataStructures/orthotic.h"
#include "DataStructures/orthoticmanager.h"
#include "DataStructures/printingstructs.h"

class OrthoticController : public QObject
{
    Q_OBJECT
public:
    /**
     * This Class acts as a middle layer between the UI and the data structures for the orthotics
     * It ensures that the UI get the data in the proper format and assures that the Rx is done
     * It is responsible for a single foot's data and processing
     **/
    explicit OrthoticController(OrthoticManager* om, QObject *parent = 0);

signals:

    /**
     * @brief scanImageGenerated
     * @param img
     *
     *  when an orthotic is loaded or made new this signal will emit
     *  the image of the Orthotics PROCESSED Scan Image
     */
    void scanImageGenerated(QImage img);

    /**
     * @brief boundaryLoopUpdated
     * @param loop
     *
     * When the boarder points are set, this loop is calculated and emited here
     * When an orthotic is loaded by ID its boarder will be emited here
     */
    void boundaryLoopUpdated(FAHLoopInXYPlane* loop);

    /**
     * @brief borderGenerated
     * @param b
     *
     * When the boarder points are set, this loop is calculated and emited here
     * When an orthotic is loaded by ID its boarder will be emited here
     */
    void borderGenerated(Border b);

    /**
     * @brief frontPoints
     * @param ps
     *
     * when a n orthotic is loaded by ID, its frontpoints will be emitted
     */
    void frontPoints(QList<QPointF> ps);

    /**
     * @brief healPoints
     * @param ps
     *
     * when an orhtotic is loaded by ID its frontpoints will be emitted here
     */
    void healPoints(QList<QPointF> ps);

    /**
     * @brief stlsGenerated
     * @param items
     *
     * when a boarder is calculated, pads are set, and posting is set
     * the system will make the STL files and emit them here with their color
     */
    void stlsGenerated(QList<View_3D_Item> items);

//    void printJobInputs(printjobinputs p);

    void topCoat(Top_Coat tc);

public:
    Orthotic* getOrthotic();

public slots:

    void setScan(QString scan_id);
    void setOrthotic(QString orthotic_id);


    void setBorderPoints(QVector< FAHVector3 > healPts, QVector< FAHVector3 > forePts);

    void setTopCoat(Top_Coat tc);

    void setBottomType(Orthotic::bottom_type type);

    void addManipulation(Manipulation* m);

    void setPosting(Posting p);

    void setHealType(Orthotic::heal_type t);

    void processFromRaw();
    void processPosting();
    void normalizeByBoundary();
    void processBoundary();
    void save();
    void offset(float mm);
    void makeSTLs();

private:

    Border borderFromLoop(FAHLoopInXYPlane* loop);
    QList<QPointF> qpointfListFromFAHVector3(QVector<FAHVector3> points);

private:
    OrthoticManager* om_;
    Orthotic* orth_;

};

#endif // ORTHOTICSCONTROLLER_H
