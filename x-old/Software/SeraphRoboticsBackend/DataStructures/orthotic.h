#ifndef ORTHOTIC_H
#define ORTHOTIC_H

#include <QObject>
#include <QUuid>
#include "manipulations.h"
#include <QStack>
#include "scan.h"

/**
  Posting is the data structure for the basic orthotic angular modifications. Each orthotic has two postings
**/
struct Posting{
    enum side{kForFoot,kRearFoot};
    enum direction {kVargus,kValgus};

    float angle; //angle
    direction varus_valgus;
    side for_rear;
    float verticle; //mm
};

QDomNode postingToNode(Posting p);

Posting nodeToPosting(QDomNode node);


class Orthotic : public QObject
{
    Q_OBJECT


public:
    enum foot_type{kRight,kLeft,kNull};
    enum bottom_type{kFlat,kPlatform,kCurved};

    explicit Orthotic(QObject *parent = 0);
    Orthotic(QString filename);

    QString getId();

    Scan* getScan();
    QString getScanID();
    Posting getForFootPosting();
    Posting getRearFootPosting();
    FAHLoopInXYPlane* getLoop();
    foot_type getFootType();
    bottom_type getBottomType();

signals:
    void manipulated();
    void needScan(QString s);

public slots:
    void addManipulation(Manipulation m);
    void undo();
    void redo();
    void writeToDisk(); //writes XML and makes Scan write to disk
    void setScan(Scan* scan);
    void setPosting(Posting p);
    void setBoundary(FAHLoopInXYPlane* loop);
    void setFootType(foot_type t);
    void setBottomType(bottom_type b);

private slots:
    void requestScanData();

private:
    QUuid id_;
    QString filename_;
    QString scanid_;
    Scan* scan_;
    FAHLoopInXYPlane* boundaryloop_;
    Posting forfoot_;
    Posting rearfoot_;
    QVector<Manipulation> manipulations_;
    QStack<Manipulation> undo_stack_;
    foot_type foot_;
    bottom_type bottom_;
};

#endif // ORTHOTIC_H
