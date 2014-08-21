#ifndef SCANMANAGER_H
#define SCANMANAGER_H

#include <QObject>
#include <QDir>
#include "scan.h"

class ScanManager : public QObject
{
    Q_OBJECT
public:
    explicit ScanManager(QObject *parent = 0);

    Scan* getScan(QString id);
    bool hasScan(QString id);
    QStringList scanIds();

public slots:
    void addScan(Scan* s);


private slots:
    void updateScanList();

private:
    QDir dir_;
    QStringList idlist_;
    QString extension_;
};

#endif // SCANMANAGER_H
