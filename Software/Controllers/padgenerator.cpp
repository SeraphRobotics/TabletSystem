#include "padgenerator.h"
#include "UnitTest/debugfunctions.h"
#include "processingfunctions.h"
#include <math.h>

float GeneratePad(Manipulation* m, XYGrid<float>* pad_grid, XYGrid<float>* shell_grid, STLMesh* mesh, STLMesh *shell_mesh, float min_z){
//    STLMesh* mesh= new STLMesh();

    float kMinThick = 0.4;

//    qDebug()<< m->depth;
//    qDebug()<< m->innerloops.size();
//    qDebug()<< m->location.x;
//    qDebug()<<m->outerloop->points.size();
//    qDebug()<< m->stiffness;
//    qDebug()<< m->thickness;

    ////// Find maximum distance
    QList<ijd> modpts;
    float max_d = -10000;
    min_z = 10000;

    for(int j=0;j<pad_grid->ny()-1;j++){
        for(int i=0;i<pad_grid->nx()-1;i++){
            FAHVector3 p1=vectorFromIJ(i,j,pad_grid->at(i,j),pad_grid->stepSize());
            if ( loopsContain(p1,m->outerloop,m->innerloops) ){
                float d = m->outerloop->distanceToPoint(p1);
                ijd pt;
                pt.i=i;
                pt.j=j;
                pt.d=d;
                modpts.append(pt);
                max_d = std::max(max_d,d);
                float z = pad_grid->at(i,j);
                min_z = std::min(min_z,z);
            }
        }
    }
    qDebug()<<"Max D: "<<max_d;
    qDebug()<<"Min Z: "<<min_z;

    ///ADJUST Heights of pad and shell
    float heightOverShell = m->thickness - m->depth;
    qDebug()<<"height: "<<heightOverShell;
    qDebug()<<"Npts:"<<modpts.size();

    float floorz = min_z - m->depth-kMinThick;

    foreach(ijd pt, modpts){
//        printPoint(pt);
        float delta =heightOverShell*pt.d/max_d;
        if(delta>heightOverShell){
            qDebug()<<"\nerror at "<<pt.i<<","<<pt.j<<"with "<<delta;
        }
        float h = pad_grid->at(pt.i,pt.j);
        pad_grid->operator ()(pt.i,pt.j)=  h+delta;
//        qDebug()<<heightOverShell/max_d*pt[2];
        ///  delta_h = height over shell / max distance * current distance
        shell_grid->operator ()(pt.i,pt.j) = h+delta;
//        shell_grid->operator ()(pt.i,pt.j) = floorz;
        /// Set shell Z to depth


    }

    blurInLoop(pad_grid,m->outerloop,3);
    blurInLoop(shell_grid,m->outerloop,3);

    //// MAP loops to Heights
    FAHLoopInXYPlane* outer = mapOntoGrid(m->outerloop,pad_grid);
    delete m->outerloop;
    m->outerloop = outer;
    QList<FAHLoopInXYPlane*> inners;
    foreach(FAHLoopInXYPlane* loop,m->innerloops){
        inners.append(mapOntoGrid(loop,pad_grid,true) );
    }


    // ADD grid
    for(int j=0;j<pad_grid->ny()-1;j++){
        for(int i=0;i<pad_grid->nx()-1;i++){

            addSquareToSTL(i,j,pad_grid, mesh, outer, inners,-1);

            addSquareToSTL(i,j,pad_grid, mesh, outer, inners,floorz,false);
            addSquareToSTL(i,j,pad_grid,shell_mesh, outer, inners,floorz,true);
//            addSquareToSTL(i,j,pad_grid,shell_mesh, outer, inners,0,false);
            addSquareToSTL(i,j,pad_grid,shell_mesh, outer, inners,5,false,true);
        }
    }
    FAHLoopInXYPlane* outer_floor = mapOntoGrid(m->outerloop,shell_grid,true,floorz);
    addBetweenTwoLoopsToSTL(mesh,outer,outer_floor);
    addLoopAtHeight(shell_mesh,outer,floorz,false,false);
    delete outer_floor;
//    delete outer;
;

    foreach(FAHLoopInXYPlane* l, inners){
        FAHLoopInXYPlane* l2 = mapOntoGrid(l,shell_grid,true,floorz);
        addBetweenTwoLoopsToSTL(mesh,l,l2);
        delete l2;
        delete l;
    }


//    QFile f("topcoat.csv");
//    if(f.open(QIODevice::WriteOnly)){
//        QTextStream s(&f);
//        s<<pad_grid->toCSV();
//        f.close();
//    }

//    QFile f2("base.csv");
//    if(f2.open(QIODevice::WriteOnly)){
//        QTextStream s(&f2);
//        s<<shell_grid->toCSV();
//        f2.close();
//    }

    return floorz;
}
