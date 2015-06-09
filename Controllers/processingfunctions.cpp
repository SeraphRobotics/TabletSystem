#include "processingfunctions.h"
#include <QtAlgorithms>
#include <algorithm>
#include <QImage>
#include <QColor>
//#include "UnitTest/debugfunctions.h"
#include <iostream>
#include "scannerfunctions.h"

#ifdef Q_OS_LINUX
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Eigenvalues>
#else
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#endif


using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::RowVectorXf;
using Eigen::Vector3f;
using Eigen::Vector4f;
using Eigen::Matrix3f;
using Eigen::EigenSolver;
//using Eigen::EigenvalueType;


FAHLoopInXYPlane* bottomLoopFromPoints(QVector< FAHVector3 > healpts, QVector< FAHVector3 > forepts){
    QVector< FAHVector3 > healpts2;

    QVector<FAHVector3> masterpts;
    masterpts.append(healpts.first());
    masterpts.append(healpts.last());
    masterpts.append(forepts.first());
    masterpts.append(forepts.last());

    FAHVector3 cent(0,0,0);
    foreach(FAHVector3 pt, masterpts){
        cent.x=cent.x+pt.x;
        cent.y=cent.y+pt.y;
    }
    cent.x=cent.x/masterpts.size();
    cent.y=cent.y/masterpts.size();


    foreach(FAHVector3 pt, healpts){
        FAHVector3 pt2 = cent-pt;
        pt2.normalize();
        pt=pt+pt2*5.0;
        healpts2.append(pt);
    }

    QVector< FAHVector3 > curve;

    QVector<FAHVector3> pts;
    pts.append(forepts.first());
    pts.append(forepts.last());
    foreach(FAHVector3 pt,pts){
        FAHVector3 pt2 = cent-pt;
        pt2.normalize();
        pt=pt+pt2*10.0;
        curve.append(pt);
    }


    curve += secondOrder(healpts2,forepts, 50);
    curve += bezier_curve(forepts,50);
    FAHLoopInXYPlane* loop = new FAHLoopInXYPlane();
    loop->points=curve;
    sortLoop(loop);
    return loop;

}

FAHLoopInXYPlane* loopFromPoints(QVector< FAHVector3 > healpts, QVector< FAHVector3 > forepts){
    QVector< FAHVector3 > curve = secondOrder(healpts,forepts, 50);
    curve += bezier_curve(forepts,50);

    int numpts = curve.size();
    //find center
    FAHVector3 cent(0,0,0);
    for (int i=0;i<numpts;i++){
        cent.x=cent.x+curve.at(i).x;
        cent.y=cent.y+curve.at(i).y;
//        printPoint(curve.at(i));
    }
    cent.x=cent.x/numpts;
    cent.y=cent.y/numpts;

    QMap<float,FAHVector3> thetamap;
    for(int i=0;i<numpts;i++){
        float theta = atan2( (curve.at(i).y-cent.y), (curve.at(i).x - cent.x) );
        thetamap[theta] = curve[i];
    }

    //Sort by polar angle
    QList<float> indecies = thetamap.keys();
    qSort(indecies);

    //Add to loop by polar angle
    FAHLoopInXYPlane* loop = new FAHLoopInXYPlane();
    for(int i=0; i<numpts;i++){
          FAHVector3 temp = FAHVector3(thetamap[indecies.at(i)]);
          //// FIX FOR SCALE
          ////temp.x = temp.x*2.0;
          loop->add( temp );

//        printPoint(curve[indecies.at(i)]);
    }

    return loop;
}

QVector< FAHVector3 > secondOrder(QVector< FAHVector3 >heal_pts, QVector< FAHVector3 > forepts, int nTimes){
    QVector< FAHVector3 > returnpts;

    if(heal_pts.size()!=3){return returnpts;}
    // dx/dy since x is dependant
    float firstslope= -1.0/twoPtSlope(heal_pts.first(),forepts.first());
    // EXTRA_SCALE y[4] = 2*twoPtSlope(heal_pts[0],heal_pts.last());
    float middleslope = -1.0/twoPtSlope(heal_pts.last(),heal_pts.first()); //
    float endslope = 1.0/twoPtSlope(heal_pts.last(),forepts.last());

    returnpts+=fourthOrder(heal_pts.first(),firstslope,
                                 heal_pts.at(1),middleslope,nTimes/2);

    QVector< FAHVector3 > secondset = fourthOrder(heal_pts.at(1),middleslope,
                                                  heal_pts.last(),endslope,nTimes/2);
    secondset.pop_front();
    returnpts+=secondset;
    return returnpts;

}
QVector< FAHVector3 > fourthOrder(FAHVector3 Start,float startslope, FAHVector3 End,float endslope, int nTimes){

    /**
     * This is a 5th order curve fit with 6 variables
     * in general Y = f+ex+dx^2+cx^3+b*x^4+a*x^5
     *           dY/dx = 0*f +e+2x*d+3x^2*c+4x^3*b+5x^4*a
     * M is the matrix of know values, c is a vector of f,e,d,c,b,a
     * A is a vector of y and dy at given x so A=M*C or (X = M*Y)
     * C = M^-1 *A
     * in out system y is the independant variable and x is the dependant variable
     * so its a system of know X at given Y
     **/

    QVector< FAHVector3 > ends;
    ends.append(Start);
    ends.append(End);

    int soln_order = 4;
    MatrixXf m(soln_order,soln_order);
    m = MatrixXf::Zero(soln_order,soln_order);
    MatrixXf mi(soln_order,soln_order);
    mi = MatrixXf::Zero(soln_order,soln_order);
    VectorXf A(soln_order);
    A=VectorXf::Zero(soln_order);
    VectorXf c(soln_order);
    c=VectorXf::Zero(soln_order);

    // column, row
    for(int i=0;i<ends.size();i++){
        for(int j=0; j<soln_order;j++){
            m(i,j) = pow(ends.at(i).y,float(j));
        }
        A(i) = ends.at(i).x;
    }
    A[2] = startslope;
    A[3] = endslope;



    // column, row
    for(int k=1;k<soln_order;k++){
        m(2,k) = k*pow(ends.at(0).y,float(k-1));
        m(3,k) = k*pow(ends.at(1).y,float(k-1));
    }


    mi = m.inverse();
    c = m.inverse()*A;

    VectorXf xval(nTimes);
    VectorXf yval(nTimes);
    yval = VectorXf::Zero(nTimes);

    float stepsize = (ends.last().y-ends.first().y)/nTimes;
    for(int i=0; i<nTimes; i++){
        xval(i) = ends.first().y+stepsize*i;
    }
    for(int i=0; i<nTimes;i++){
        for(int j=0; j<soln_order; j++){
            yval(i)+=c(j)*pow(xval(i),float(j));
        }
    }

    QVector<FAHVector3> curvepts;
    for(int i=0;i<nTimes;i++){
        FAHVector3 p(0,0,0);
        p.y = xval(i);
        p.x = yval(i);
        curvepts.append(p);
    }
    return curvepts;


}

QVector< FAHVector3 > bezier_curve(QVector< FAHVector3 > points, int nTimes){
    /**
       Given a set of control points, return the
       bezier curve defined by the control points.

       points should be a list of lists, or list of tuples
       such as [ [1,1],
                 [2,3],
                 [4,5], ..[Xn, Yn] ]
        nTimes is the number of time steps, defaults to 1000

        See http://processingjs.nihongoresources.com/bezierinfo/
    """

    nPoints = len(points)
    xPoints = np.array([p[0] for p in points])
    yPoints = np.array([p[1] for p in points])

    t = np.linspace(0.0, 1.0, nTimes)

    polynomial_array = np.array([ bernstein_poly(i, nPoints-1, t) for i in range(0, nPoints)   ])

    xvals = np.dot(xPoints, polynomial_array)
    yvals = np.dot(yPoints, polynomial_array)
    **/
    int npoints = points.size();
    MatrixXf m(npoints,nTimes);
    RowVectorXf xs(npoints);
    RowVectorXf ys(npoints);
    VectorXf xval(nTimes);
    VectorXf yval(nTimes);

    for(int i=0;i<npoints;i++){
        xs(i) = points.at(i).x;
        ys(i) = points.at(i).y;
    }


    //poly array is nPoints X nTimes
    for(int l=0;l<nTimes;l++){
        float t = 1.0/nTimes*l;
        for(int i=0;i<npoints;i++){
            m(i,l)  = bernstein_poly(i,npoints-1,t);
        }
    }

    xval = xs*m;
    yval = ys*m;

    QVector<FAHVector3> curvepts;
    for(int i=0;i<nTimes;i++){
        FAHVector3 p(0,0,0);
        p.x = xval(i);
        p.y = yval(i);
        curvepts.append(p);
    }
    return curvepts;

}

float bernstein_poly(int i, int n, float t){
    return comb(n,i)*pow(t,float((n-i)))*pow((1-t),float(i));
}

void projectGridOntoPlane(FAHVector3 n, FAHVector3 cent, XYGrid< float >* grid){
    Vector3f r;
    Vector3f centv;
    centv[0]=cent.x;
    centv[1]=cent.y;
    centv[2]=cent.z;

    n.normalize();
    Vector3f norm;
    norm[0]=n.x;
    norm[1]=n.y;
    norm[2]=n.z;


    /// using the eq from http://mathworld.wolfram.com/Point-PlaneDistance.html
    ///D = n*(x-cent)
    /// the sign of D is + if the planes are on the same side of origin
    /// the sign of D is - if the planes are on the oposite side of origin
    ///Since we will compare to the XY plane, we need to multiple by -1 of a points above and 1 if z is bellow
    int signN=-1;
    if(norm[2]<0){signN=1;}
//    std::cout<<"\n"<<centv;
//    std::cout<<"\n"<<norm<<"\n";


//    float Dz=-1*(n.x*cent.x+n.y+cent.y*n.y+n.z*cent.z);

    float D=0;
    for(int i=0;i<grid->nx();i++){
        for(int j=0; j<grid->ny(); j++){
            r[0]=i;
            r[1]=j;
            r[2]=0;
            r=r-centv;
            D= signN*r.dot(norm);

//            D = (Dz-n.x*i+n.y*j)/n.z;
            grid->operator ()(i,j)=grid->operator ()(i,j)-D;
//            if(i==10 and j==10){qDebug()<<"D: "<<QString::number(D);}
        }
    }
}

QVector<FAHVector3> makePostingPlane(FAHVector3 hp1,FAHVector3 hp2,FAHVector3 fp1, FAHVector3 fp2){

    /// using algorithm from http://stackoverflow.com/questions/1400213/3d-least-squares-plane
    ///https://groups.google.com/forum/#!topic/comp.graphics.algorithms/qrtcLb4QHFE
    QVector<FAHVector3> returns;

    /// We calculate the center of the points
    FAHVector3 cent = hp1+hp2+fp1+fp2;
    cent = cent/4.0;
    ///And subtract center from the points
    QList<FAHVector3> pts;
    pts.append(hp1-cent);
    pts.append(hp2-cent);
    pts.append(fp1-cent);
    pts.append(fp2-cent);


    /// We produce the vector m
    Matrix3f m = Matrix3f::Zero();
    for( int i=0;i<pts.size();i++){
        m(0,0)= m(0,0)+ pts.at(i).x*pts.at(i).x;
        m(0,1)= m(0,1)+ pts.at(i).x*pts.at(i).y;
        m(1,0)= m(1,0)+ pts.at(i).x*pts.at(i).y;
        m(1,1)= m(1,1)+ pts.at(i).y*pts.at(i).y;
        m(0,2)= m(0,2)+ pts.at(i).x*pts.at(i).z;
        m(2,0)= m(2,0)+ pts.at(i).x*pts.at(i).z;
        m(1,2)= m(1,2)+ pts.at(i).y*pts.at(i).z;
        m(2,1)= m(2,1)+ pts.at(i).y*pts.at(i).z;
        m(2,2)= m(2,2)+ pts.at(i).z*pts.at(i).z;
    }

    ///We find the smallest eigenvalue of m
//    std::cout<<m;

    EigenSolver<Matrix3f> es(m);
    std::complex<float> min (100000,0);
    float realmin = fabs(std::real(min));

    int index = 0;
    for(int i=0;i<3;i++){
        float realnew = fabs(std::real(es.eigenvalues()(i)));
        if(realnew < realmin){
            realmin=realnew;
            min = es.eigenvalues()(i);
            index=i;
        }
    }

    ///make a FAHvector of the eigenvector with the smallest eigenvalue
    FAHVector3 returnvec(std::real(es.eigenvectors().col(index)[0]),
                         std::real(es.eigenvectors().col(index)[1]),
                         std::real(es.eigenvectors().col(index)[2])
                         );
//    printPoint(returnvec);

    /// we unnormalize the vector to give us the proper plane vector which is normal to the surface and points from the origin to the centerpoint
    float d = returnvec.x*cent.x+returnvec.y*cent.y+returnvec.z*cent.z;
    if(d==0){d=1;}
    returnvec*=d;
    returns.append(returnvec);
    returns.append(cent);
    return returns;
}

FAHVector3 minAlongLine(XYGrid< float >* grid, FAHVector3 p1, FAHVector3 p2){
    FAHVector3 v(0,0,0);
    FAHVector3 minpt(0,0,10000);
    FAHVector3 p = p1;
    FAHVector3 testp(0,0,0);
    v.x = p2.x-p1.x;
    v.y = p2.y-p1.y;
    //v.normalize();
//    printPoint(v);
    float lastz=0;
    int numpts=1000;
    for(int i=0; i<numpts;i++){
        float t = 1.0/numpts*i;
        testp = p+t*v;
        //NOTE:: btl check git conflict
        /* <<<<<<< HEAD:Controllers/processingfunctions.cpp
        int ix = floor(testp.x);
        int j = floor(testp.y);
        testp.z=grid->operator ()(ix,j);
        ======= */
        int xi = floor(testp.x);
        int j = floor(testp.y);
        testp.z=grid->operator ()(xi,j);
        //>>>>>>> origin/pluggedInScanner:Software/Controllers/processingfunctions.cpp
        if (lastz!=testp.z){
//            printPoint(testp);
            lastz = testp.z;
        }
        if (testp.z < minpt.z){
            minpt = testp;
        }
    }
    return minpt;
}

FAHVector3 normFrom3Pts(FAHVector3 p1, FAHVector3 p2,FAHVector3 p3){
    Vector3f p21( (p2.x-p1.x), (p2.y-p1.y),(p2.z-p1.z) );
    Vector3f p31( (p3.x-p1.x), (p3.y-p1.y),(p3.z-p1.z) );

    Vector3f cp = p21.cross(p31);
    cp=cp.normalized();
    return FAHVector3(cp[0],cp[1],cp[2]);
}

void thresholdWithLoop(XYGrid< float >* grid, FAHLoopInXYPlane* loop){
    float min = 10000;
    for(int i=0; i<grid->nx();i++){
        for(int j=0; j<grid->ny();j++){
            FAHVector3 pt=vectorFromIJ(i,j,0,1,1);
            if (loop->pointInside(pt)){
                if (min>grid->at(i,j)){
                    min = grid->at(i,j);
                }
            }
        }
    }
//    qDebug()<<"Min: "<<min;
    for(int i=0; i<grid->nx();i++){
        for(int j=0; j<grid->ny();j++){
            if(grid->at(i,j)<min){
                grid->operator ()(i,j)=0.0;
            }else{
                grid->operator ()(i,j)=grid->at(i,j)-min;
            }
        }
    }

}



void blurInLoop(XYGrid<float>* grid,FAHLoopInXYPlane* borderloop, int times){
    QList<FAHLoopInXYPlane*> innerLoops;
    for(int n=0;n<times;n++){
        XYGrid<float> copy(grid->asVector(),grid->ny(),grid->stepSizeX(),grid->stepSizeY());

        for(int j=1;j<grid->ny()-1;j++){
            for(int i=1;i<grid->nx()-1;i++){

                FAHVector3 p1,p2,p3,p4,pcent;
                bool b1=true,b2=true,b3=true,b4=true,bp=true;
                Q_UNUSED(bp);

                QVector<FAHVector3> pts;

                pcent = vectorFromIJ(i,j,copy.at(i,j),copy.stepSizeX(),copy.stepSizeY());
                bp = loopsContain(pcent,borderloop,innerLoops);

                p1=vectorFromIJ(i,j,copy.at(i-1,j),copy.stepSizeX(),copy.stepSizeY());
                b1=loopsContain(p1,borderloop,innerLoops);
                if(b1){pts.append(p1);
                }else{
                    p1.z=pcent.z;
                    pts.append(p1);
                }

                p2=vectorFromIJ(i+1,j,copy.at(i+1,j),copy.stepSizeX(),copy.stepSizeY());
                b2=loopsContain(p2,borderloop,innerLoops);
                if(b2){pts.append(p2);}else{
                    p2.z=pcent.z;
                    pts.append(p2);
                }



                p3=vectorFromIJ(i,j+1,copy.at(i,j-1),copy.stepSizeX(),copy.stepSizeY());
                b3=loopsContain(p3,borderloop,innerLoops);
                if(b3){pts.append(p3);}else{
                    p3.z=pcent.z;
                    pts.append(p3);
                }

                p4=vectorFromIJ(i+1,j+1,copy.at(i,j+1),copy.stepSizeX(),copy.stepSizeY());
                b4=loopsContain(p4,borderloop,innerLoops);
                if(b4){pts.append(p4);}else{
                    p4.z=pcent.z;
                    pts.append(p4);
                }

                if (pts.size()>0){
                    float z = 0;
                    foreach(FAHVector3 pt,pts){
                        z+=pt.z;
                    }
                    z= z/pts.size();
//                    (p1.z+p2.z+p3.z+p4.z)/4.0;
                    grid->operator ()(i,j)=z;
                }
            }
        }
    }
}

void anchorFront(XYGrid<float>* grid,QVector<FAHVector3>forepts){
    QVector<FAHVector3> pts = bezier_curve(forepts,500);
    float minz = 1000;
    foreach(FAHVector3 pt,pts){
        int i = int(pt.x);
        int j = int(pt.y);
        minz = std::min(grid->at(i,j),minz);
    }

    int bordersize=3;
    foreach(FAHVector3 pt,pts){
        int i = int(pt.x);
        int j = int(pt.y);
        for(int p=-bordersize;p<bordersize;p++){
            grid->operator ()(i+p,j)=minz;
            grid->operator ()(i,j+p)=minz;
            grid->operator ()(i+p,j+p)=minz;
        }
    }
}

void blurByBorder(XYGrid<float>* grid,FAHLoopInXYPlane* borderloop, int times){
    FAHLoopInXYPlane* mapped = mapOntoGrid(borderloop,grid);
    QList<FAHLoopInXYPlane*> innerLoops;
    QVector<FAHVector3> pts;
    foreach(FAHVector3 pt,mapped->points){
        if(!pt.isInvalid()){pts.append(pt);}
    }

    int size =  pts.size();
    int bordersize = 2;
    for(int n=0; n<times;n++){
        XYGrid<float> copy(grid->asVector(),grid->ny(),grid->stepSizeX(),grid->stepSizeY());
        for(int k=0; k<size-1; k++){
            FAHVector3 pt =  pts.at(k) ;

            for(int p=-bordersize;p<bordersize;p++){
                for(int n=-bordersize;n<bordersize;n++)
                {
                    int i = int(pt.x)+p;
                    int j = int(pt.y)+n;


                    FAHVector3 p1,p2,p3,p4,pcent;
                    bool b1=true,b2=true,b3=true,b4=true,bp=true;
                    Q_UNUSED(bp);

                    QVector<FAHVector3> pts;

                    p1=vectorFromIJ(i,j,copy.at(i-1,j),copy.stepSizeX(),copy.stepSizeY());
                    b1=loopsContain(p1,borderloop,innerLoops);
                    if(b1){pts.append(p1);}

                    p2=vectorFromIJ(i+1,j,copy.at(i+1,j),copy.stepSizeX(),copy.stepSizeY());
                    b2=loopsContain(p2,borderloop,innerLoops);
                    if(b2){pts.append(p2);}

                    pcent = vectorFromIJ(i,j,copy.at(i,j),copy.stepSizeX(),copy.stepSizeY());
                    bp = loopsContain(pcent,borderloop,innerLoops);

                    p3=vectorFromIJ(i,j+1,copy.at(i,j-1),copy.stepSizeX(),copy.stepSizeY());
                    b3=loopsContain(p3,borderloop,innerLoops);
                    if(b3){pts.append(p3);}

                    p4=vectorFromIJ(i+1,j+1,copy.at(i,j+1),copy.stepSizeX(),copy.stepSizeY());
                    b4=loopsContain(p4,borderloop,innerLoops);
                    if(b4){pts.append(p4);}

                    if (pts.size()>0){
                        float z = 0;
                        foreach(FAHVector3 pt,pts){
                            z+=pt.z;
                        }
                        z= z/pts.size();
                        grid->operator ()(i,j)=z;
                    }
                }
            }
        }
    }
}



void normalizeBorder(XYGrid<float>* grid,FAHLoopInXYPlane* borderloop, int times){
    FAHLoopInXYPlane* mapped = mapOntoGrid(borderloop,grid);

//    writeLoopToXDFL(mapped,"mapped.xdfl");

    QVector<FAHVector3> pts;

    foreach(FAHVector3 pt,mapped->points){
        if(!pt.isInvalid()){pts.append(pt);}
    }

    for(int n=0; n<times;n++){
        QVector<FAHVector3> newpts;
        int size = pts.size();//mapped->points.size();
        for(int i=0; i<size; i++){
            //TODO:: btl proper fix for out of bounds errors
            if(i%size < 0 || i%size > size )
                qDebug() << "out of bounds i%size"; continue;
            if((i+1)%size < 0 || (i+1)%size > size)
                qDebug() << "out of bounds (i+1)%size"; continue;
            if((i-1)%size < 0 || (i-1)%size > size)
                qDebug() << "out of bounds (i-1)%size: " << (i-1)%size; continue;
            float z1 = pts.at( i%size ).z;
            float z2 = pts.at( (i+1)%size ).z;
            float z3 = pts.at( (i-1)%size ).z;
            if(i==0){
                z3=pts.last().z;
            }

//            if(i==size-1){
//                z1=0;//pts.last().z;
//                z2=0;//pts.first().z;
//                z3=0;//pts.at( (size-2) ).z;
//            }

            float z = (z1+z2+z3)/3.0;
//            if (z1==0 || z2==0 ||z3==0 || (i==size-1 ) ){//|| z>1.1*z3 || z>1.1*z1
//                qDebug()<<i<<": "<<z1<<","<<z2<<","<<z3;
//            }
            FAHVector3 pt =  pts[(i%size)] ;
            pt.z=z;
            newpts.append(pt);
        }
        pts.clear();
        pts=newpts;
    }

    int size =  pts.size();
    int bordersize = 2;
    for(int k=0; k<size-1; k++){
        FAHVector3 pt =  pts.at(k) ;
        int i = int(pt.x);
        int j = int(pt.y);
        for(int p=-bordersize;p<bordersize;p++){
            grid->operator ()(i+p,j)=pt.z;
            grid->operator ()(i,j+p)=pt.z;
//            grid->operator ()(i+p,j+p)=pt.z;
        }
//        grid->operator ()(i+1,j)=pt.z;
//        grid->operator ()(i-1,j)=pt.z;
//        grid->operator ()(i,j+1)=pt.z;
//        grid->operator ()(i,j-1)=pt.z;
    }
    delete mapped;

}

QVector< FAHVector3> transformPointsWithPosting(FAHVector3 p1,FAHVector3 p2,Posting p){
    p2.z=0;
    p1.z=0;

    /**
      We calculate the height of a triangle

          /|
         / | H
        /  |
      p2---p1
         d

      but we invert all of the heights since this will be used to make a plane vector for remaping the grid.
    **/

    FAHVector3 D = p2-p1;
    float h = D.magnitude()*tan(p.angle);
    if(Posting::kValgus==p.varus_valgus){
        p2.z=-1.0*(h+p.verticle);
        p1.z=-1.0*(p.verticle);
    }else{
        p2.z=-1.0*(p.verticle);
        p1.z=-1.0*(h+p.verticle);
    }
    QVector< FAHVector3> returnvec;
    returnvec.append(p1);
    returnvec.append(p2);
    return returnvec;
}


QImage makeHeightMap(XYGrid< float >* grid){
    QImage img(grid->ny(),grid->nx(), QImage::Format_RGB32);


    QVector<float> data = grid->asVector();
    float minimum=1000;
    float maximum=-1000;
    for (int i=0;i<data.size();i++){
        maximum = std::max(data.at(i),maximum);
        minimum = std::min(data.at(i),minimum);
    }
    float range = maximum-minimum;
    if(range<1){return img;}

    for(int i=0;i<grid->nx();i++){
        for(int j=0;j<grid->ny();j++){
            float dist = (grid->at(i,j)-minimum)/range;

            int r = falseColorR(dist);
            int g = 255-falseColorG(dist);
            //qDebug()<<"dist: "<<dist<<"\tr:"<<r<<"\tg"<<g;
            //QColor color(r,g,0);

            QColor color = QColor::fromHsv(240,g,r);

            img.setPixel(j,i,color.rgb());
        }
    }
    return img;

}

int falseColorR(float dist){
    int max_r = 250;
    float thresh = 0.7;
    if(dist>1){return max_r;}
    if(dist< thresh) {
        return max_r;
    }
    float m = -max_r/(1.0-thresh);
    float b = max_r-(m*thresh);
    return int(m*dist+b);
}
int falseColorG(float dist){
    int max_g = 200;
    float thresh = 0.7;
    if(dist>thresh){return max_g;}
    float m = max_g/thresh;
    float b = max_g-(m*thresh);
    return int(m*dist+b);
}


QDebug operator<< (QDebug d,const FAHVector3 v){
    d<<"\nX: "<<v.x<<", Y: "<<v.y<<", Z: "<<v.z;
    return d;
}

float twoPtSlope(FAHVector3 p1, FAHVector3 p2){
    if( (p2.x-p1.x)==0 ){return 0;}
    return (p2.y-p1.y)/(p2.x-p1.x);
}


int comb(int n, int k){
    int result = 1;
    if(k>n || n<0 ||k<0){return 0;}

    result = factorial(n)/(factorial(k)*factorial(n-k));
    return result;
}

int factorial(int n){
    int result = 1;
    for(int i=1;i<n+1;i++){
        result = result*i;
    }
    return result;
}
