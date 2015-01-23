#ifndef MASTERCONTROLUNIT_H
#define MASTERCONTROLUNIT_H

#include <QObject>
#include "UI_structs.h"

#include "patientmanager.h"
#include "usermanager.h"
#include "scanmanager.h"
#include "orthoticmanager.h"

class MasterControlUnit : public QObject
{
    Q_OBJECT
public:
    explicit MasterControlUnit(QObject *parent = 0);

    QList< User > getUsersList(); //start
    QList< UI_Patient > getPatientsList();//choose patient
    QList< UI_Shell_Modification> getShellModificationsList();

signals:
    void logOnSuccessful(QString user_id);//start
    void logOnFailed();//start
    void loggedOff();

    void users(QList< UI_User > users); //start
    void patients(QList< UI_Patient > patient);//choose patient
    void shellModifications(QList< UI_Shell_Modification> mods);

    void USBPlugedIn();//choose patient
    void USBItemsChanged(QList < UI_USB_Item > items);
    void USBUnPluged();//choose patient


    void scanLoaded(Foot_Type foottype, QImage scanimage);//settings

    void view3D(Foot_Type foot, View_3D_Item mesh);//settings and review



    void connectedToInternet(); // setup
    void disconnectedFromInternet(); // setup



public slots:

    void logIn(QString user_id, QString password);//start
    void logOff();
    void deleteAccount(QString user_id);
    void setUserPin(QString user_id,QString pin);
    void addNewAccount(UI_User new_user,QString pin);
    void updateUser(UI_User user);
    void updateUserList(QList<QObject *> *list); // save user list from qml


    void getUsers(); //start
    void getPatients();//choose patient
    void getShellModifications();



    void deleteUSBItem(QString id);//choose patient //patient history
    void addScansToPatient(QStringList scanids, QString);//choose patient
    void newPatientFromScans(QStringList scanids, UI_User, QString);//choose patient
    void loadPatient(QString patient_id);//choose patient

    void transferRxToUSB(QString rxid);//patient history

    void setFrontEdge(Front_Edge points, Foot_Type footside);//settings-rx1

    void setTopCoat(Foot_Type foot,Top_Coat coating);//settings-rx2-topcoat

    void addModifications(QList< UI_Shell_Modification> modifications, Foot_Type foot);//settings-rx2-shellmods
    void removeAllModifications(Foot_Type foot);//settings-rx2-shellmods
    void newTypeofShellModification(UI_Shell_Modification modification);

    void setPosting(Posting posting, Foot_Type foot);//settings-rx2-posting

    void saveCurrentRx(); //review
    void saveCurrentRxAndLoadToUSB(); //review


    void loadCustomerNumber(QString); //setup

    void USBConnectionTest();
private slots:

    void test3d(Foot_Type foot);
    void testScanLoads(int num);

private:
    UserDataManager* um_;
    PatientManager* pm_;
    ScanManager* sm_;
    OrthoticManager* om_;
    QList< UI_USB_Item > items_;
};


int randInt(int low, int high);
#endif // MASTERCONTROLUNIT_H
