#include "test.h"
#include <QDebug>

test::test(QObject *parent) :
    QObject(parent)
{
}

void test::users(QList< UI_User > us){

    qDebug()<<"users:";

    for(int i=0;i<us.size();i++){
        qDebug() << us[i].name.toQString();
    }
    emit logIn(us[0].id,"123");
}


void test::logOnSuccessful(QString user_id){
    qDebug()<<"user: "<<user_id<<" logged on";

    userid = user_id;
    emit getPatients();
    emit getShellModifications();
    emit testUSB();
}

void test::patients(QList< UI_Patient > patients){
    for(int i=0; i<patients.size();i++){
        qDebug()<<"Patient: "<< patients[i].name.toQString();
        if(i==0){patient=patients[0].id;}
    }

}

void test::shellModifications(QList< UI_Shell_Modification> mods){
    for(int i=0; i<mods.size();i++){
        qDebug()<<"Mods: "<< mods[i].name;
        if(i==0){m = mods[0];}
    }

}

void test::USBItemsChanged(QList < UI_USB_Item > items){
    for(int i=0;i<items.size();i++){
        qDebug() << "item type" << items[i].type << " id: " << items[i].id;
    }
    emit addScansToPatient(QStringList(items[0].id),patient);
}

void test::scanLoaded(Foot_Type foottype, QImage scanimage){
    qDebug()<<"Loaded"<<foottype;

    QList<UI_Shell_Modification> ms;
    ms.append(m);

    addModifications(ms,foottype);
}//settings

void test::view3D(Foot_Type foot, View_3D_Item mesh){
    qDebug()<<"3D foot:"<<foot <<", Color"<<mesh.color;
}//settings and review
