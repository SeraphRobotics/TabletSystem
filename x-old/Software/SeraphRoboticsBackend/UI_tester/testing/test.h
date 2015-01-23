#ifndef TEST_H
#define TEST_H

#include <QObject>
#include "mastercontrolunit.h"



class test : public QObject
{
    Q_OBJECT
public:
    explicit test(QObject *parent = 0);

signals:
    void logIn(QString user_id, QString password);
    void getPatients();
    void getShellModifications();
    void addScansToPatient(QStringList scanids, QString patientid);
    void addModifications(QList< UI_Shell_Modification> modifications, Foot_Type foot);
    void testUSB();

public slots:
    void users(QList< UI_User > us);
    void logOnSuccessful(QString user_id);
    void patients(QList< UI_Patient > patient);//choose patient
    void shellModifications(QList< UI_Shell_Modification> mods);
    void USBItemsChanged(QList < UI_USB_Item > items);
    void scanLoaded(Foot_Type foottype, QImage scanimage);//settings
    void view3D(Foot_Type foot, View_3D_Item mesh);//settings and review

private:
    QString userid;
    QString patient;
    UI_Shell_Modification m;
};

#endif // TEST_H
