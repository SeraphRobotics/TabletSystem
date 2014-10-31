#include "topcoatcontroller.h"
#include <math.h>
#include "scannerfunctions.h"
#include <QSettings>
#include "UnitTest/debugfunctions.h"

TopCoatController::TopCoatController(Orthotic* orth, QString dir, QObject *parent):
    QObject(parent),dir_(dir)
{
    orth_=orth;
}


void TopCoatController::generateTopCoat(){
    Top_Coat t =  orth_->getTopCoat();
    if(t.style!=Top_Coat::kAuto){
        emit generatedCoatingFile("");
        return;
    }

    QVector< FAHVector3> pts;
    QSettings s;
    XYGrid<float>* grid = orth_->getScan()->getPostedXYGrid();
    float scaley = 1.0*grid->stepSize();
    float scalex = 2.0;
    float path_width=1.0;
    float path_height=1.0;
    float z_offset=1.0;
    float speed = 10.0;

    FAHLoopInXYPlane* outerLoop = mapOntoGrid( orth_->getLoop(), grid,false);
    FAHVector3 minp = outerLoop->min();
    minp.x = minp.x*scalex;
    minp.y = minp.y*scaley;
    QList<FAHLoopInXYPlane* > innerLoops;

    printPoint(minp);
    //    enum Style{ kNone,kAuto,kCloth};
    //   enum Density {kLow,kMedium,kHigh};



    if(t.density == Top_Coat::kLow){
        path_width = s.value("printing/topcoat/low/pw","1.0").toFloat();
        path_height = s.value("printing/topcoat/low/ph","1.0").toFloat();
        z_offset = s.value("printing/topcoat/low/zo","1.0").toFloat();
        speed =  s.value("printing/topcoat/low/ps","10").toFloat();
    }else if(t.density == Top_Coat::kMedium){
        path_width = s.value("printing/topcoat/medium/pw","1.0").toFloat();
        path_height = s.value("printing/topcoat/medium/ph","1.0").toFloat();
        z_offset = s.value("printing/topcoat/medium/zo","1.0").toFloat();
        speed =  s.value("printing/topcoat/medium/ps","10").toFloat();
    }else if(t.density == Top_Coat::kHigh){
        path_width = s.value("printing/topcoat/high/pw","1.0").toFloat();
        path_height = s.value("printing/topcoat/high/ph","1.0").toFloat();
        z_offset = s.value("printing/topcoat/high/zo","1.0").toFloat();
        speed =  s.value("printing/topcoat/high/ps","10").toFloat();
    }


    QVector<float> xs;
    float x=0;
    float stop=grid->nx()*scalex;
    int n=0;
    while(x<stop && n<1000){
        xs.append(x);
        x+=path_width;
        n++;
    }


    int j=0;
    foreach(float x,xs){
        j++;

        for(int i=0; i<grid->ny();i++){

            ///Oscilate directions
            int index = i;
            if(j%2==0){index = i;}
            else{index = grid->ny()-i;}





            //Calculate hights
            int xjminus = floor(x/scalex);
            float xminus = xjminus;
            float zminus = grid->at(xjminus,index);

            int xjplus = ceil(x/scalex);
            float xplus = xjplus;
            float zplus = grid->at(xjplus,index);

            float m;
            if( (xplus-xminus)>0.1 && (zplus-zminus)>0.1 ){
                m = (zplus-zminus)/(xplus-xminus);
            }else{
                m = 0.0;
            }
//            float b = zplus-m*xplus;

//            pt.z = m*x+b + z_offset;
             FAHVector3 pt(xplus,index,0);
             pt.z = zplus + z_offset;

            //append if inside outer border
            if (loopsContain(pt,outerLoop,innerLoops) ){
                pt.x = x - minp.x+6;
                pt.y = pt.y*scaley + 10;
                pts.append(pt);
//                qDebug()<<"Z: "<< pt.z;
            }
        }
    }



    //// GENERATE GCODE FOR THE PATTERN
    QStringList gcodes;
    speed = 60*speed;
    int numlayers = floor(t.thickness/path_height);

    for(int layer=0;layer<numlayers;layer++){
        qDebug()<<"Layer: "<<layer;
        QString openLine = s.value("Printing/open","\nG4 P2\nM340 P0 S2100").toString();
        QString closeLine = s.value("Printing/close","\nG4 P2\nM340 P0 S1650").toString();
        QString relativelift = s.value("Printing/lift","\nG91 \nG1 Z40 F2400 \nG90").toString();

        FAHVector3 first = pts.first();
        QString first_pt_line = "G1 X"+QString::number(first.x)+
                           " Y"+QString::number(first.y)+
                           " Z"+QString::number(first.z+layer*path_height)+
                           " F"+QString::number(speed);

        gcodes<<relativelift;
        gcodes<<first_pt_line;
        gcodes<<openLine;
        foreach(FAHVector3 pt,pts){
            QString line = "G1 X"+QString::number(pt.x)+
                               " Y"+QString::number(pt.y)+
                               " Z"+QString::number(pt.z+layer*path_height)+
                               " F"+QString::number(speed);
            gcodes.append(line);
        }
        gcodes<<closeLine;
    }
    QString filename = "topcoat.gcode";
    QFile f(filename);
    if(!f.open(QFile::WriteOnly | QFile::Text)){
        return;
    }
    QTextStream fs(&f);
    for (int line = 0; line < gcodes.size(); ++line){
      fs << gcodes.at(line) << '\n';
    }
    f.close();
    emit generatedCoatingFile(filename);
}
