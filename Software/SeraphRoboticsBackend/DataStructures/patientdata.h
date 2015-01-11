#ifndef PATIENTDATA_H
#define PATIENTDATA_H
#include <QDate>
#include <QString>
#include <QDomNode>
#include "basicstructures.h"
#include <QUuid>

struct Rx{
    QString docId;
    QDateTime date;
    QString note;
    float shoesize;
    QString leftScanId;
    QString rightScanId;
    QString leftOrthoticId;
    QString rightOrthoticId;
    Rx();
    Rx(const Rx& r);
    bool operator==(Rx r);
};

QDomNode nodeFromRx(Rx r);
Rx rxFromNode(QDomNode node);

struct Patient{
    enum ShoeGender {kMale,kFemale};
    QString id;
    ShoeGender gender;
    QList<Rx> rxs;
    Name name;
    Patient();
    Patient(const Patient& p);
    bool operator==(Patient p);

};

QDomNode nodeFromPatient(Patient p);

Patient patientFromNode(QDomNode node);
#endif // PATIENTDATA_H
