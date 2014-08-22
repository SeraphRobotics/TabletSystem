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
    for(unsigned int i=0;i<mchildren.length();i++){
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

            for(unsigned int j=0;j<lchildren.length();j++){

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

QDomNode nodeFromTopCoat(Top_Coat tc){
    QDomDocument d("dummy");

    QDomElement node = d.createElement("TopCoat");
    QDomElement typeEl = d.createElement("type");
    typeEl.appendChild(d.createTextNode(QString::number(tc.style)));
    node.appendChild(typeEl);

    QDomElement stiffEl = d.createElement("density");
    stiffEl.appendChild(d.createTextNode(QString::number(tc.density)));
    node.appendChild(stiffEl);

    QDomElement depthEl = d.createElement("depth");
    depthEl.appendChild(d.createTextNode(QString::number(tc.depth)));
    node.appendChild(depthEl);

    QDomElement thicknessEl = d.createElement("thickness");
    thicknessEl.appendChild(d.createTextNode(QString::number(tc.thickness)));
    node.appendChild(thicknessEl);

    return node;
}



Top_Coat topCoatFromNode(QDomNode node){
    Top_Coat tc;
    if(!("topcoat"==node.nodeName().toLower())){return tc;}


    QDomNodeList mchildren = node.childNodes();
    for(unsigned int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName();

        if("type"==name){
            tc.style= static_cast<Top_Coat::Style>(el.text().toUInt());
        }else if ("density"==name){
            tc.density = static_cast<Top_Coat::Density>(el.text().toUInt());
        }else if("depth" == name){
            tc.depth = el.text().toFloat();
        }else if("thickness"==name){
            tc.thickness = el.text().toFloat();
        }
    }
    return tc;
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
            p=pointFromNode(pchild);
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


QDomNode postingToNode(Posting p){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("posting");

    QDomElement sideEl = d.createElement("side");
    sideEl.appendChild(d.createTextNode(QString::number(p.for_rear)));
    node.appendChild(sideEl);

    QDomElement angleEl = d.createElement("angle");
    angleEl.appendChild(d.createTextNode(QString::number(p.angle)));
    node.appendChild(angleEl);

    QDomElement verticleEl = d.createElement("verticle");
    verticleEl.appendChild(d.createTextNode(QString::number(p.verticle)));
    node.appendChild(verticleEl);

    QDomElement directionEl = d.createElement("direction");
    directionEl.appendChild(d.createTextNode(QString::number(p.varus_valgus)));
    node.appendChild(directionEl);

    return node;
}

Posting nodeToPosting(QDomNode node){
    Posting p;
    p.angle=0;
    p.varus_valgus=Posting::kValgus;
    p.for_rear=Posting::kForFoot;
    p.verticle=0;

    if(!("posting"==node.nodeName().toLower())){return p;}


    QDomNodeList mchildren = node.childNodes();
    for(unsigned int i=0;i<mchildren.length();i++){
        QDomNode mchild = mchildren.at(i);
        if(!mchild.isElement()){continue;}

        QDomElement el = mchild.toElement();
        QString name = mchild.nodeName();

        if("side"==name){
            p.for_rear= static_cast<Posting::side>(el.text().toUInt());
        }else if("angle"==name){
            p.angle=el.text().toFloat();
        }else if("verticle"==name){
            p.verticle=el.text().toFloat();
        }else if("direction"==name){
            p.varus_valgus=static_cast<Posting::direction>(el.text().toUInt());
        }
    }
    return p;
}


Posting::Posting(){
    angle=0;
    verticle=0;
    varus_valgus = Posting::kValgus;
    for_rear = Posting::kForFoot;
}


FAHVector3 pointFromNode(QDomNode node){
    FAHVector3 p(0,0,0);
    if ("point"==node.nodeName().toLower()) {

        QDomNodeList pointChildren = node.childNodes();
        for(int j=0; j < pointChildren.size(); j++) {
            if (pointChildren.at(j).isComment()) {continue;}
            QDomElement coordNode = pointChildren.at(j).toElement();

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
    }
    return p;
}


QDomNode nodeFromPoint(FAHVector3 p){
    QDomDocument d("dummy");
    QDomElement pointEl = d.createElement("point");
    QDomElement xEl = d.createElement("x");
    QDomElement yEl = d.createElement("y");
    QDomElement zEl = d.createElement("z");

    xEl.appendChild(d.createTextNode(QString::number(p.x)));
    yEl.appendChild(d.createTextNode(QString::number(p.y)));
    zEl.appendChild(d.createTextNode(QString::number(p.z)));

    pointEl.appendChild(xEl);
    pointEl.appendChild(yEl);
    pointEl.appendChild(zEl);
    return pointEl;
}

QVector< FAHVector3 > pointsFromNode(QDomNodeList nodes){
    QVector< FAHVector3 > points;
    for(int i=0;i<nodes.size();i++){
        QDomNode n = nodes.at(i);
        QDomElement el = n.toElement();
        QString name = n.nodeName();
        if("point"==name){
            points.append(pointFromNode(el));
        }
    }
    return points;
}

QVector< QDomNode> nodeListFromVector( QVector <FAHVector3> points){
    QVector< QDomNode> nodes;
    for(int i=0;i<points.size();i++){
        nodes.append( nodeFromPoint( points.at(i) ) );
    }
    return nodes;
}
