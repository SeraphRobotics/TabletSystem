#ifndef BASICSTRUCTURES_H
#define BASICSTRUCTURES_H

#include <QString>
#include <QDomNode>
#include <QDate>

struct Name{
    QString title;
    QString first;
    QString last;
    explicit Name();
    explicit Name(QString t,QString f, QString l);
    QString toQString();
};

Name nameFromNode(QDomNode node);
QDomNode nodeFromName(Name n);
QDateTime dateFromNode(QDomNode node);
QDomNode nodeFromDate(QDateTime date);


#endif // BASICSTRUCTURES_H
