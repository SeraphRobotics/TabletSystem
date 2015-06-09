#include "xygrid.h"


/*
 *  The file uses a grid of the structure
 *   *-----y----->  row = Y
 *   | 0123
 *   | 4567
 *   x
 *   |
 *  \|/
 *   column = x
 *
 *  its stored as a Vector of length #x*#y where the value i,j is at #n*i+j
 *  it can be any type in the XYGrid but it is designed to be access by lengths as well as indecies
*/



template <class T>
XYGrid<T>::XYGrid():stepsize_x_(1.0),nx_(0),ny_(0)
{
}

/**
 *http://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
 *
**/

// Constructs a null grid
template <class T>
XYGrid<T>::XYGrid(const XYGrid<T> *otherxygrid){
    stepsize_x_ = otherxygrid->stepsize_x_;
    stepsize_y_ = otherxygrid->stepsize_y_;
//    data_ = otherxygrid->data_;
    for(int i=0;i<otherxygrid->data_.size();i++){
        data_.append(otherxygrid->data_.at(i));
    }
    nx_=otherxygrid->nx_;
    ny_=otherxygrid->ny_;
}
//Copy constructor
template <class T>
XYGrid<T>::XYGrid(int x, int y){
    nx_=x;
    ny_=y;
    data_ = QVector<T>(x*y);
    stepsize_x_=1.0;
    stepsize_y_=1.0;
}
//Creates a grid of size x,y of zeros
template <class T>
XYGrid<T>::XYGrid(int x, int y, float stepsizex, float stepsizey){
    nx_=x;
    ny_=y;
    data_ = QVector<T>(x*y);
    stepsize_x_=stepsizex;
    stepsize_y_=stepsizey;
}
//Creates a grid of size x,y of zeros and sets stepsize
template <class T>
XYGrid<T>::XYGrid(float Lx, float Ly, float stepsizex, float stepsizey){
    nx_=(int) Lx/stepsizex+1;
    ny_=(int) Ly/stepsizey+1;
    data_ = QVector<T>(nx_*ny_);
    stepsize_x_=stepsizex;
    stepsize_y_=stepsizey;
}
//Creats a grid with the ceiling of Lx/stepsize, Ly/stepsize and sets stepsize
template <class T>
XYGrid<T>::XYGrid(FAHVector3 min, FAHVector3 max, float stepsizex, float stepsizey) {
    FAHVector3 delta(min.sub(max));
    float x,y;
    x = abs(delta.x);
    y = abs(delta.y);
    nx_=(int) x/stepsizex+1;
    ny_=(int) y/stepsizex+1;
    data_ = QVector<T>(x*y);
    stepsize_x_=stepsizex;
    stepsize_y_=stepsizey;
}
//Creates a grid of delta of the points in the X and Y dimensions
template <class T>
XYGrid<T>::XYGrid(FAHVector3 Delta,float stepsizex, float stepsizey)  {
    float x,y;
    x = abs(Delta.x);
    y = abs(Delta.y);
    nx_=(int) x/stepsizex+1;
    ny_=(int) y/stepsizey+1;
    data_ = QVector<T>(x*y);
    stepsize_x_=stepsizex;
    stepsize_y_=stepsizey;
}
//Creates a grid of delta of the points in the X and Y dimensions
template <class T>
XYGrid<T>::XYGrid(QVector<T> v, int rowSize){
    if(rowSize==0){XYGrid<T>();}
    else{
        ny_ = rowSize;
        nx_ = v.size()/rowSize;
        data_ = v;
        stepsize_x_=1.0;
    }
}
//Creates a grid from a Qvector and sets the rowSize
template <class T>
XYGrid<T>::XYGrid(QVector<T> v, int rowSize, float stepsize){
    data_=v;
    ny_=rowSize;
    nx_ = v.size()/ny_;
    stepsize_x_=stepsize;

}
//Overloaded constructor
//template <class T>
//XYGrid<T>::XYGrid(QByteArray barray,int rowSize,float stepsize) {}
//Loads form QByteArray
template <class T>
XYGrid<T>::XYGrid(QString csv):stepsize_x_(1.0),stepsize_y_(1.0),nx_(0),ny_(0) {

         QString data;
         QStringList lines;
         QStringList vals;
         QTextStream in(&csv);
        //file opened successfully
         while(!in.atEnd()){
             lines.append(in.readLine());
         }

         // the first line contains the stepsize value alone

         data = lines.first();
         lines.pop_front();
         vals = data.split(",");
         if (vals.size()>0){
             stepsize_x_=vals[0].toFloat();
         }

         if (vals.size()>1){
             stepsize_y_=vals[1].toFloat();
         }else{
             stepsize_y_=stepsize_x_;
         }
         nx_ = lines.size();
         ny_ = lines[1].split(",").size();
         data_ = QVector<T>(nx_*ny_);
         for(int row=0;row<nx_;row++){
             vals = lines[row].split(",");
             for(int col=0;col<ny_;col++){
                 data_[(ny_*row+col)]= (T)vals[col].toFloat();
             }
         }



}
//Loads from CSV file


template <class T>
int XYGrid<T>::nx(){return nx_;}

template <class T>
int XYGrid<T>::ny(){return ny_;}

template <class T>
void XYGrid<T>::scale(float s){
    for(int i=0;i<data_.size();i++){
        data_[i]=(T) s*data_[i];
    }
}

/// Save Data
template <class T>
QString XYGrid<T>::toCSV(){
    QString csv;
    QTextStream ss(&csv);
    ss<<stepsize_x_;
    for(int row=0; row<nx_;row++){
        ss<<"\n";
        for(int col=0;col<ny_-1;col++){
            ss<<at(row,col)<<",";
        }
        ss<<at(row,ny_-1);
    }
    return csv;
}



template <class T>
QByteArray XYGrid<T>::toQByteArray() {return QByteArray();}


/// Data Set and return
template <class T>
void XYGrid<T>::setDim(int x, int y){
    QVector<T> newdata(x*y);
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            if((i>nx_) or (j>ny_)){
                newdata[(y*i*j)]=T();
            }
            else{
                newdata[(y*i+j)]=at(i,j);
            }
        }
    }
    ny_=y;
    nx_=x;
    data_=newdata;
}
//Resets the dimensions of the array

template <class T>
void XYGrid<T>::setStepSizes(float x,float y){
    stepsize_x_=x;
    stepsize_y_=y;
}
//Sets the stepzie

template <class T>
float XYGrid<T>::stepSizeX() {return stepsize_x_;}
//Returns stepsize

template <class T>
float XYGrid<T>::stepSizeY() {return stepsize_y_;}
//Returns stepsize

template <class T>
bool XYGrid<T>::isNull() {return data_.isEmpty();}
//Returns true if Data is empty

template <class T>
FAHVector3 XYGrid<T>::getDimensions(){
    return FAHVector3(xDimension(),yDimension(),0);
}
//Returns a point of the dx, dy, 0

template <class T>
float XYGrid<T>::xDimension() {return nx_*stepsize_x_;}
//Returns x length

template <class T>
float XYGrid<T>::yDimension() {return ny_*stepsize_y_;}
//Returns y length



/// Data Access
template <class T>
QVector<T> XYGrid<T>::asVector(){
    return QVector<T>(data_);
}
//Returns a copy of the data vector
template <class T>
const T XYGrid<T>::at(int I, int j){
    return T(data_[(ny_*I+j)]);
}
//Returns an integer of the value at I,j

template <class T>
T& XYGrid<T>::operator()(int I,int j){
    return data_[(ny_*I+j)];
}
//Returns the interger at I,j


template <class T>
QList<T> XYGrid<T>::getValueRange(){
    QList<T> values;
    for(int i=0;i<data_.size();i++){
        if(!values.contains(data_.at(i))){values.append(data_[i]);}
    }
    return values;
}
//Returns a list of the values in the slice

template <class T>
XYGrid<T> XYGrid<T>::getValueGrid( T v){
    QVector<T> newdata(data_);
    for(int i=0;i<data_.size();i++){
        if(newdata.at(i)!=v){newdata[i]=T();}
    }
    return XYGrid(newdata,ny_,stepsize_x_);
}
//Retruns a grid with only the points at the value. Null grid if value not in grid


template <class T>
void thresholdGrid(XYGrid<T>* grid,double threshold){
    double val;

    /// scale grid
    for(int i=0;i<grid->nx();i++){
        for(int j=0;j<grid->ny();j++){
            val = grid->at(i,j);
            if (val>threshold){val=threshold;}
            grid->operator ()(i,j)=(T) val;
        }
    }
}


template <class T>
void scaleAndOffset(XYGrid<T>* grid, double scale, double offset){
    double val;
    /// scale grid
    for(int i=0;i<grid->nx();i++){
        for(int j=0;j<grid->ny();j++){
            val = grid->at(i,j);
            val = scale*(val+offset);
            grid->operator ()(i,j)=(T) val;
        }
    }
}


template <class T>
void blurGrid(XYGrid<T>* grid,int ntimes){
    /** calculated average of the 4 points around it
     *     p1
     *     |
     * p2--p0--p3
     *     |
     *     p4
     *
     *  p0 = sum(p1,p2,p3,p4,p0)/5
     **/
    // make a copy
    for(int blurs=0;blurs<ntimes;blurs++){
        XYGrid<T> copy(grid->asVector(),grid->ny(),grid->stepSizeX());
        double p0,p1,p2,p3,p4;
        for(int i=1;i<grid->nx()-1;i++){
            for(int j=1;j<grid->ny()-1;j++){
                p0=copy.at(i,j);
                p1=copy.at(i-1,j);
                p2=copy.at(i,j-1);
                p3=copy.at(i,j+1);
                p4=copy.at(i+1,j);
                grid->operator ()(i,j)=(p0+p1+p2+p3+p4)/5.0;
            }
        }
    }
}



template <class T>
XYGrid<T>* increaseResolution(XYGrid<T>* grid, int zoom){
    XYGrid<T>* newgrid = new XYGrid<T>((grid->nx()-1)*zoom,
                                       (grid->ny()-1)*zoom,
                                       grid->stepSizeX()*1.0/zoom,
                                       grid->stepSizeY()*1.0/zoom);


    for(int i=0;i<grid->nx()-1;i++){
        // get this layers bounds
        for(int j=0;j<grid->ny()-1;j++){
            /** calculated vectors of the 4 points and determin case
             *  p1--p2 -->j y
             *  | \ |
             *  p3--p4
             *  |
             * \|/
             *  i
             *  x
             *
             *
             **/
            FAHVector3 p1,p2,p3,p4;

            p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSizeX(),grid->stepSizeY());
            p3=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSizeX(),grid->stepSizeY());
            p2=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSizeX(),grid->stepSizeY());
            p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSizeX(),grid->stepSizeY());
            FAHTriangle t1(p1,p2,p4);
            FAHTriangle t2(p1,p3,p4);

            FAHVector3 O;
            FAHVector3 R;
            O.zero();
            R=R.zero();
            R[2]=1;
            float h;
            float x,y;
            for( int k=0;k<zoom;k++){
                for(int l=0;l<zoom;l++){
                    h=0;
                    x=1.0/zoom*k;
                    y=1.0/zoom*l;
                    O[0]=p1[0]+x;
                    O[1]=p1[1]+y;
                    O[2]=0;
                    if (t1.intersectedByRay(O,R)){
                        h=t1.heightAt(O,R);
                    } else if(t2.intersectedByRay(O,R)){
                        h=t2.heightAt(O,R);
                    }else{
                        printf("\n no intersection x:%f\ty:%f",x,y);
                    }
                    newgrid->operator ()(i*zoom+k,j*zoom+l)=h;
                }
            }
            newgrid->operator ()(i*zoom,j*zoom)=grid->at(i,j);

        }
    }
    return newgrid;
}


FAHVector3 vectorFromIJScales(int i, int j, float val, float stepsize, float scaleX, float scaleY){
    float x=i*stepsize*scaleX;
    float y=j*stepsize*scaleY;
    return FAHVector3(x,y,val);
}

FAHVector3 vectorFromIJ(int i, int j, float val, float stepsize_x, float stepsize_y){
    float x=i*stepsize_x;
    float y=j*stepsize_y;
    return FAHVector3(x,y,val);
}

template class XYGrid<float>;
template class XYGrid<int>;
template void thresholdGrid(XYGrid<float>* grid,double threshold);
template void thresholdGrid(XYGrid<int>* grid,double threshold);
template void scaleAndOffset(XYGrid<float>* grid, double scale, double offset);
template void scaleAndOffset(XYGrid<int>* grid, double scale, double offset);
template void blurGrid(XYGrid<float>* grid,int ntimes);
template void blurGrid(XYGrid<int>* grid,int ntimes);
template XYGrid<float>* increaseResolution(XYGrid<float>* grid, int zoom);
template XYGrid<int>* increaseResolution(XYGrid<int>* grid, int zoom);
