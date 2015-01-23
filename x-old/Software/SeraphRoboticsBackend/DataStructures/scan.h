#ifndef SCAN_H
#define SCAN_H

#include <QObject>
#include "xygrid.h"
#include <QUuid>

/**
  This function will store an XYGrid of the initial raw data. That will be unedited and will be stored for the life of the object and written to the file
  when loaded if the load contains an initiall scan the XYGrid will be uneditable. A second processed scan unit will be given out. This will allow us to restore to the itnial scan at any point.
**/

class Scan : public QObject
{
    Q_OBJECT
public:
    explicit Scan(QObject *parent = 0);
    Scan(QString filename);

    QString getID();
    XYGrid<float>* getXYGrid();

signals:

public slots:
    void reset();
    void writeToDisk();
    void setInitialData(XYGrid<float>* grid); // makes a copy of the data
    void setProcessedGrid(XYGrid<float>* grid);

private:
    QUuid id_;
    QString filename_;
    XYGrid<float>* raw_;
    XYGrid<float>* processed_;
};

#endif // SCAN_H
