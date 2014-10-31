#include "topcoatcontroller.h"
#include <math.h>
#include "scannerfunctions.h"
#include <QSettings>

TopCoatController::TopCoatController(Orthotic* orth, QString dir, QObject *parent):
    QObject(parent),dir_(dir)
{
    orth_=orth;
}


void TopCoatController::generateTopCoat(){
    Top_Coat t =  orth_->getTopCoat();
    if(t.style!=Top_Coat::kAuto){return;}

    QVector< FAHVector3> pts;
    QSettings s;
    XYGrid<float>* grid = orth_->getScan()->getPostedXYGrid();
    float scaley = grid->stepSize();
    float scalex = 1.0;
    float path_width=1.0;
    float path_height=1.0;
    float z_offset=1.0;
    float speed = 10.0;

    FAHLoopInXYPlane* outerLoop = orth_->getLoop();
    QList<FAHLoopInXYPlane* > innerLoops;

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
    float stop=grid->nx();
    int n=0;
    while(x<stop && n<1000){
        xs.append(x);
        xs+=path_width;
        n++;
    }

    int j=0;
    foreach(float x,xs){
        j++;
        for(int i=0; i<grid->ny();i++){

            ///Oscilate directions
            int index;
            if(j%2==0){index = i;}
            else{index = grid->ny()-i;}



            FAHVector3 pt(x,i*scaley,0);

            //Calculate hights
            int xjminus = floor(x/scalex);
            float xminus = scalex*xjminus;
            float zminus = grid->at(xjminus,index);

            int xjplus = ceil(x/scalex);
            float xplus = scalex*xjplus;
            float zplus = grid->at(xjplus,index);

            float m = (zplus-zminus)/(xplus-xminus);
            float b = zplus-m*xplus;

            pt.z = m*x+b + z_offset;

            //append if inside outer border
            if (loopsContain(pt,outerLoop,innerLoops)){
                pts.append(pt);
            }
        }
    }



    //// GENERATE GCODE FOR THE PATTERN
    QStringList gcodes;
    speed = 60*speed;
    int numlayers = floor(t.depth/path_height);

    for(int layer=0;layer<numlayers;layer++){
        QString openLine = s.value("Printing/open","\nG4 P2\nM340 P0 S2100").toString();
        QString closeLine = s.value("Printing/close","\nG4 P2\nM340 P0 S1650\n").toString();
        QString relativelift = s.value("Printing/lift","\nG91 \nG1 Z20 F2400 \nG90 \n").toString();

        FAHVector3 first = pts.first();
        QString first_pt_line = "\nG1 X"+QString::number(first.x)+
                           " Y"+QString::number(first.y)+
                           " Z"+QString::number(first.z+layer*path_height)+
                            "F"+QString::number(speed);

        gcodes<<relativelift;
        gcodes<<first_pt_line;
        gcodes<<openLine;
        foreach(FAHVector3 pt,pts){
            QString line = "\nG1 X"+QString::number(pt.x)+
                               " Y"+QString::number(pt.y)+
                               " Z"+QString::number(pt.z+layer*path_height)+
                                "F"+QString::number(speed);
            gcodes.append(line);
        }
        gcodes<<closeLine;
    }
    QString filename = dir_+"\\"+"topcoat.gcode";
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
