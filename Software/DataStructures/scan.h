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

    /**
     * @brief Scan loads file from XML tree with CSV structures
     * @param filename
     */
    Scan(QString filename);

    /**
     * @brief getID returns ID
     * @return
     */
    QString getID();

    /**
     * @brief getXYGrid
     * @return processed XYGrid
     */
    XYGrid<float>* getProcessedXYGrid();

    /**
     * @brief getPostedXYGrid
     * @return posted XYGrid
     */
    XYGrid<float>* getPostedXYGrid();
    /**
     * @brief setId overides the ID generated internally
     * @param id
     */
    void setId(QString id);

signals:

public slots:

    /**
     * @brief resets the processed grid to the raw scan
     */
    void reset();

    /**
     * @brief writeToDisk writes a CSV files inside of XML tree structure
     */
    void writeToDisk();

    /**
     * @brief setInitialData sets the XYGRid
     * @param grid
     */
    void setInitialData(XYGrid<float>* grid); // makes a copy of the data

    /**
     * @brief setProcessedGrid
     * @param grid
     */
    void setProcessedGrid(XYGrid<float>* grid);

    void setPostedGrid(XYGrid<float>* grid);

private:
    QUuid id_;
    QString filename_;
    XYGrid<float>* raw_;
    XYGrid<float>* processed_;
    XYGrid<float>* posted_;
};

//Q_DECLARE_METATYPE(Scan*);
#endif // SCAN_H
