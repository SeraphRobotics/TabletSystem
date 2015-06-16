#ifndef TOPCOATCONTROLLER_H
#define TOPCOATCONTROLLER_H

#include <QObject>
#include "DataStructures/orthotic.h"

class TopCoatController : public QObject
{
    Q_OBJECT
public:
    explicit TopCoatController(Orthotic* orth, QString dir, FAHMatrix4x4 transform, QObject *parent = 0);

signals:
    void Failed(QString s);
    void generatedCoatingFile(QString file);

public slots:
     void generateTopCoat();

private:
     Orthotic* orth_;
     QString dir_;
     FAHMatrix4x4 transform_;
};

#endif // TOPCOATCONTROLLER_H
