// qt
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

// local
#include "globals.h"

void printPoint(FAHVector3 pt){
    printf("\n %.4f\t%.4f\t%.4f",pt.x,pt.y,pt.z);
}

void writeLoopToXDFL(FAHLoopInXYPlane *loop, QString file){
     QDomDocument document;
     QDomElement root = document.createElement("xdfl");
     document.appendChild(root);

     /// ADD palette
     QDomElement palette = document.createElement("palette");
     root.appendChild(palette);
     QDomElement name = document.createElement("name");
     name.appendChild(document.createTextNode("test"));
     palette.appendChild(name);
     QDomElement id = document.createElement("id");
     id.appendChild(document.createTextNode("1"));
     palette.appendChild(id);

     QDomElement cmds = document.createElement("commands");
     QDomElement path = document.createElement("path");
     QDomElement matId = document.createElement("materialid");
     matId.appendChild(document.createTextNode("1"));
     path.appendChild(matId);
     for(int i=0;i<loop->points.size();i++){
         FAHVector3 pt = loop->points.at(i);

         QDomElement point = document.createElement("point");
         QDomElement xEl = document.createElement("x");
         xEl.appendChild(document.createTextNode(QString::number(pt.x)));
         QDomElement yEl = document.createElement("y");
         yEl.appendChild(document.createTextNode(QString::number(pt.y)));
         QDomElement zEl = document.createElement("z");
         zEl.appendChild(document.createTextNode(QString::number(pt.z)));

         point.appendChild(xEl);
         point.appendChild(yEl);
         point.appendChild(zEl);
         path.appendChild(point);

     }
     FAHVector3 pt = loop->points.at(0);

     QDomElement point = document.createElement("point");
     QDomElement xEl = document.createElement("x");
     xEl.appendChild(document.createTextNode(QString::number(pt.x)));
     QDomElement yEl = document.createElement("y");
     yEl.appendChild(document.createTextNode(QString::number(pt.y)));
     QDomElement zEl = document.createElement("z");
     zEl.appendChild(document.createTextNode(QString::number(pt.z)));

     point.appendChild(xEl);
     point.appendChild(yEl);
     point.appendChild(zEl);
     path.appendChild(point);
     cmds.appendChild(path);
     root.appendChild(cmds);



     QFile f(file);
     if (f.open(QFile::WriteOnly)) {
         QTextStream out(&f);
         out<<document.toString();
         f.close();
     }

}

FAHLoopInXYPlane* circle(float x, float y,float r){
    FAHLoopInXYPlane* loop = new FAHLoopInXYPlane();
    for( int i=0; i<100; i++){
        float theta = i*Math::k2Pi/100.0;
        FAHVector3 pt(0.5*r*sin(theta)+x,r*cos(theta)+y,0);
        loop->add(pt);
    }
    return loop;
}
