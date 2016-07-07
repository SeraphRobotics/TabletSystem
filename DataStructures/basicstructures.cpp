#include "basicstructures.h"

Name::Name(){
    first="";
    last="";
    title="";
}
Name::Name(QString t,QString f, QString l){
    first=f;
    last=l;
    title=t;
}

QString Name::toQString(){
    QString s = title+QString(" ")+first+QString(" ")+last;
    return s;
}

Name nameFromNode(QDomNode node){
    Name n;
    if(!("name"==node.nodeName().toLower())){return n;}

    QDomNodeList nchildren = node.childNodes();
    for(int i=0; i < nchildren.length(); i++){
        QDomNode nchild = nchildren.at(i);
        if(!nchild.isElement()){continue;}

        QDomElement el = nchild.toElement();
        QString name = nchild.nodeName().toLower();

        if("title"==name){
            n.title = el.text();
        }else if("first"==name){
            n.first = el.text();
        }else if("last"==name){
            n.last = el.text();
        }
    }
    return n;
}

QDomNode nodeFromName(Name n){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("name");

    QDomElement title = d.createElement("title");
    title.appendChild(d.createTextNode(n.title));
    node.appendChild(title);

    QDomElement first = d.createElement("first");
    first.appendChild(d.createTextNode(n.first));
    node.appendChild(first);

    QDomElement last = d.createElement("last");
    last.appendChild(d.createTextNode(n.last));
    node.appendChild(last);

    return node;
}

QDomNode nodeFromDate(QDate date){
    QDomDocument d("dummy");
    QDomElement node = d.createElement("date");

    QDomElement day = d.createElement("day");
    day.appendChild(d.createTextNode(QString::number(date.day())));
    node.appendChild(day);

    QDomElement month = d.createElement("month");
    month.appendChild(d.createTextNode(QString::number(date.month())));
    node.appendChild(month);

    QDomElement year = d.createElement("year");
    year.appendChild(d.createTextNode(QString::number(date.year())));
    node.appendChild(year);

    return node;
}

QDate dateFromNode(QDomNode node){
    int month=0,day=0,year=0;
    if(!("date"==node.nodeName().toLower())){return QDate();}

    QDomNodeList dchildren = node.childNodes();
    for(int i=0;i<dchildren.length();i++){
        QDomNode dchild = dchildren.at(i);
        if(!dchild.isElement()){continue;}

        QDomElement el = dchild.toElement();
        QString name = dchild.nodeName().toLower();

        if("day"==name){
            day = el.text().toUInt();
        }else if("month"==name){
            month = el.text().toUInt();
        }else if ("year"){
            year = el.text().toUInt();
        }
    }

    if((0==month)||(0==year)||(0==day)){
        return QDate();
    }

    QDate d(year,month,day);
    return d;
}
