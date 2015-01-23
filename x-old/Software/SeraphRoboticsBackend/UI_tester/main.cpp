#include <QApplication>

#include "testing/test.h"
#include "mastercontrolunit.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MasterControlUnit* mcu = new MasterControlUnit();
    test* t = new test();
    mcu->connect(mcu,SIGNAL(users(QList< UI_User >)),t,SLOT(users(QList< UI_User >)));
    mcu->connect(mcu,SIGNAL(logOnSuccessful(QString)),t,SLOT(logOnSuccessful(QString)));
    mcu->connect(mcu,SIGNAL(patients(QList<UI_Patient>)),t,SLOT(patients(QList<UI_Patient>)));
    mcu->connect(mcu,SIGNAL(shellModifications(QList< UI_Shell_Modification>)),t,SLOT(shellModifications(QList< UI_Shell_Modification>)));
    mcu->connect(mcu,SIGNAL(USBItemsChanged(QList<UI_USB_Item>)),t,SLOT(USBItemsChanged(QList<UI_USB_Item>)));
    mcu->connect(mcu,SIGNAL(scanLoaded(Foot_Type,QImage)),t,SLOT(scanLoaded(Foot_Type,QImage)));
    mcu->connect(mcu,SIGNAL(view3D(Foot_Type,View_3D_Item)),t,SLOT(view3D(Foot_Type,View_3D_Item)));

    mcu->connect(t,SIGNAL(logIn(QString,QString)),mcu,SLOT(logIn(QString,QString)));
    mcu->connect(t,SIGNAL(testUSB()),mcu,SLOT(USBConnectionTest()));
    mcu->connect(t,SIGNAL(addModifications(QList<UI_Shell_Modification>,Foot_Type)),mcu,SLOT(addModifications(QList<UI_Shell_Modification>,Foot_Type)));
    mcu->connect(t,SIGNAL(addScansToPatient(QStringList,QString)),mcu,SLOT(addScansToPatient(QStringList,QString)));
    mcu->connect(t,SIGNAL(getPatients()),mcu,SLOT(getPatients()));
    mcu->connect(t,SIGNAL(getShellModifications()),mcu,SLOT(getShellModifications()));



    QTimer::singleShot(0,mcu,SLOT(getUsers()));

    qDebug()<<"start";
    return a.exec();
}
