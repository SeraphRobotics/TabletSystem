#ifndef XYGRID_H
#define XYGRID_H
#include <QtCore>
#include "../libraries/shared/fah-constants.h"

template <class T>
class XYGrid {
public:
    XYGrid();
    // Constructs a null grid
    XYGrid(const XYGrid<T>* grid);
    //Copy constructor
    XYGrid(int x, int y);
    //Creates a grid of size x,y of zeros
    XYGrid(int x, int y, float stepsizex, float stepsizey) ;
    //Creates a grid of size x,y of zeros and sets stepsize
    XYGrid(float Lx, float Ly, float stepsizex, float stepsizey);
    //Creats a grid with the ceiling of Lx/stepsize, Ly/stepsize and sets stepsize
    XYGrid(FAHVector3 min, FAHVector3 Max ,float stepsizex, float stepsizey) ;
    //Creates a grid of delta of the points in the X and Y dimensions
    XYGrid(FAHVector3 Delta,float stepsizex, float stepsizey) ;
    //Creates a grid of delta of the points in the X and Y dimensions
    XYGrid(QVector<T> v, int rowSize);
    //Creates a grid from a Qvector and sets the rowSize
    XYGrid(QVector<T> v, int rowSize, float stepsize_x, float stepsize_y) ;
    //Overloaded constructor
//    XYGrid(QByteArray barray,int rowSize,float stepsize) ;
    //Loads form QByteArray
    XYGrid(QString csv) ;
    //Loads from CSV file


    /// Save Data
    QString toCSV();
    QByteArray toQByteArray() ;


    /// Data Set and return
    void setDim(int x, int y) ;
    //Resets the dimensions of the array
    void setStepSizes(float x, float y) ;
    //Sets the stepzie
    float stepSizeX() ;
    //Sets the stepzie
    float stepSizeY() ;
    //Returns stepsize
    bool isNull() ;
    //Returns true if Data is empty
    FAHVector3 getDimensions();
    //Returns a point of the dx, dy, 0
    float xDimension() ;
    //Returns x length
    float yDimension() ;
    //Returns y length

    int nx();
    int ny();
    void scale(float s);


    /// Data Access
    QVector<T> asVector();
    //Returns a copy of the data vector
    const T at(int I, int j);
    //Returns an integer of the value at I,j
    T& operator()(int I,int j);
    //Returns the interger at I,j


    QList<T> getValueRange();
    //Returns a list of the values in the slice
    XYGrid<T> getValueGrid( T i);
    //Retruns a grid with only the points at the value. Null grid if value not in grid


private:
    QVector<T> data_;
    float stepsize_x_ ;
    float stepsize_y_ ;
    int nx_; //  (number of X)
    int ny_; // (number of Y)
};


FAHVector3 vectorFromIJ(int i, int j, float val, float stepsize_x, float stepsize_y);

FAHVector3 vectorFromIJScales(int i, int j, float val, float stepsize, float scaleX, float scaleY);

template <class T>
void thresholdGrid(XYGrid<T>* grid,double threshold);

template <class T>
void scaleAndOffset(XYGrid<T>* grid, double scale, double offset);

template <class T>
void blurGrid(XYGrid<T>* grid, int ntimes);

template <class T>
XYGrid<T>* increaseResolution(XYGrid<T>* grid, int zoom);



#endif // XYGRID_H
