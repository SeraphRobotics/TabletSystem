#include "manipulations.h"
#include <QDomDocument>

Manipulation::Manipulation():depth(0),stiffness(0),location(0,0,0){

}

Manipulation::Manipulation(QDomNode node):depth(0),stiffness(0),location(0,0,0){
    /**
        <manipulation>
            <type>12</type>
            <stiffness units="MPA">12312</stiffness>
            <depth units="mm">10</depth>
            <location>
                <x>1</x>
                <y>1</y>
            </location>
            <outerloop>
                <loop>....</loop>
            </outerloop>
            <innerloops>
                <loop>....</loop>
                <loop>....</loop>
                <loop>....</loop>
            </innerloops>
        </manipulation>
    **/
    if(!("manipulation"==node.nodeName().toLower())){return;}


    QDomNodeList mchildren = node.childNodes();
    for(int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName();

        if("type"==name){
            type= static_cast<ManipulationType>(el.text().toUInt());
        }else if ("stiffness"==name){
            stiffness = el.text().toFloat();
        }else if ("depth"==name){
            depth = el.text().toFloat();
        }else if ("location"==name){
            QDomNodeList lchildren = mchild.childNodes();
            if(1>lchildren.length()){continue;}

            for(int j=0;j<lchildren.length();j++){

                QDomNode lchild = lchildren.at(j);
                if(!lchild.isElement()){continue;}
                QDomElement lel = lchild.toElement();

                if("x"==lel.nodeName().toLower()){
                    location.x=lel.text().toFloat();
                }else if("y"==lel.nodeName().toLower()){
                    location.y=lel.text().toFloat();
                }else if("z"==lel.nodeName().toLower()){
                    location.z=lel.text().toFloat();
                }
            }

        }else if ("outerloop"==name){
            QDomNode loop = mchild.namedItem("loop");
            outerloop=loopFromNode(loop);

        }else if ("innerloops"==name){
            QDomNodeList iloops = mchild.childNodes();
            for(int k=0;k<iloops.size();k++){
                QDomNode loop = iloops.at(k);
                FAHLoopInXYPlane* l = loopFromNode(loop);
                if(l->points.size()>2){innerloops.append(l);}
            }
        }
    }

}

QDomNode Manipulation::toNode(){
    QDomDocument d("dummy");

    QDomElement node = d.createElement("Manipulation");
    QDomElement typeEl = d.createElement("type");
    typeEl.appendChild(d.createTextNode(QString::number(type)));
    node.appendChild(typeEl);

    QDomElement stiffEl = d.createElement("stiffness");
    stiffEl.appendChild(d.createTextNode(QString::number(stiffness)));
    node.appendChild(stiffEl);

    QDomElement depthEl = d.createElement("depth");
    depthEl.appendChild(d.createTextNode(QString::number(depth)));
    node.appendChild(depthEl);

    QDomElement locationEl = d.createElement("location");
    QDomElement xEl = d.createElement("x");
    QDomElement yEl = d.createElement("y");
    xEl.appendChild(d.createTextNode(QString::number(location.x)));
    yEl.appendChild(d.createTextNode(QString::number(location.y)));
    locationEl.appendChild(xEl);
    locationEl.appendChild(yEl);
    node.appendChild(locationEl);


    QDomElement outerEl = d.createElement("outerloop");
    QDomNode ol=nodeFromLoop(outerloop);
    outerEl.appendChild(ol);
    node.appendChild(outerEl);

    QDomElement innerEl = d.createElement("innerloops");
    for(int i=0; i<innerloops.size();i++){
        innerEl.appendChild(nodeFromLoop(innerloops.at(i)));
    }
    node.appendChild(innerEl);

    return node;
}


FAHLoopInXYPlane* loopFromNode(QDomNode node){
    if(!("loop"==node.nodeName())){return new FAHLoopInXYPlane();}

    FAHLoopInXYPlane* loop = new FAHLoopInXYPlane();

    QDomNodeList pathChildren = node.childNodes();
    QDomNodeList pointChildren;
    QDomElement pchild;
    QDomElement coordNode;
    FAHVector3 p;
    for (int i=0;i < pathChildren.size(); i++) {
        pchild = pathChildren.at(i).toElement();
        if (pchild.isComment()) {continue;}

        if ("point"==pchild.nodeName().toLower()) {
            p.x=0;
            p.y=0;
            p.z=0;
            pointChildren = pchild.childNodes();

            for(int j=0; j < pointChildren.size(); j++) {
                if (pointChildren.at(j).isComment()) {continue;}
                coordNode = pointChildren.at(j).toElement();

                QString temp = coordNode.nodeName().toLower();
                double tempd = coordNode.text().toDouble();

                if ("x" == temp) {
                    p.x = tempd;
                } else if ("y" == temp) {
                    p.y = tempd;
                } else if ("z" == temp) {
                    p.z = tempd;
                }
            }
            loop->add(p);
        }
    }
    return loop;

}
QDomNode nodeFromLoop(FAHLoopInXYPlane* loop){
    QDomDocument d("dummy");
    QDomElement loopEl=d.createElement("loop");

    for(int i=0; i<loop->points.size();i++){
        QDomElement pointEl = d.createElement("point");
        QDomElement xEl = d.createElement("x");
        QDomElement yEl = d.createElement("y");
//        QDomElement zEl = d.createElement("z");
        FAHVector3 p = loop->points.at(i);

        xEl.appendChild(d.createTextNode(QString::number(p.x)));
        yEl.appendChild(d.createTextNode(QString::number(p.y)));
//        zEl.appendChild(d.createTextNode(QString::number(p.z)));

        pointEl.appendChild(xEl);
        pointEl.appendChild(yEl);
//        pointEl.appendChild(zEl);
        loopEl.appendChild(pointEl);
    }
    return loopEl;
}
