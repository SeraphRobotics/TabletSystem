#ifndef REPAIRCONTROLLER_H
#define REPAIRCONTROLLER_H

#include <QObject>
#include "libraries/shared/stl/stlfile.h"

class RepairController : public QObject
{
    Q_OBJECT
public:
    explicit RepairController(STLMesh* mesh,QString name, QObject *parent = 0);

signals:
    void Success();
    void Failed(QString s);

public slots:
    void repairMesh();

private:
    STLMesh* mesh_;
    QString name_;
};

#endif // REPAIRCONTROLLER_H
