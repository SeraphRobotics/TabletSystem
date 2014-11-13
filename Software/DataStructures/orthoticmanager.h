#ifndef ORTHOTICMANAGER_H
#define ORTHOTICMANAGER_H

#include <QObject>
#include "scanmanager.h"
#include "orthotic.h"

class OrthoticManager : public QObject
{
    Q_OBJECT
public:
    explicit OrthoticManager(ScanManager* sm, QObject *parent = 0);

    Orthotic* getOrthotic(QString id);
    Orthotic* newOrthotic(QString scan_id);
    bool hasOrthotic(QString id);
    QStringList getList();


private slots:
    void updateList();

public:
    ScanManager* sm_;

private:
    QDir dir_;
    QStringList idlist_;
    QString extension_;

};

#endif // ORTHOTICMANAGER_H
