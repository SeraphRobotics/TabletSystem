#include "stlgeneration.h"
#include "../libraries/shared/stl/stlmesh.h"
#include "../libraries/shared/stl/stlfile.h"
#include "../libraries/shared/stl/stlfacet.h"
#include "UnitTest/debugfunctions.h"

#include <iostream>


FAHLoopInXYPlane* angledBase(float theta, float min_threshold, float max_threshold, FAHLoopInXYPlane* baseloop){
    QVector<FAHVector3>points = baseloop->points;
    QVector<FAHVector3>new_points;
    new_points += points;

    float amount=0;

    // Set displacements for the loop
    for (int i = 0; i < points.size(); ++i) {

      //Dont use loop less than threshold
      if(points[i].z<min_threshold){
          continue;
      }else if(points[i].z>max_threshold){
          amount = max_threshold*tan(Math::kPi/180*theta);
      }else{
          amount = points[i].z*tan(Math::kPi/180*theta);
      }
      int before = i - 1;
      if (before == -1) before = points.size() - 1;
      int after = i + 1;
      if (after == points.size()) after = 0;

      FAHLine before_segment(points[before], points[i]);
      FAHLine after_segment(points[i], points[after]);

      FAHVector3 before_ray = before_segment.ray();
      FAHVector3 after_ray  = after_segment.ray();

      // form the perpendicular displacement vector
      FAHVector3 before_displacement;
      before_displacement.x =  before_ray.y;
      before_displacement.y = -before_ray.x;
      before_displacement.z = 0.0;
      before_displacement.normalize();

      FAHVector3 after_displacement;
      after_displacement.x =  after_ray.y;
      after_displacement.y = -after_ray.x;
      after_displacement.z = 0.0;
      after_displacement.normalize();

      // sum the displacements and adjust the current vertex
      FAHVector3 pt_displacement;
      pt_displacement.set(before_displacement).add(after_displacement);
      pt_displacement.normalize().scale(-amount);
      pt_displacement.z=0;
      new_points[i].add(pt_displacement);
    }
    FAHLoopInXYPlane* returnloop = new FAHLoopInXYPlane();
//    returnloop->points=new_points;
    for(int i=0;i<new_points.size();i++){
        new_points[i].z=0;
        returnloop->points.append(new_points[i]);
//        returnloop->points[i].z=0;
    }
//    sortLoop(returnloop);
    QVector<FAHLoopInXYPlane> planes;
    returnloop->simplifyAndExpand(0,&planes);
    qDebug()<<"\nplanes:"<<planes.size();
    return returnloop;
}


STLMesh* STLFromSection(XYGrid<float>* grid, FAHLoopInXYPlane* angled, FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoop){
    STLMesh* mesh= new STLMesh();

//    kChannelType type;

    FAHLoopInXYPlane* borderWithHeight = mapOntoGrid(OuterLoop,grid);
    FAHLoopInXYPlane* angleloop = mapOntoGrid(angled,grid,false);

//    qDebug()<<"mapped:"<<borderWithHeight->points.size();
//    qDebug()<<"unmapped:"<<OuterLoop->points.size();
//    qDebug()<<"bottom:"<<angled->points.size();
//    qDebug()<<"bottom mapped"<<angleloop->points.size();



//    writeLoopToXDFL(angleloop,"angled.xdfl");
    for(int j=0;j<grid->ny()-1;j++){
        for(int i=0;i<grid->nx()-1;i++){

            addSquareToSTL(i,j,grid, mesh, OuterLoop, innerLoop,true);

            addSquareToSTL(i,j,grid, mesh, angled, innerLoop,false);
        }
    }

    addBetweenTwoLoopsToSTL(mesh,borderWithHeight,angleloop);

    delete borderWithHeight;
    delete angleloop;

    return mesh;
}




template <class T>
void addSquareToSTL(int i, int j, XYGrid<T>* grid, STLMesh *mesh,
                    FAHLoopInXYPlane* OuterLoop,
                    QList<FAHLoopInXYPlane*> innerLoops, bool top){

    /** calculated vOuterLoophe 4 points and determin case
     *  p1--p2 -->i x  thesebounds
     *  | \  |
     *  p3--p4         nextbounds
     *  |
     * \|/
     *  j
     *  y
     *
     *
     **/
    FAHVector3 p1,p2,p3,p4,d(0,0,-1);
    bool b1=true,b2=true,b3=true,b4=true;

    int numInBounds=0;
//    int numOnLoop=0;

    p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
    b1=loopsContain(p1,OuterLoop,innerLoops);
    if(b1){numInBounds++;}

    p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
    b2=loopsContain(p2,OuterLoop,innerLoops);
    if(b2){numInBounds++;}

    p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
    b3=loopsContain(p3,OuterLoop,innerLoops);
    if(b3){numInBounds++;}

    p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());
    b4=loopsContain(p4,OuterLoop,innerLoops);
    if(b4){numInBounds++;}

    if(numInBounds<1){return;}

//    if(numInBounds!=2){return;}

    if(!top){// if its the bottom set all Z points to 0
        p1.z=0;
        p2.z=0;
        p3.z=0;
        p4.z=0;
        d.z=1;
    }

    FAHLine l12(p1,p2);
    l12.a.z=0;
    l12.b.z=0;
    FAHLine l13(p1,p3);
    l13.a.z=0;
    l13.b.z=0;
    FAHLine l24(p2,p4);
    l24.a.z=0;
    l24.b.z=0;
    FAHLine l34(p3,p4);
    l34.a.z=0;
    l34.b.z=0;
    QList<FAHLine> lines;
    lines.append(l12);
    lines.append(l13);
    lines.append(l24);
    lines.append(l34);


    QVector<FAHVector3> outerpoints;
    for(int k=0;k<OuterLoop->points.size(); k++){
        if ((OuterLoop->points[k].x > p1.x) &&
            (OuterLoop->points[k].x < p4.x) &&
            (OuterLoop->points[k].y > p1.y) &&
            (OuterLoop->points[k].y < p4.y) ){
            outerpoints.append(OuterLoop->points[k]);
        }
    }

    QVector<FAHVector3> innerpoints;
    for(int l=0; l<innerLoops.size();l++){
        for(int k=0;k<innerLoops[l]->points.size(); k++){
            if ((OuterLoop->points[k].x > p1.x) &&
                (OuterLoop->points[k].x < p4.x) &&
                (OuterLoop->points[k].y > p1.y) &&
                (OuterLoop->points[k].y < p4.y) ){
                innerpoints.append(OuterLoop->points[k]);
            }
        }
    }


    QVector<FAHVector3> intersected;
    QVector<FAHVector3> points;
    QVector<FAHVector3> sorted_points;
    FAHVector3 anchor_main(0,0,0), anchor_left(0,0,0), anchor_right(0,0,0);
    FAHVector3 anchor1(0,0,0), anchor2(0,0,0);
    FAHVector3 anchor(0,0,0);

    switch(numInBounds){
    case 4:
        addFacetWithDirection(p1,p2,p4,mesh,d);
        addFacetWithDirection(p1,p3,p4,mesh,d);
        break;
    case 3:
        intersected = intersection_points(lines,OuterLoop,innerLoops);

        points = outerpoints;
        points =points + innerpoints;
        points =points + intersected;

        for(int i=0;i<points.size();i++){
            points[i].z=getHeightAt(points[i].x,points[i].y,p1,p2,p3,p4);
        }


        if(!b4){
            anchor_main = p1;
            anchor_left = p2;
            anchor_right = p3;
            sorted_points = sortByIndex(points,0,false);
        }else if(!b3){
            anchor_main = p2;
            anchor_left = p4;
            anchor_right = p1;
            sorted_points = sortByIndex(points,1,false);
        }else if(!b2){
            anchor_main = p3;
            anchor_left = p1;
            anchor_right = p4;
            sorted_points = sortByIndex(points,1,true);
        }else if(!b1){
            anchor_main = p4;
            anchor_left = p3;
            anchor_right = p2;
            sorted_points = sortByIndex(points,0,true);
//            break;
        }
        for(int m=0; m<sorted_points.size()-1; m++){
            addFacetWithDirection(anchor_main,sorted_points[m],sorted_points[m+1],mesh,d);
        }
        addFacetWithDirection(anchor_main, anchor_left, sorted_points.last(), mesh, d);
        addFacetWithDirection(anchor_main, anchor_right, sorted_points.first(), mesh, d);

        break;
    case 2:
        intersected = intersection_points(lines,OuterLoop,innerLoops);

        points = outerpoints;
        points =points + innerpoints;
        points = points + intersected;

        for(int i=0;i<points.size();i++){
            points[i].z=getHeightAt(points[i].x,points[i].y,p1,p2,p3,p4);
        }


        if(b1&&b2){
            anchor1 = p1;
            anchor2 = p2;
            sorted_points = sortByIndex(points,0,false);
        }else if(b2&&b4){
            anchor1 = p2;
            anchor2 = p4;
            sorted_points = sortByIndex(points,1,false);
        }else if(b1&&b3){
            anchor1 = p1;
            anchor2 = p3;
            sorted_points = sortByIndex(points,1,false);
        }else if(b3&&b4){
            anchor1 = p3;
            anchor2 = p4;
            sorted_points = sortByIndex(points,0,false);
        }
        for(int m=0; m<sorted_points.size()-1; m++){
//            printPoint( sorted_points[m] );
            addFacetWithDirection(anchor1,sorted_points[m],sorted_points[m+1],mesh,d);
        }
        addFacetWithDirection(anchor1,anchor2,sorted_points.last(),mesh,d);

        break;
    case 1:

        intersected = intersection_points(lines,OuterLoop,innerLoops);
//        for(int k=0;k<intersected.size(); k++){
//            qDebug()<<"I: "<<intersected[k].x<<","<<intersected[k].y<<","<<intersected[k].z;
////            intersected[k].z=getHeightAt(intersected[k].x,intersected[k].y,grid);
//        }
        points = intersected;
        points = points + outerpoints;
        points = points + innerpoints;

        for(int i=0;i<points.size();i++){
            points[i].z=getHeightAt(points[i].x,points[i].y,p1,p2,p3,p4);
        }

//        qDebug()<<"N=1: ";
//        qDebug()<<"points: "<<points.size();
//        qDebug()<<"\touterpoints: "<<outerpoints.size();
//        qDebug()<<"\tinnerpoints: "<<innerpoints.size();
//        qDebug()<<"\tintersected points:" <<intersected.size();

        if(b1){
            sorted_points = sortByIndex(points,0,false);
            anchor = p1;
        }else if(b2){
            sorted_points = sortByIndex(points,0,false);
            anchor = p2;
        }else if(b3){
            sorted_points = sortByIndex(points,1,false);
            anchor = p3;
        }else if(b4){
            sorted_points = sortByIndex(points,1,false);
            anchor = p4;
        }

//        qDebug()<<"Sorted size:"<<sorted_points.size();


        for(int m=0; m<sorted_points.size()-1; m++){
            addFacetWithDirection(anchor,sorted_points[m],sorted_points[m+1],mesh,d);
        }
        break;
    default:
        break;

    }



}



void addChannelToSTL(int i, int j, XYGrid<float>* grid, STLMesh* mesh, FAHLoopInXYPlane *OuterLoop, QList<FAHLoopInXYPlane *> innerLoops, kChannelType type, float depth){
    FAHVector3 p1,p2,p3,p4,d(0,0,-1);
    FAHVector3 d1,d2,d3,d4;
    bool b1=true,b2=true,b3=true,b4=true;

    int numInBounds=0;

    p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
    d1=p1;
    d1.z=p1.z-depth;
    b1=loopsContain(p1,OuterLoop,innerLoops);
    if(b1){numInBounds++;}

    p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
    d2=p2;
    d2.z=p2.z-depth;
    b2=loopsContain(p2,OuterLoop,innerLoops);
    if(b2){numInBounds++;}

    p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
    d3=p3;
    d3.z=p3.z-depth;
    b3=loopsContain(p3,OuterLoop,innerLoops);
    if(b3){numInBounds++;}

    p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());
    d4=p4;
    d4.z=p4.z-depth;
    b4=loopsContain(p4,OuterLoop,innerLoops);
    if(b4){numInBounds++;}


    if(numInBounds<4){
        addSquareToSTL(i,j,grid, mesh, OuterLoop, innerLoops,true);
        return;
    }

        addFacetWithDirection(d1,d2,d4,mesh,d);
        addFacetWithDirection(d1,d3,d4,mesh,d);
        /** calculated vOuterLoophe 4 points and determin case
         *  p1--p2 -->i x  thesebounds
         *  | \  |
         *  p3--p4         nextbounds
         *  |
         * \|/
         *  j
         *  y
         *
         *
         **/
        //enum kChannelType{sqr,left,right,top,bottom,tlc,trc,brc,blc};
        //ctb,clr,cltb,crtb
        QList<kChannelType> types;
        if(type==ctlc){
            types.append(cleft);
            types.append(ctop);

        }else if(type == ctrc){
            types.append(cright);
            types.append(ctop);

        }else if(type == cbrc){
            types.append(cright);
            types.append(cbottom);

        }else if(type == cblc){
            types.append(cleft);
            types.append(cbottom);

        }else if(type == ctb){
            types.append(ctop);
            types.append(cbottom);

        }else if(type == clr){
            types.append(cleft);
            types.append(cright);

        }else if(type == cltb){
            types.append(cleft);
            types.append(ctop);
            types.append(cbottom);

        }else if(type == crtb){
            types.append(cright);
            types.append(ctop);
            types.append(cbottom);

        }else{
            types.append(type);
        }

        QListIterator<kChannelType> t(types);

        while (t.hasNext()){
            kChannelType next = t.next();
            if(ctop == next){
                d=FAHVector3(0,-1,0);
                addFacetWithDirection(p1,d1,d2,mesh,d);
                addFacetWithDirection(p1,p2,d2,mesh,d);
            }else if(cbottom==next){
                d=FAHVector3(0,1,0);
                addFacetWithDirection(p3,d3,d4,mesh,d);
                addFacetWithDirection(p3,p4,d4,mesh,d);
            }else if(cleft==next){
                d=FAHVector3(-1,1,0);
                addFacetWithDirection(p1,d1,d3,mesh,d);
                addFacetWithDirection(p1,p3,d3,mesh,d);
            }else if(cright==next){
                d=FAHVector3(1,0,0);
                addFacetWithDirection(p2,d2,d4,mesh,d);
                addFacetWithDirection(p2,p4,d4,mesh,d);
            }
        }
}


template <class T>
STLMesh* makeSTLfromScan(XYGrid<T>* grid ){
    STLMesh* mesh= new STLMesh();
    for(int j=0;j<grid->ny()-1;j++){
        for(int i=0;i<grid->nx()-1;i++){
            /** calculated vectors of the 4 points and determin case
             *  p1--p2 -->i x  thesebounds
             *  | \  |
             *  p3--p4         nextbounds
             *  |
             * \|/
             *  j
             *  y
             *
             *
             **/
            FAHVector3 p1,p2,p3,p4;
            FAHVector3 p1prime,p2prime,p3prime,p4prime;


            p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
            p1prime = FAHVector3(&p1);
            p1prime[2]=0;


            p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
            p2prime = FAHVector3(&p2);
            p2prime[2]=0;


            p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
            p3prime = FAHVector3(&p3);
            p3prime[2]=0;


            p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());
            p4prime = FAHVector3(&p4);
            p4prime[2]=0;


            /// Generate top and bottom

            /** MAKE TRIANGLES
             *  p1--p2 --> x
             *  | \ |
             *  p3--p4
             *   |
             *  \|/
             *   Y
             *
             *  x Z
             *  STLs use right hand rule to determin normal
             **/
            /// Make Triangles  p1-p4-p3 and p2-p4-p1 which form the top
            addFacetToSTL(p1,p4,p3,mesh);
            addFacetToSTL(p2,p4,p1,mesh);

            /// Make Triangles p4'-p1'-p3' and p4'-p2'-p1' with Zs=0 to form bottom
            addFacetToSTL(p4prime,p1prime,p3prime,mesh);
            addFacetToSTL(p4prime,p2prime,p1prime,mesh);

            /// BOUNDARIES
            /// NEED CASSES FOR i=0,j=0,i=nx-1,j=ny-1 to make walls
            if ( (0==j) ){ // FRONT section
                /**
                 * p1--p2
                 *  | \ |
                 * p1'--p2'
                 *
                 * need facets p1-p1'-p2' ,   p2-p1-p2'
                **/
                addFacetToSTL(p1,p1prime,p2prime,mesh);
                addFacetToSTL(p2,p1,p2prime,mesh);
            }
            if ( ((grid->ny()-2)==j) ){//BACK
                /**
                 * p3--p4
                 *  | \ |
                 * p3'--p4'
                 *
                 * need facets p3-p3'-p4' ,   p4-p3-p4'
                **/
                addFacetToSTL(p3,p3prime,p4prime,mesh);
                addFacetToSTL(p4,p3,p4prime,mesh);
            }




            if (0==i){//LEFT
                /**
                 * p1--pb
                 *  | \ |
                 * p1'--pb'
                 *
                 * need facets p1-p1'-pb' ,   pb-p1'-pb'
                **/
                FAHVector3 pb, pbprime;
                pb=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
                pbprime = FAHVector3(&pb);
                pbprime[2]=0;

                addFacetToSTL(p1,p1prime,pbprime,mesh);
                addFacetToSTL(pb,p1,pbprime,mesh);
            }
            if (grid->nx()-2==i){//RIGHT
                /**
                 * p2--p4
                 *  | \ |
                 * p2'--p4'
                 *
                 * need facets p1-p1'-pb' ,   pb-p1-pb'
                **/
                addFacetToSTL(p4prime,p4,p2,mesh);
                addFacetToSTL(p2,p2prime,p4prime,mesh);
            }
        }
    }
    return mesh;
}

void addFacetWithNormal(FAHVector3 p1, FAHVector3 p2, FAHVector3 p3, STLMesh* mesh, bool zpositive){
    float thresh=0.05;
    if( p1.x<thresh || p2.x<thresh ||p3.x<thresh || p1.y<thresh || p2.y<thresh ||p3.y<thresh  ){
//        printf("\nERROR IN Pts");
//        printPoint(p1);
//        printPoint(p2);
//        printPoint(p3);
        return;
    }

    if (p1.z<thresh){p1.z=0;}
    if (p2.z<thresh){p2.z=0;}
    if (p3.z<thresh){p3.z=0;}

    FAHTriangle t1(p1,p2,p3);
    STLFacet s1;
    if(((t1.normal().z>0)&&zpositive)||((t1.normal().z<0)&&!zpositive)){

        s1.triangle=t1;
        s1.normal=t1.normal();
    }else{
        FAHTriangle t2(p1,p3,p2);
        s1.triangle=t2;
        s1.normal=t2.normal();
    }
    if(!s1.normal.isInvalid()){
        mesh->AddFacet(s1);
    }

}

void addFacetWithDirection(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh, FAHVector3 direction){
    float thresh=0.05;
    if( p1.x<thresh || p2.x<thresh ||p3.x<thresh || p1.y<thresh || p2.y<thresh ||p3.y<thresh  ){
//        printf("\nERROR IN Pts");
//        printPoint(p1);
//        printPoint(p2);
//        printPoint(p3);
        return;
    }

    if (p1.z<thresh){p1.z=0;}
    if (p2.z<thresh){p2.z=0;}
    if (p3.z<thresh){p3.z=0;}

    FAHTriangle t1(p1,p2,p3);
    STLFacet s1;
    bool used1=false;
    if( (t1.normal().dot(direction))<0){
        used1=true;
        s1.triangle=t1;
        s1.normal=t1.normal();
    }else{
        FAHTriangle t2(p1,p3,p2);
        s1.triangle=t2;
        s1.normal=t2.normal();
    }
    if(!s1.normal.isInvalid()){
        mesh->AddFacet(s1);
    }else{
//        printf("\nERROR IN NORMAL\t used:%s",(used1?"1":"2"));
//        printPoint(p1);
//        printPoint(p2);
//        printPoint(p3);
    }
}


void addBetweenTwoLoopsToSTL(STLMesh* mesh,
                    FAHLoopInXYPlane* OuterLoop,
                    FAHLoopInXYPlane* innerLoop)
{
    QMap<int, QVector<FAHVector3> >indexToOuterloop;


    for(int i=0;i<OuterLoop->points.size();i++){
        FAHVector3 outpt = OuterLoop->points.at(i);
        if (outpt.isInvalid()){continue;}
        int index = 0;
        float min_d=10000;

        for(int j=0;j<innerLoop->points.size();j++){
            float d = fabs(innerLoop->points.at(j).x-outpt.x)+
                      fabs(innerLoop->points.at(j).y-outpt.y);

            if(d<min_d){
                min_d = d;
                index = j;
            }
        }
        indexToOuterloop[index].append(outpt.copy());
    }


    //Find center of loop
    FAHVector3 cent(0,0,0);
    foreach(FAHVector3 pt, innerLoop->points){
        cent.x=cent.x+pt.x;
        cent.y=cent.y+pt.y;
    }
    cent.x=cent.x/innerLoop->points.size();
    cent.y=cent.y/innerLoop->points.size();


//    qDebug()<<"keys: "<<indexToOuterloop.keys().size();
    QList<int> indices = indexToOuterloop.keys();
    qSort(indices);
    for(int i=0; i<indices.size();i++){
        int k = indices.at(i%indices.size());
        int kp = indices.at( (i+1)%indices.size() );
//        if(kp==(k+1)){qDebug()<<k<<","<<kp;}
        if(kp<k){
            kp=kp+innerLoop->points.size()+1;
        }

        FAHVector3 inner = innerLoop->points.at(k);

        QVector<FAHVector3> pts = indexToOuterloop[k];
        QVector<FAHVector3> ptsp = indexToOuterloop[kp%innerLoop->points.size()];

        FAHVector3 direction(0,0,1);



        for(int j=0; j<pts.size()-1;j++){
            direction = pts.at(j)-cent;
            direction.normalize();
            addFacetWithDirection(inner,pts.at(j),
                                  pts.at( (j+1)%pts.size() ),
                                  mesh,direction);
        }
        addFacetWithDirection(inner,pts.last(),
                              ptsp.first(),
                              mesh,direction);

        for(int j=k;j<kp;j++){

            FAHVector3 p1 = innerLoop->points.at(j%innerLoop->points.size());
            FAHVector3 p2 = innerLoop->points.at( (j+1)%innerLoop->points.size());

            direction = cent-ptsp.first();
            direction.z=0;
            direction.normalize();

            addFacetWithDirection(p1,p2,
                                  ptsp.first(),
                                  mesh,direction);
        }
    }
    {
        QVector<FAHVector3> pts = indexToOuterloop[indices.last()];
        QVector<FAHVector3> ptsp = indexToOuterloop[0];

        FAHVector3 p1 = innerLoop->points.first();
        FAHVector3 p2;
        p2 = innerLoop->points.last();
        int n=0;
        while(n<10 && p2.isInvalid()){

            innerLoop->points.pop_back();
            p2= innerLoop->points.last();
            n++;
//            qDebug()<<"n:"<<n;
        }
        n=0;
        FAHVector3 outpt = pts.last();
//        qDebug()<<"pts: "<<pts.size();
        while(n<10 && outpt.isInvalid() && pts.size()>0){
            pts.pop_back();
            outpt = pts.last();
            n++;
//            qDebug()<<"n:"<<n;
//            printPoint(outpt);
        }

//        qDebug()<<"\n\n 0";
        addFacetWithDirection(p2,outpt,
                              ptsp.first(),
                              mesh,FAHVector3(1,1,1));
//        qDebug()<<"\n\n 1";
        addFacetWithDirection(p1,p2,
                              ptsp.first(),
                              mesh,FAHVector3(1,1,1));
    }

}


template <class T>
void addLoopToSTL(const FAHLoopInXYPlane& loop, XYGrid<T>* grid, STLMesh* mesh , bool inner){
    FAHLoopInXYPlane newloop;
    float ds = 10;

    for(int i=1;i<loop.points.size()+1;i++){
        FAHLine line(loop.points.at(i-1),loop.points.at(i%loop.points.size()));
        newloop.add(line.a);
        bool inseg = true;
        int j=0;
        while(inseg && (j<10000)){
            j++;
            FAHVector3 ab = (line.b.copy().sub(line.a));
            ab.normalize();
            ab*=j*ds;
            FAHVector3 pt= line.a.copy()+=ab;
            if(line.pointOnSegment(pt)){
                newloop.add(pt);
//                printf("\n added point");

            }
            else{
//                printf("\n\n Segment");
//                printPoint(line.a);
//                printPoint(line.b);
//                printPoint(pt);
                inseg=false;
            }
        }

    }


    // get point values
    QVector<FAHVector3> points(newloop.points.size());
    QVector<FAHVector3> primepoints(newloop.points.size());
    for(int k=0;k<points.size();k++){
        points[k]=newloop.points.at(k).copy();
        primepoints[k]=newloop.points.at(k).copy();
        primepoints[k].z=0;
        points[k].z=getHeightAt(points[k].x,points[k].y,grid);
    }


    // Add loop to STL
    for (int i = 1; i < points.size() + 1; i++) {
        FAHVector3 p1,p2,p1p,p2p;
        p1=points[i-1];
        p2=points[i%points.size()];
        p1p=primepoints[i-1];
        p2p=primepoints[i%points.size()];

        FAHTriangle t1(p1,p2,p2p);
        FAHVector3 testpt = p1.copy().add(t1.normal());
        bool contains = loop.pointInside(testpt);
        if(!inner){
            if(contains){
                addFacetToSTL(p2p,p2,p1,mesh);
                addFacetToSTL(p1,p1p,p2p,mesh);
            }else{
                addFacetToSTL(p1,p2,p2p,mesh);
                addFacetToSTL(p2p,p1p,p1,mesh);
            }
        }else{
            if(!contains){
                addFacetToSTL(p2p,p2,p1,mesh);
                addFacetToSTL(p1,p1p,p2p,mesh);
            }else{
                addFacetToSTL(p1,p2,p2p,mesh);
                addFacetToSTL(p2p,p1p,p1,mesh);
            }
        }

    }




}


template <class T>
STLMesh* makeSTLfromScanSection(XYGrid<T>* grid,FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops){
    STLMesh* mesh= new STLMesh();
    // Construct triangles for top and bottom


    for(int j=0;j<grid->ny()-1;j++){
        for(int i=0;i<grid->nx()-1;i++){
            /** calculated vOuterLoophe 4 points and determin case
             *  p1--p2 -->i x  thesebounds
             *  | \  |
             *  p3--p4         nextbounds
             *  |
             * \|/
             *  j
             *  y
             *
             *
             **/
            FAHVector3 p1,p2,p3,p4;
            FAHVector3 p1prime,p2prime,p3prime,p4prime;
            bool b1=true,b2=true,b3=true,b4=true;
            //bool l1=false, l2=false, l3=false, l4=false;
            int numInBounds=0;
            //int numOnLoop=0;

            p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
            p1prime = p1.xyprojection();//FAHVector3(&p1);
//            p1prime[2]=0;
            b1=loopsContain(p1,OuterLoop,innerLoops);
            //l1=onLoops(p1,OuterLoop,innerLoops);
            if(b1){numInBounds++;}
            //if(l1){numOnLoop++;}

            p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
            p2prime = p2.xyprojection();//FAHVector3(&p2);
//            p2prime[2]=0;
            b2=loopsContain(p2,OuterLoop,innerLoops);
            //l2=onLoops(p1,OuterLoop,innerLoops);
            if(b2){numInBounds++;}
            //if(l2){numOnLoop++;}


            p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
            p3prime = p3.xyprojection();//FAHVector3(&p3);
//            p3prime[2]=0;
            b3=loopsContain(p3,OuterLoop,innerLoops);
            //l3=onLoops(p1,OuterLoop,innerLoops);
            if(b3){numInBounds++;}
            //if(l3){numOnLoop++;}

            p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());
            p4prime = p4.xyprojection();//FAHVector3(&p4);
//            p4prime[2]=0;
            b4=loopsContain(p4,OuterLoop,innerLoops);
            //l4=onLoops(p4,OuterLoop,innerLoops);
            if(b4){numInBounds++;}
            //if(l4){numOnLoop++;}
//            printPoint(p4);
//            if(numInBounds>1){
//                qDebug()<<"\n\n"<<p4.x<<","<<p4.y<<":"<<numInBounds;
//            }

//            printf("\n\nGrid:%i,%i\tX:%.1f - %.1f\tY:%.1f-%.1f",
//                   i,j,p1.x,p4.x,p1.y,p4.y);

//            printf("\n Case:%i \tb1:%s\tb2:%s\tb3:%s\tb4:%s",numInBounds,
//                   (b1)?"true":"false",
//                   (b2)?"true":"false",
//                   (b3)?"true":"false",
//                   (b4)?"true":"false");


            /// THere are 5 cases 0,1,2,3,4 points are in bound
            /// for case 0 do nothing


            if(4==numInBounds){
                /// Make Triangles  p1-p4-p3 and p2-p4-p1 which form the top
                addFacetToSTL(p1,p4,p3,mesh);
                addFacetToSTL(p2,p4,p1,mesh);

                /// Make Triangles p4'-p1'-p3' and p4'-p2'-p1' with Zs=0 to form bottom
                addFacetToSTL(p4prime,p1prime,p3prime,mesh);
                addFacetToSTL(p4prime,p2prime,p1prime,mesh);
            }else if(1<=numInBounds){
                // All other cases
                bool fromOuterloop = false;
                FAHVector3 direction;


                QVector<FAHVector3> allpoints;
                QVector<FAHVector3> points;

                fromOuterloop= getPointsInGrid(i,j,grid->stepSize(),OuterLoop,innerLoops,&allpoints);
                QList<FAHVector3> gridpoints;
                gridpoints.append(p1);
                gridpoints.append(p2);
                gridpoints.append(p3);
                gridpoints.append(p4);
                for(int k=0;k<allpoints.size();k++){
                    allpoints[k].z=getHeightAt(allpoints[k].x,allpoints[k].y,p1,p2,p3,p4);
                }

                /// get rid of overlap

                removePointsIfInList(&allpoints,&points,gridpoints);

                QVector<FAHVector3> primepoints(points);
                for(int k=0;k<points.size();k++){
                    primepoints[k].z=0;
                }

                QVector<FAHVector3> allprimepoints(allpoints);
                for(int k=0;k<allpoints.size();k++){
                    allprimepoints[k].z=0;
                }

                if(2<=points.size()){
                    if(3==numInBounds){
                        FAHVector3 anchorPt, anchorPtPrime;
                        FAHVector3 leftPt, leftPtPrime;
                        FAHVector3 rightPt, rightPtPrime;
                        FAHVector3 leftBorderPt, leftBorderPtPrime;
                        FAHVector3 rightBorderPt, rightBorderPtPrime;

                        if(!b1){ // anchor to p4
                            direction = p4.copy().sub(p1);
                            anchorPt = p4;
                            anchorPtPrime=p4prime;
                            leftPt=p3;
                            leftPtPrime=p3prime;
                            rightPt=p2;
                            rightPtPrime=p2prime;
                            if(Math::floatsEqual(points.first().x,leftPt.x)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else if (Math::floatsEqual(points.last().x,leftPt.x)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }else if (!b2){
                            direction = p3.copy().sub(p2);
                            anchorPt = p3;
                            anchorPtPrime=p3prime;
                            leftPt=p1;
                            leftPtPrime=p1prime;
                            rightPt=p4;
                            rightPtPrime=p4prime;
                            if(Math::floatsEqual(points.first().y,leftPt.y)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else {//if (Math::floatsEqual(points.last().y,leftPt.y)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }else if (!b3){
                            direction = p2.copy().sub(p3);
                            anchorPt = p2;
                            anchorPtPrime=p2prime;
                            leftPt=p4;
                            leftPtPrime=p4prime;
                            rightPt=p1;
                            rightPtPrime=p1prime;
                            if(Math::floatsEqual(points.first().y,leftPt.y)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else {//if (Math::floatsEqual(points.last().y,leftPt.y)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }else if (!b4){
                            direction = p1.copy().sub(p4);
                            anchorPt = p1;
                            anchorPtPrime=p1prime;
                            leftPt=p2;
                            leftPtPrime=p2prime;
                            rightPt=p3;
                            rightPtPrime=p3prime;
                            if(Math::floatsEqual(points.first().x,leftPt.x)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else {//if (Math::floatsEqual(points.last().x,leftPt.x)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }
                        //ADD TOP AND BOTTOM
                        for(int k=0;k<points.size()-1;k++){
                            addFacetWithNormal(points[k+1],points[k],anchorPt,mesh,true);
                            addFacetWithNormal(anchorPtPrime,primepoints[k],primepoints[k+1],mesh,false);
                        }
                        addFacetWithNormal(leftBorderPt,anchorPt,leftPt,mesh,true);
                        addFacetWithNormal(leftPtPrime,anchorPtPrime,leftBorderPtPrime,mesh,false);

                        addFacetWithNormal(rightBorderPt,rightPt,anchorPt,mesh,true);
                        addFacetWithNormal(anchorPtPrime,rightPtPrime,rightBorderPtPrime,mesh,false);

//                        for(int k=0;k<points.size()-1;k++){
//                            addFacetWithDirection(points[k],points[k+1],primepoints[k],mesh,direction);
//                            addFacetWithDirection(points[k+1],primepoints[k+1],primepoints[k],mesh,direction);
//                        }
//                        addFacetWithDirection(leftPt,leftBorderPt,leftBorderPtPrime, mesh,direction);
//                        addFacetWithDirection(leftBorderPtPrime, leftPtPrime,leftPt, mesh,direction);

//                        addFacetWithDirection(rightPt,rightBorderPt, rightBorderPtPrime, mesh,direction);
//                        addFacetWithDirection(rightBorderPtPrime, rightPtPrime,rightPt, mesh,direction);


                    }else if(2==numInBounds){
                    /// 4 sub cases, left,right,top and bottom
                        FAHVector3 anchorPt, anchorPtPrime;
                        FAHVector3 farPt, farPtPrime;
                        FAHVector3 farPtBorder, farPtBorderPrime;

                        if(b2&&b4){// right
                            direction = p2.copy().sub(p1);
                            anchorPt=p4;
                            anchorPtPrime=p4prime;
                            farPt=p2;
                            farPtPrime=p2prime;
                            if(Math::floatsEqual(points.last().y,farPt.y)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }

                        }else if(b1&&b3){//left
                            direction = p1.copy().sub(p2);
                            anchorPt=p3;
                            anchorPtPrime=p3prime;
                            farPt=p1;
                            farPtPrime=p1prime;
                            if(Math::floatsEqual(points.last().y,farPt.y)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }

                        }else if(b1&&b2){//top
                            direction = p1.copy().sub(p3);
                            anchorPt=p1;
                            anchorPtPrime=p1prime;
                            farPt=p2;
                            farPtPrime=p2prime;
                            if(Math::floatsEqual(points.last().x,farPt.x)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }

                        }else if(b3&&b4){//bottom
                            direction = p3.copy().sub(p1);
                            anchorPt=p3;
                            anchorPtPrime=p3prime;
                            farPt=p4;
                            farPtPrime=p4prime;
                            if(Math::floatsEqual(points.last().x,farPt.x)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }
                        }
                        //STLs use right hand rule to determin normal

                        for(int k=0;k<points.size()-1;k++){
                            addFacetWithNormal(points[k+1],points[k],anchorPt,mesh,true);
                            addFacetWithNormal(anchorPtPrime,primepoints[k],primepoints[k+1],mesh,false);
                        }
                        addFacetWithNormal(anchorPt,farPt,farPtBorder,mesh,true);
                        addFacetWithNormal(farPtBorderPrime,farPtPrime,anchorPtPrime,mesh,false);


//                        for(int k=0;k<points.size()-1;k++){
//                            addFacetWithDirection(points[k],points[k+1],primepoints[k],mesh,direction);
//                            addFacetWithDirection(points[k+1],primepoints[k+1],primepoints[k],mesh,direction);
//                        }
                        //addFacetWithDirection(leftPt,leftBorderPt,leftBorderPtPrime, mesh,direction);
                        //addFacetWithDirection(leftBorderPtPrime, leftPtPrime,leftPt, mesh,direction);

                        //addFacetWithDirection(rightPt,rightBorderPt, rightBorderPtPrime, mesh,direction);
                        //addFacetWithDirection(rightBorderPtPrime, rightPtPrime,rightPt, mesh,direction);




                    }else if(1==numInBounds){
                        FAHVector3 anchorPt, anchorPtPrime;

                        if(b1){
                            direction = p1.copy().sub(p4);
                            anchorPt=p1;
                            anchorPtPrime=p1prime;
                        }else if(b2){
                            direction = p2.copy().sub(p3);
                            anchorPt=p2;
                            anchorPtPrime=p2prime;
                        }else if(b3){
                            direction = p3.copy().sub(p2);
                            anchorPt=p3;
                            anchorPtPrime=p3prime;
                        }else if(b4){
                            direction = p4.copy().sub(p1);
                            anchorPt=p4;
                            anchorPtPrime=p4prime;
                        }

                        for(int k=0;k<points.size()-1;k++){
                            addFacetWithNormal(points[k+1],points[k],anchorPt,mesh,true);
                            addFacetWithNormal(anchorPtPrime,primepoints[k],primepoints[k+1],mesh,false);
                        }




                    }

                    for(int k=0;k<points.size()-1;k++){
                        addFacetWithDirection(points[k],points[k+1],primepoints[k],mesh,direction);
                        addFacetWithDirection(points[k+1],primepoints[k+1],primepoints[k],mesh,direction);
                    }

                    // ADD LOOP POINTS
//                    for(int k=1; k<allpoints.size(); k++){
//                        addFacetWithDirection(allpoints[k-1],allpoints[k],allprimepoints[k-1],mesh, direction);
//                        addFacetWithDirection(allpoints[k],allprimepoints[k-1],allprimepoints[k],mesh,direction);
//                    }


                }else{

//                    printf("\n found %i points",points.size());
                    for(int k=0;k<allpoints.size()-1;k++){
                        direction.operator *=(-1);
                        addFacetWithDirection(allpoints[k],allpoints[k+1],allprimepoints[k],mesh,direction);
                        addFacetWithDirection(allpoints[k+1],allprimepoints[k+1],allprimepoints[k],mesh,direction);
                    }

                }
            }
//            if(1<numOnLoop){
//                FAHVector3 direction;
//                qDebug()<<"onloop at "<<i<<","<<j ;
//                if(l1&&l2){//top
//                    direction = p1.copy().sub(p3);
//                    addFacetWithDirection(p1,p2,p2prime,mesh,direction);
//                    addFacetWithDirection(p2prime,p1prime,p1,mesh,direction);

//                }else if(l2&&l4){//right
//                    direction = p2.copy().sub(p1);
//                    addFacetWithDirection(p2,p4,p4prime,mesh,direction);
//                    addFacetWithDirection(p4prime,p2prime,p2,mesh,direction);
//                }else if(l1&&l3){//left
//                    direction = p1.copy().sub(p2);
//                    addFacetWithDirection(p1,p3,p3prime,mesh,direction);
//                    addFacetWithDirection(p3prime,p1prime,p1,mesh,direction);
//                }else if(l3&&l4){//bottom
//                    direction = p3.copy().sub(p1);
//                    addFacetWithDirection(p3,p4,p4prime,mesh,direction);
//                    addFacetWithDirection(p4prime,p3prime,p3,mesh,direction);
//                }
//            }
        }
    }




    // Construct triangles for the borders.

    //addLoopToSTL(OuterLoop,grid,mesh,false);
    //for(int i=0;i<innerLoops.size();i++){
    //    addLoopToSTL(innerLoops.at(i),grid,mesh,true);
    //}

    return mesh;
}



void addFacetToSTL(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh){
    /// need to make sure that no 2 vectors are the same or it will not define a plane

        FAHTriangle t1(p1,p2,p3);
        STLFacet s1;
        s1.triangle=t1;
        s1.normal=t1.normal();
        if(!s1.normal.isInvalid()){
            mesh->AddFacet(s1);
       }

}

bool stlToFile(STLMesh* m, QString file){
    STLFile stl;
    stl.SetMesh(m);
    return stl.WriteASCII(file);
}


template void addSquareToSTL(int i, int j, XYGrid<float>* grid,STLMesh* mesh, FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops, bool top);
template void addSquareToSTL(int i, int j, XYGrid<int>* grid,STLMesh* mesh, FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops, bool top);
template STLMesh* makeSTLfromScan(XYGrid<float>* grid );
template STLMesh* makeSTLfromScan(XYGrid<int>* grid );
template STLMesh* makeSTLfromScanSection(XYGrid<float>* grid, FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops);
template STLMesh* makeSTLfromScanSection(XYGrid<int>* grid, FAHLoopInXYPlane* OuterLoop, QList<FAHLoopInXYPlane*> innerLoops);
template void addLoopToSTL(const FAHLoopInXYPlane& loop,XYGrid<float>* grid,STLMesh* mesh, bool inner );
template void addLoopToSTL(const FAHLoopInXYPlane& loop,XYGrid<int>* grid,STLMesh* mesh, bool inner );
