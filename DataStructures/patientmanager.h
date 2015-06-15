#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QObject>
#include <QDir>
#include <QString>
#include "patientdata.h"
#include <QPixmap>


struct PatientSearch{
    QString id;
    Name name;
    QList<QDate> dates;
    QStringList docIds;
};

class PatientManager : public QObject
{
    Q_OBJECT
public:
    explicit PatientManager(QObject *parent = 0);

    Patient getByID(QString id);
    Patient getByOrthoticsID(QString id);
    bool hasPatientID(QString id);
    QList<PatientSearch> getPatients();

signals:

public slots:
    void updateRx(QString PatientID, Rx r);
    void addPatient(Patient P);
    void removePatient(QString PatientID);
    void writeToDisk();

private slots:
    void rescan();

private:

    Patient getOriginalByID(QString id);

private:
    QString filename_;
    QDir dir_;
    QList<Patient> patients_;
};

#endif // PATIENTMANAGER_H
