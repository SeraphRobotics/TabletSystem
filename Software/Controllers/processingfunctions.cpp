#include "processingfunctions.h"
#include <Eigen/Dense>
#include <QtAlgorithms>
#include <algorithm>
#include <QImage>
#include <QColor>
#include "UnitTest/debugfunctions.h"
#include <Eigen/Eigenvalues>

using Eigen::MatrixXf;
using Eigen::VectorXf;
using Eigen::RowVectorXf;
using Eigen::Vector3f;
using Eigen::Vector4f;
using Eigen::Matrix3f;
using Eigen::EigenSolver;
//using Eigen::EigenvalueType;


FAHLoopInXYPlane loopFromPoints(QVector< FAHVector3 > healpts, QVector< FAHVector3 > forepts){
    QVector< FAHVector3 > curve = secondOrder(healpts);
    curve += bezier_curve(forepts);

    int numpts = curve.size();
    //find center
    FAHVector3 cent(0,0,0);
    for (int i=0;i<numpts;i++){
        cent.x=cent.x+curve.at(i).x;
        cent.y=cent.y+curve.at(i).y;
        printPoint(curve.at(i));
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
    FAHLoopInXYPlane loop;
    for(int i=0; i<numpts;i++){
//        loop.add( curve[indecies.at(i)] );
          loop.add(curve.at(i));
//        printPoint(curve[indecies.at(i)]);
    }

    return loop;
}

QVector< FAHVector3 > secondOrder(QVector< FAHVector3 >heal_pts, int nTimes){
    // This is a 6th order curve fittings system
    // y = ax6+bx5+cx4 ... e*x+f
    // dy = 6ax5+5bx4 ... e
    // M is the matrix of k*x^n, c is a vector of a,b,c,d,e,f
    // Y is a vector of y and dy
    QVector< FAHVector3 > returnpts;
    if(heal_pts.size()!=3){return returnpts;}
    int soln_order = 6;
    MatrixXf m(soln_order,soln_order);
    VectorXf y(soln_order);
    VectorXf c(soln_order);

    for(int i=0;i<heal_pts.size();i++){
        for(int j=0; j<soln_order;j++){
            m(i,j) = pow(heal_pts.at(i).x,float(j));
        }
        y(i) = heal_pts.at(i).y;
    }

    y[3] = -100;
    y[4] = 2*twoPtSlope(heal_pts[0],heal_pts.last());
    y[5] = 100;


    for(int k=0;k<soln_order;k++){
        m(3,k)+= k*pow(heal_pts.at(0).x,float(k));
        m(4,k)+= k*pow(heal_pts.at(1).x,float(k));
        m(5,k)+= k*pow(heal_pts.at(2).x,float(k));
    }

    c = m.inverse()*y;

    VectorXf xval(nTimes);
    VectorXf yval(nTimes);

    float stepsize = (heal_pts.at(2).x-heal_pts.at(0).x)/nTimes;
    for(int i=0; i<nTimes; i++){
        xval(i) = heal_pts.at(0).x+stepsize*i;
    }
    for(int i=0; i<nTimes;i++){
        for(int j=0; j<soln_order; j++){
            yval(i)+=c(j)*pow(xval(i),float(j));
        }
    }

    QVector<FAHVector3> curvepts;
    for(int i=0;i<nTimes;i++){
        FAHVector3 p(0,0,0);
        p.x = xval(i);
        p.y = yval(i);
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

void projectGridOntoPlane(FAHVector3 n, XYGrid< float >* grid){
    Vector3f r;
    Vector3f unnorm;
    unnorm[0]=n.x;
    unnorm[1]=n.y;
    unnorm[2]=n.z;

    n.normalize();
    Vector3f norm;
    norm[0]=n.x;
    norm[1]=n.y;
    norm[2]=n.z;
    float D=0;
    for(int i=0;i<grid->nx();i++){
        for(int j=0; j<grid->ny(); j++){
            r[0]=i;
            r[1]=j;
            r[2]=0;
            r=r-norm;
            D= r.dot(unnorm);
            grid->operator ()(i,j)=grid->operator ()(i,j)+D;
        }
    }
}

FAHVector3 makePostingPlane(FAHVector3 hp1,FAHVector3 hp2,FAHVector3 fp1, FAHVector3 fp2){

    /// using algorithm from http://stackoverflow.com/questions/1400213/3d-least-squares-plane
    ///https://groups.google.com/forum/#!topic/comp.graphics.algorithms/qrtcLb4QHFE


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
    return returnvec;
}

FAHVector3 minAlongLine(XYGrid< float >* grid, FAHVector3 p1, FAHVector3 p2){
    FAHVector3 v(0,0,0);
    FAHVector3 minpt(0,0,10000);
    FAHVector3 p = p1;
    FAHVector3 testp(0,0,0);
    v.x = p2.x-p1.x;
    v.y = p2.y-p1.y;
    //v.normalize();
    printPoint(v);
    float lastz=0;
    int numpts=1000;
    for(int i=0; i<numpts;i++){
        float t = 1.0/numpts*i;
        testp = p+t*v;
        int i = floor(testp.x);
        int j = floor(testp.y);
        testp.z=grid->operator ()(i,j);
        if (lastz!=testp.z){
            printPoint(testp);
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

void thresholdWithLoop(XYGrid< float >* grid, FAHLoopInXYPlane loop){
    float min = 0;
    for(int i=0; i<grid->nx();i++){
        for(int j=0; j<grid->ny();j++){
            FAHVector3 pt(i,j,0);
            if (loop.pointInside(pt)){
                if (min>grid->at(i,j)){
                    min = grid->at(i,j);
                }
            }
        }
    }

    for(int i=0; i<grid->nx();i++){
        for(int j=0; j<grid->ny();j++){
            grid->operator ()(i,j)=grid->at(i,j)-min;
        }
    }

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
    QImage img(grid->nx(), grid->ny(), QImage::Format_RGB32);


    QVector<float> data = grid->asVector();
    float minimum=1000;
    float maximum=-1000;
    for (int i=0;i<data.size();i++){
        maximum = std::max(data.at(i),maximum);
        minimum = std::max(data.at(i),minimum);
    }
    float range = maximum-minimum;

    int max_r = 200;
    int max_g = 200;
    for(int i=0;i<grid->nx();i++){
        for(int j=0;j<grid->ny();j++){
            float dist = (grid->at(i,j)-minimum)/range;
            int r = int( (1-dist)*max_r);
            int g = int( dist*max_g);
            QColor color(r,g,0);
            img.setPixel(i,j,color.rgb());
        }
    }
    return img;

}


QDebug operator<< (QDebug d,const FAHVector3 v){
    d<<"\nX: "<<v.x<<", Y: "<<v.y<<", Z: "<<v.z;
    return d;
}

float twoPtSlope(FAHVector3 p1, FAHVector3 p2){
    if( (p2.y-p1.y)==0 ){return 0;}
    return (p2.x-p1.x)/(p2.y-p1.y);
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
