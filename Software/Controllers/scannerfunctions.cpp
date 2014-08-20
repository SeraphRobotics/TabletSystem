#include "scannerfunctions.h"
#include <stdio.h>
#include "../libraries/shared/math/float.h"
#include <QDomDocument>
#include <math.h>
#include "../libraries/shared/stl/stlfile.h"


//bool onLoops(FAHVector3 pt, const FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops){
//    bool onouter=false;
//    bool oninner=false;
//    foreach(FAHLoopInXYPlane* loop,innerLoops){
//        oninner=loop->pointOnLoop(pt);
//        if (oninner){
//            return oninner;
//        }
//    }

//    onouter = OuterLoop->pointOnLoop(pt);
//    return onouter;
//}

bool loopsContain(FAHVector3 pt, const FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops){
    bool containedByOuter=true;
    bool notContainedByInners=true;
    foreach(FAHLoopInXYPlane* loop,innerLoops){
        notContainedByInners = notContainedByInners && !loop->pointInside(pt);
    }

    containedByOuter = OuterLoop->pointInside(pt);
    return (containedByOuter&&notContainedByInners);
}

//void printPoint(FAHVector3 pt){
//    printf("\n\t%.2f\t%.2f\t%.2f",pt.x,pt.y,pt.z);
//}

//void writeLoopToXDFL(const FAHLoopInXYPlane& loop, QString file){
//     QDomDocument document;
//     QDomElement root = document.createElement("xdfl");
//     document.appendChild(root);

//     /// ADD palette
//     QDomElement palette = document.createElement("palette");
//     root.appendChild(palette);
//     QDomElement name = document.createElement("name");
//     name.appendChild(document.createTextNode("test"));
//     palette.appendChild(name);
//     QDomElement id = document.createElement("id");
//     id.appendChild(document.createTextNode("1"));
//     palette.appendChild(id);

//     QDomElement cmds = document.createElement("commands");
//     QDomElement path = document.createElement("path");
//     QDomElement matId = document.createElement("materialid");
//     matId.appendChild(document.createTextNode("1"));
//     path.appendChild(matId);
//     for(int i=0;i<loop.points.size();i++){
//         FAHVector3 pt = loop.points.at(i);

//         QDomElement point = document.createElement("point");
//         QDomElement xEl = document.createElement("x");
//         xEl.appendChild(document.createTextNode(QString::number(pt.x)));
//         QDomElement yEl = document.createElement("y");
//         yEl.appendChild(document.createTextNode(QString::number(pt.y)));
//         QDomElement zEl = document.createElement("z");
//         zEl.appendChild(document.createTextNode(QString::number(pt.z)));

//         point.appendChild(xEl);
//         point.appendChild(yEl);
//         point.appendChild(zEl);
//         path.appendChild(point);

//     }
//     FAHVector3 pt = loop.points.at(0);

//     QDomElement point = document.createElement("point");
//     QDomElement xEl = document.createElement("x");
//     xEl.appendChild(document.createTextNode(QString::number(pt.x)));
//     QDomElement yEl = document.createElement("y");
//     yEl.appendChild(document.createTextNode(QString::number(pt.y)));
//     QDomElement zEl = document.createElement("z");
//     zEl.appendChild(document.createTextNode(QString::number(pt.z)));

//     point.appendChild(xEl);
//     point.appendChild(yEl);
//     point.appendChild(zEl);
//     path.appendChild(point);
//     cmds.appendChild(path);
//     root.appendChild(cmds);



//     QFile f(file);
//     if (f.open(QFile::WriteOnly)) {
//         QTextStream out(&f);
//         out<<document.toString();
//         f.close();
//     }

//}

void removePointsIfInList(QVector<FAHVector3>* vector,QVector<FAHVector3>* newvector, QList<FAHVector3> points){
    QVector<FAHVector3> copy(vector->size());

    for(int i=0;i<vector->size();i++){
        copy[i]=vector->at(i).copy();
    }

    QMutableVectorIterator<FAHVector3> iter(copy);
    while(iter.hasNext()){
        FAHVector3 pt = iter.next();
        for(int i=0;i<points.size();i++){
            FAHVector3 point = points[i];
            if(point.equals(pt)){
                iter.remove();
                i=points.size();
//                printf("\n point removed");
            }
        }
    }
    newvector->clear();
    for(int i=0;i<copy.size();i++){
        newvector->append(copy[i]);
    }
}


void removedDuplicatePoints(QVector<FAHVector3>* vector){
    QList<int> points_to_remove;
    int vectorsize= vector->size();
    QVector<FAHVector3> copy(vectorsize);
    for(int i=0;i<vectorsize;i++){
        FAHVector3 pt = vector->operator [](i);
        copy[i]=pt;
        for(int j=0;j<vectorsize;j++){
            if(i!=j){
                if((vector->at(j).equals(vector->at(i)))
                        &&!points_to_remove.contains(i)
                        &&!points_to_remove.contains(j)){
                    points_to_remove.append(i);
//                    printf("\nDupplicate\t%.2f\t%.2f\t%.2f",pt.x,pt.y,pt.z);
                    copy[i].zero();
                }
            }
        }
    }

    vector->clear();
    QVectorIterator<FAHVector3> iter(copy);
    while(iter.hasNext()){
        FAHVector3 pt = iter.next();
        if(!pt.isInvalid()){
            vector->append(pt);
        }
    }


}


float ptOnLine(FAHLine line, FAHVector3 pt){
    float x = pt.x-line.b.x;
    float y = pt.y-line.b.y;
//    float r=sqrt(x*x+y*y);
    float dx = line.a.x-line.b.x;
    float dy = line.a.y-line.b.y;
    float dz = line.a.z-line.b.z;
    float sx = 0;
    float sy = 0;
    if(dx>0){sx=dz/dx;}
    if(dy>0){sy=dz/dy;}

    float z = line.b.z + sx*x + sy*y;
    return z;
}

QVector<FAHVector3> intersection_points(QList<FAHLine> lines, const FAHLoopInXYPlane *outerloop, QList<FAHLoopInXYPlane *> loops){
    QVector<FAHVector3> points;
    
    foreach(FAHLine line,lines){
        FAHVector3 pt;
        outerloop->intersect(line,&pt);
        if(!pt.isInvalid()){
//            pt.z = ptOnLine(line, pt);
//            qDebug()<<"z: "<<pt.z;
            points.append(pt.copy());
        }
    }
    
    for(int k=0;k<loops.size();k++){
        FAHLoopInXYPlane* loop = loops.at(k);
        foreach(FAHLine line,lines){
            FAHVector3 pt;
            loop->intersect(line,&pt);
            if(!pt.isInvalid()){
//                pt.z = ptOnLine(line, pt);
                points.append(pt.copy());
            }
        }

    }
    return points;
}


bool getPointsInGrid(int i, int j, float stepsize, FAHLoopInXYPlane *outerloop , QList<FAHLoopInXYPlane *> innerLoops, QVector<FAHVector3>* pointsInGrid){
    bool pointsFromOuterLoop=false;

    FAHVector3 p1,p2,p3,p4;
    p1=vectorFromIJ(i,j,0,stepsize);
    p2=vectorFromIJ(i,j+1,0,stepsize);
    p3=vectorFromIJ(i+1,j,0,stepsize);
    p4=vectorFromIJ(i+1,j+1,0,stepsize);

    FAHLine l12(p1,p2);
    FAHLine l13(p1,p3);
    FAHLine l24(p2,p4);
    FAHLine l34(p3,p4);
    QList<FAHLine> lines;
    lines.append(l12);
    lines.append(l13);
    lines.append(l24);
    lines.append(l34);







    // get intersections along grid
    QVector<FAHVector3> edgepoints;
    bool inloop = false;
    int numLoops = 0;


    foreach(FAHLine line,lines){
        FAHVector3 pt;
        outerloop->intersect(line,&pt);
        if(!pt.isInvalid()){
            pointsFromOuterLoop = true;
            edgepoints.append(pt.copy());
            if(!inloop){
                inloop=true;
                numLoops++;
            }

        }
    }



    for(int k=0;k<innerLoops.size();k++){
        inloop= false;
        FAHLoopInXYPlane* loop = innerLoops.at(k);
        foreach(FAHLine line,lines){
            FAHVector3 pt;
            loop->intersect(line,&pt);
            if(!pt.isInvalid()){
                pointsFromOuterLoop = false;
                edgepoints.append(pt.copy());
                if(!inloop){
                    inloop=true;
                    numLoops++;
                }

            }
        }

    }

    if(numLoops>1){
        printf("\n %i LOOPS",numLoops);
    }else{


        // NEED A SYSTEM FOR solving if item passes through more than 2 walls
        int size = edgepoints.size();
//        if (size!=2){
//            printf("\n Index %i,%i Found %i intersects",i,j,edgepoints.size());
//            for(int k=0;k<edgepoints.size();k++){
//                printf("\n\tPoint: %.2f\t%.2f\t%.2ff",edgepoints.at(k).x,edgepoints.at(k).y,edgepoints.at(k).z);
//            }
//        }
        if (size>=2){
            QList<FAHLoopInXYPlane*> loops(innerLoops);
            loops.append(outerloop);

            QVector<FAHVector3> innerpoints;
            for(int k=0;k<loops.size();k++){
                FAHLoopInXYPlane* loop = loops.at(k);
                // Collect points between
                for(int i=0;i<loop->points.size();i++){
                    FAHVector3 pt = FAHVector3(loop->points.at(i));
                    if( (pt.x>=p1.x) && (pt.x<=p3.x) &&
                        (pt.y>=p1.y) && (pt.y<=p2.y) ){
                        // inside grid
                        innerpoints.append(pt);
                    }
                }
            }

            /// NEED TO CHECK FOR CUVES BAHAVIOR

            // Merge two lists
            pointsInGrid->append(edgepoints[0]);
            for(int i=0;i<innerpoints.size();i++){
                pointsInGrid->append(innerpoints[i]);

            }
            pointsInGrid->append(edgepoints[1]);
        }
    }
    return pointsFromOuterLoop;
}


template <class T>
float getHeightAt(float x, float y,XYGrid<T>* grid){
    int i=(int) (x/grid->stepSize());
    int j=(int) (y/grid->stepSize());

    int ip = i+1;
    int jp = j+1;

    if((ip>(grid->nx()-1)) || (jp>(grid->ny()-1))){
        return grid->at(i-1,j-1);
    }

    FAHVector3 p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
    FAHVector3 p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
    FAHVector3 p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
    FAHVector3 p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());

    return getHeightAt(x,y,p1,p2,p3,p4);

}


float getHeightAt(float x, float y, FAHVector3 p1, FAHVector3 p2, FAHVector3 p3, FAHVector3 p4){
    FAHTriangle t1(p1,p2,p4);
    FAHTriangle t2(p1,p3,p4);

    FAHVector3 O(x,y,0);
    FAHVector3 R(0,0,1);
    float h=0;
    if (t1.intersectedByRay(O,R)){
        h=t1.heightAt(O,R);
    }else if(t2.intersectedByRay(O,R)){
        h=t2.heightAt(O,R);
    }else{
        printf("\nCouldnt set value");
        h=p1.z;
    }
    return h;
}


/**
    // Need to make 5 point average of the border
    QMap<int, QVector<float> > refinedBorder;

    int reach=2;
    for(int j=0; j<grid->ny();j++){

        QVector<int> temp;
        QVector<float> border(2,0);
        QVector<int> tempBorder(2,0);
        QVector<int> totalNum(2,0);
        QVector<float> sum(2,0.0);

        tempBorder = bordermap[j];

        if((j<reach) || (j>(grid->ny()-1-reach)) ){
            border[0]=tempBorder[0]*grid->stepSize();
            border[1]=tempBorder[1]*grid->stepSize();

        }else{
            for(int l=0;l<2;l++){
                for(int k = -reach;k<reach;k++){// Average over last and next number of reach points
                    temp=bordermap[j+k];
                    if( !((temp[l]==0)||(temp[l]==grid->nx())) ){
                        totalNum[l]++;
                        sum[l]=sum[l]+temp[l]*grid->stepSize();
                    }
                }
                if (totalNum[l]>0){
                    border[l]=sum[l]/totalNum[l];
                }else{
                    border[l]=tempBorder[l];
                }
            }
        }
        refinedBorder[j]=border;
    }
    //// New 5 point average for x coordinate of border.
**/




FAHLoopInXYPlane smoothLoop(FAHLoopInXYPlane* loop, int times, float ylimit){
    FAHLoopInXYPlane newloop(loop);
    int size = loop->points.size();
    float x, y;
//    printf("\nsize:%i",size);
    for(int i=0;i<loop->points.size();i++){
        FAHVector3 pim1;
        if (0==i){
            pim1 = loop->points.last();
        }else{
            pim1 = loop->points.at((i-1)%size);
        }
        FAHVector3 pi = loop->points.at(i%size);
        FAHVector3 pip1 = loop->points.at((i+1)%size);
        FAHVector3 pt;
        x = (pim1.x+pi.x+pip1.x)/3.0;
        y= (pim1.y+pi.y+pip1.y)/3.0;
        if(y>ylimit){
            pt.x = pi.x;
            pt.y = pi.y;
        }else{
            pt.x = x;
            pt.y = y;
        }
        newloop.points[i] = pt;
//        printf("%i ",i);
    }

//    printf("\ntime %i",times);
    times--;

    if(times<=0){return newloop;}
    else{return smoothLoop(&newloop,times,ylimit);}

}

FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid){
    return boundaryLoopFromGrid(grid,0,50,50);
}

FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid, int offsetFromBorder, float slopetrigger, float sensitivity){
    FAHLoopInXYPlane loop;
    QVector<FAHVector3> left, right;
    float max;
    int leftsize=0,rightsize=0;
    bool addedLeft = false, addedRight=false;
//    float sensitivity = 3;


    for(int j=1+offsetFromBorder;j<grid->ny()-1-offsetFromBorder;j++){
        bool leftTrigger = true, rightTrigger = true;
        // find left point
        leftsize = left.size();
        max = grid->at(0,j);
        for(int i=1+offsetFromBorder;i<(grid->nx()-1-offsetFromBorder);i++){
            if (!Math::floatsEqual(max,grid->at(i,j))){
                leftTrigger= true;
//                printf("\nleft triggered");
            }
            if (leftTrigger){
//                printf()
                float backslope = (grid->at(i,j) - grid->at(i-1,j))/grid->stepSize();
                float forslope = (grid->at(i+1,j) - grid->at(i,j))/grid->stepSize();
                float frontslope = (grid->at(i,j+1) - grid->at(i,j))/grid->stepSize();
                float rearslope = (grid->at(i,j) - grid->at(i,j-1))/grid->stepSize();
                float xcurve = (fabs(fabs(forslope)-fabs(backslope)));
                float ycurve = (fabs(fabs(frontslope)-fabs(rearslope)));

                //if( (xcurve<slopetrigger) && fabs(forslope)<sensitivity && fabs(backslope)<sensitivity
                //    && (ycurve<slopetrigger) && fabs(frontslope)<sensitivity && fabs(rearslope)<sensitivity)
                if (grid->at(i,j)>60)
                {
                    left.append(vectorFromIJ(i,j,0,grid->stepSize()));
                    i=grid->nx();
                }
            }
        }
        addedLeft = (left.size()==leftsize);


        // find right point
        rightsize=right.size();
        max = grid->at(grid->nx()-1,j);
        for(int i=(grid->nx()-1-offsetFromBorder); i>1+offsetFromBorder ;i--){
            if (!Math::floatsEqual(max,grid->at(i,j))){
                rightTrigger= true;
            }
            if (rightTrigger){
                float backslope = (grid->at(i,j) - grid->at(i-1,j))/grid->stepSize();
                float forslope = (grid->at(i+1,j) - grid->at(i,j))/grid->stepSize();
                float frontslope = (grid->at(i,j+1) - grid->at(i,j))/grid->stepSize();
                float rearslope = (grid->at(i,j) - grid->at(i,j-1))/grid->stepSize();
                float xcurve = (fabs(fabs(forslope)-fabs(backslope)));
                float ycurve = (fabs(fabs(frontslope)-fabs(rearslope)));

                //if( (xcurve<slopetrigger) && fabs(forslope)<sensitivity && fabs(backslope)<sensitivity
                //    && (ycurve<slopetrigger) && fabs(frontslope)<sensitivity && fabs(rearslope)<sensitivity)
                if (grid->at(i,j)>60)
                {
                    right.append(vectorFromIJ(i,j,0,grid->stepSize()));
                    i=0;
                }
            }
        }
        addedRight = (right.size()==rightsize);

        if (addedRight && ! addedLeft){
            left.append(vectorFromIJ(1+offsetFromBorder,j,0,grid->stepSize()));
        }else if(addedLeft && !addedRight){
            right.append((vectorFromIJ((grid->nx()-2-offsetFromBorder),j,0,grid->stepSize())));
        }

    }
    loop.points = left;
    for(int i=right.size()-1;i>0;i--){
        loop.points.append(right[i]);
    }


//    removedDuplicatePoints(& loop.points);
    return loop;


}

BoundaryMap boundaryGridDetection(XYGrid<float>* grid){
    /// FIND BORDERS
    QMap<int,QVector<int> > bordermap;
    float max;
    bool started= false;
    bool ended = false;
    for(int j=0;j<grid->ny();j++){
        QVector<int> border(2,0);
        bool foundLeft=false;
        bool foundRight=false;
        // FIND left border
        max = grid->at(0,j);
        for(int i=1;i<(grid->nx()-1);i++){
            if (!Math::floatsEqual(max,grid->at(i,j)) && !foundLeft ){
                border[0]=i;
                //printf("\ni:%i\tmax:%f\tval:%f",i,max,grid->at(i,j));
                foundLeft=true;
            }
        }

        //Find right border
        max = grid->at(grid->nx()-1,j);
        for(int i=(grid->nx()-1); i>1 ;i--){
            if ( !Math::floatsEqual(max,grid->at(i,j)) && (!foundRight) ) {
                border[1]=i;
//                printf("\ni:%i\tmax:%f\tval:%f",i,max,grid->at(i,j));
                foundRight=true;
            }
        }

        // if only 1 found, make border edge
        if (foundLeft && !foundRight ){
            border[1] = grid->nx()-1;
            if(!started){started=true;}
        }else if (!foundLeft && foundRight){
            border[0] = 0;
            if(!started){started=true;}
        }
        if(foundLeft&&foundRight&&!started){started=true;}

        if(!foundLeft&&!foundRight&& started){ended=true;}

        if ( foundLeft&&foundRight&& !ended){
            bordermap[j]=border;
//            printf("\n J:%i border: %i,%i",j,border[0],border[1]);
        }
    }
    return bordermap;
}



void mapOntoGrid(FAHLoopInXYPlane* loop, XYGrid<float>* grid){
    for(int i=0;i<loop->points.size();i++){
        float z = getHeightAt(loop->points[i].x,loop->points[i].y,grid);
        loop->points[i].z=z;
    }
}


QVector<FAHVector3> sortByIndex(QVector<FAHVector3> vec, int index, bool reverse){
    QMap<float,FAHVector3> map;
    for(int i=0; i<vec.size();i++){
        if(index==0){map[vec.at(i).x] = vec.at(i);}
        else if(index==1){map[vec.at(i).y] = vec.at(i);}
        else if(index==2){map[vec.at(i).z] = vec.at(i);}
    }
    QList<float> keys = map.keys();
    qSort(keys);
    QVector<FAHVector3> vecs;
    if(!reverse){
        for(int i=0; i<keys.size();i++){
            vecs.append(map[keys[i]]);
        }
    }else{
        for(int i=0; i<keys.size(); i++){
            vecs.append(map[keys[keys.size()-1-i]]);
        }
    }



    return vecs;
}



template float getHeightAt(float x, float y,XYGrid<float>* grid);
template float getHeightAt(float x, float y,XYGrid<int>* grid);
