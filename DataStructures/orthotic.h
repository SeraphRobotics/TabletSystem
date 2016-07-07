#ifndef ORTHOTIC_H
#define ORTHOTIC_H

#include <QObject>
#include <QUuid>
#include "manipulations.h"
#include <QStack>
#include "scan.h"
#include "printingstructs.h"


struct manipulationpair{
    float stiffness;
    float z_height;
    float x_center;
    float y_center;
    STLMesh* mesh;
    QString id;
};

struct printjobinputs{
    manipulationpair shellpair;
    QList<manipulationpair> manipulationpairs;
};

Q_DECLARE_METATYPE(printjobinputs)




class Orthotic : public QObject
{
    Q_OBJECT


public:
    enum foot_type{kRight,kLeft,kNull};
    enum bottom_type{kFlat,kPlatform,kCurved};
    enum heal_type{kShallow,kDeep};

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
    heal_type getHealType();
    QVector< FAHVector3 > getHealPoints();
    QVector< FAHVector3 > getForePoints();
    Top_Coat getTopCoat();
    QVector<Manipulation*> getManipulations();

signals:
    void manipulated();
    void needScan(QString s);

public slots:
    void addManipulation(Manipulation *m);
    void setTopCoat(Top_Coat tc);
    void undo();
    void redo();
    void writeToDisk(); //writes XML and makes Scan write to disk
    void setScan(Scan* scan);
    void setPosting(Posting p);
    void setBoundary(FAHLoopInXYPlane* loop);
    void setFootType(foot_type t);
    void setBottomType(bottom_type b);
    void setHealType(heal_type h);
    void setBorderPoints(QVector< FAHVector3 > healPts, QVector< FAHVector3 > forePts);


private slots:
    void requestScanData();

public:
    printjobinputs printjob;
    XYGrid<float>* topcoatgrid;
    XYGrid<float>* shellgrid;

private:
    QUuid id_;
    QString filename_;
    QString scanid_;
    Scan* scan_;
    FAHLoopInXYPlane* boundaryloop_;
    Posting forfoot_;
    Posting rearfoot_;
    QVector<Manipulation*> manipulations_;
    QStack<Manipulation*> undo_stack_;
    foot_type foot_;
    bottom_type bottom_;
    heal_type heal_;
    QVector< FAHVector3 > healPts_;
    QVector< FAHVector3 > forePts_;
    Top_Coat tc_;

};

#endif // ORTHOTIC_H
