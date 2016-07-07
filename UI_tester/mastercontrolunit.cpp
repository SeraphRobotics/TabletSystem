#include "mastercontrolunit.h"

#include "shared/stl/stlfile.h"

#include <qglobal.h>
#include <QTime>

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    um_ = new UserDataManager(this);
    pm_ = new PatientManager(this);
    sm_ = new ScanManager(this);
    om_ = new OrthoticManager(sm_,this);

    UI_USB_Item i1;
    i1.comment = "";
    i1.datetime =  QDateTime( QDate(2014,5,29),QTime(12,10,0) );
    i1.patient =Name();
    i1.id="{64844aae-619c-47e1-ae93-55ba93b5ab9a}";
    i1.type=UI_USB_Item::kScan;
    items_.append(i1);

    UI_USB_Item i2;
    i2.comment = "";
    i2.datetime =  QDateTime( QDate(2014,5,29),QTime(12,12,30) );
    i2.patient = Name();
    i2.id="{64844aae-619c-47e1-ae93-66ba63b5ab9a}";
    i2.type=UI_USB_Item::kScan;
    items_.append(i2);

    UI_USB_Item i3;
    i3.comment = "Left";
    i3.datetime =  QDateTime( QDate(2014,5,29),QTime(12,40,0) );
    i3.patient =Name("Mr","John","Doe");
    i3.id="{64844aae-619c-47e1-ae93-77ba93b5ab9a}";
    i3.type=UI_USB_Item::kRx;
    items_.append(i3);

    UI_USB_Item i4;
    i4.comment = "Right";
    i4.datetime =  QDateTime( QDate(2014,5,29),QTime(12,42,30) );
    i4.patient =Name("Mr","John","Doe");
    i4.id="{64844aae-619c-47e1-ae93-88ba63b5ab9a}";
    i4.type=UI_USB_Item::kRx;
    items_.append(i4);

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}


void MasterControlUnit::USBConnectionTest(){
    emit USBPlugedIn();
    emit USBItemsChanged(items_);
}

void  MasterControlUnit::test3d(Foot_Type foot){
    STLFile f;
    View_3D_Item v3d;

    if(foot == kLeft){
        f.read("left.stl");

    }else{
        f.read("right.stl");
    }
    v3d.color = QColor(randInt(0,255),randInt(0,255),randInt(0,255));

    foreach(STLFacet* facet,f.GetMesh().GetFacets()){
        STLFacet fct;
        fct.triangle = facet->triangle;
        fct.normal = facet->normal;
        v3d.mesh.AddFacet(fct);
    }

    emit view3D(foot,v3d);
}

void  MasterControlUnit::testScanLoads(int num){
    if (num >1){
        QImage i;
        i.load("left-scan.png");
        emit scanLoaded(kLeft,i);
    }

    QImage i2;
    i2.load("right-scan.png");
    emit scanLoaded(kRight,i2);
}


QList< User > MasterControlUnit::getUsersList(){//start
    return um_->getUsers();
}

QList< UI_Patient > MasterControlUnit::getPatientsList(){//choose patient
    qDebug()<<__FUNCTION__;
    QList<PatientSearch> patients = pm_->getPatients();
    QList< UI_Patient> ui_patients;

    for(int i=0;i<patients.size();i++) {
        PatientSearch ps = patients.at(i);
        UI_Patient uip;
        uip.name = ps.name;
        uip.id=ps.id;
        uip.doctorid = ps.docIds.at(0);
        Patient p = pm_->getByID(ps.id);

        QList<Rx> rxs=p.rxs;

        /// create orthotic
        for(int j=0;j<rxs.size();j++){
            Rx rx = rxs.at(j);
            UI_USB_Item item;
            item.id=rx.leftOrthoticId;
            item.type=UI_USB_Item::kRx;
            item.datetime= QDateTime(rx.date);
            item.patient=uip.name;
            item.comment=rx.note;
            uip.item_list.append(item);
        }

        /// create scan
        ///
        for(int j=0;j<rxs.size();j++){
            Rx rx = rxs.at(j);
            UI_USB_Item item;
            item.id=rx.leftScanId;
            item.type=UI_USB_Item::kScan;
            item.datetime= QDateTime(rx.date);
            item.patient=uip.name;
            item.comment=rx.note+"This is comment from scan data from ";
            uip.item_list.append(item);
        }

        ui_patients.append(uip);


    }
    return ui_patients;
}
QList< UI_Shell_Modification> MasterControlUnit::getShellModificationsList(){
    QList< UI_Shell_Modification> mods;
    UI_Shell_Modification m1;
    m1.name="Hole";
    m1.height=0;
    m1.depth=100;
    m1.stiffness=0;
    Border b;
    b.append(QPointF(0,0));
    b.append(QPointF(10,0));
    b.append(QPointF(0,10));
    b.append(QPointF(10,10));
    m1.outer_border=b;

    UI_Shell_Modification m2;
    m2.name="box with hole";
    m2.height=10;
    m2.depth=0;
    m2.stiffness=110;
    Border b2;
    b2.append(QPointF(0,0));
    b2.append(QPointF(30,0));
    b2.append(QPointF(0,30));
    b2.append(QPointF(30,30));
    m2.outer_border=b2;
  //  m2.inner_borders.append(b);

    mods.append(m1);
    mods.append(m2);
    return mods;

}

void MasterControlUnit::logIn(QString user_id, QString password){
    User u=um_->getUserByPwd(password);
    if(u.id!=user_id){
        emit logOnFailed();
    }else{
        emit logOnSuccessful(user_id);
    }
}//start

void MasterControlUnit::logOff(){
    qDebug()<<"logoff";
    emit loggedOff();
}

void MasterControlUnit::deleteAccount(QString user_id){
    qDebug()<<"Delete: "<<user_id<< " :"<<um_->hasUserID(user_id);
    um_->removeUser(um_->getUserByID(user_id));
}

void MasterControlUnit::setUserPin(QString user_id,QString pin){
    if(um_->hasUserID(user_id)){
        um_->changeUserPin(user_id,pin);
    }
}
void MasterControlUnit::addNewAccount(UI_User new_user,QString pin){
    User u;
    u.name=new_user.name;
    u.id=new_user.id;
    new_user.icon.save(u.name.toQString());
    u.iconfile=u.name.toQString();
    u.pwd=pin;
}

void MasterControlUnit::updateUser(UI_User user){
    User u = um_->getUserByID(user.id);
    if(!(u.id=="")){return;}

    u.name=user.name;
    u.id=user.id;
    user.icon.save(u.name.toQString());
    u.iconfile=u.name.toQString();

    um_->updateUser(u);
}

/*
 * This Function is called whenever "Save & Continue" button from page 6 is pressed.
 * Its only parameter "list" represents QList of UserObjects (Ui/src/Models/)
 * that was saved by the user. I suggest that function should reload users.xml to this list
*/
void MasterControlUnit::updateUserList(QList<QObject*> *list)
{
    qDebug() << "updateUserList called with a list of size " << list->size();
}

void MasterControlUnit::getUsers(){
    //emit users(getUsersList()); //start
} //start

void MasterControlUnit::getPatients(){
    emit patients(getPatientsList());//choose patient
}//choose patient

void MasterControlUnit::getShellModifications(){
    emit shellModifications(getShellModificationsList());
}



void MasterControlUnit::deleteUSBItem(QString id){
    int index=-1;
    for(int i=0;i<items_.size();i++){
        if(id==items_.at(i).id){
            index=i;
        }
    }

    if(index>=0){
        items_.removeAt(index);
        emit USBItemsChanged(items_);
    }
}//choose patient //patient history

void MasterControlUnit::addScansToPatient(QStringList scanids, QString /*patientid*/){
    qDebug()<<"add scan";
    testScanLoads(scanids.size());
}//choose patient

void MasterControlUnit::newPatientFromScans(QStringList scanids, UI_User /*user*/, QString /*comments*/){
    qDebug()<<"new patient";
    testScanLoads(scanids.size());
}//choose patient

void MasterControlUnit::loadPatient(QString patient_id){
    qDebug()<<"load patient: "<<patient_id;
    testScanLoads(2);
}//choose patient

void MasterControlUnit::transferRxToUSB(QString rxid){
    qDebug()<<"Transfer "<<rxid<<" to USB";
}//patient history

void MasterControlUnit::setFrontEdge(Front_Edge points, Foot_Type footside){
    qDebug()<<"Front Edge for "<<footside<<" is: ";
    qDebug()<<"\t" << points.p1 << ", " << points.p2 << ", "<< points.p3 << ", " << points.p4;

    if(footside==kLeft){
        QImage i;
        i.load("left-top.jpg");
        emit scanLoaded(footside,i);
    }else if(footside ==kRight){
        QImage i;
        i.load("right-top.jpg");
        emit scanLoaded(footside,i);
    }
}//settings-rx1

void MasterControlUnit::setTopCoat(Foot_Type foot,Top_Coat coating){
    qDebug()<<"set Top coat for "<<foot<<".";
    qDebug()<<"\tdensity (l/m/h): " <<coating.density;
    qDebug()<<"\tdepth (mm): " <<coating.depth;
    qDebug()<<"\tthickness (mm): " <<coating.thickness;
    qDebug()<<"\tstyle: " <<coating.style;
}//settings-rx2-topcoat

void MasterControlUnit::addModifications(QList< UI_Shell_Modification> modifications, Foot_Type foot){
    qDebug()<<"added mods on "<<foot<<" : ";
    for(int i=0;i<modifications.size();i++){
        qDebug()<<"\t"<<modifications.at(i).name;
    }
    test3d(foot);

}//settings-rx2-shellmods

void MasterControlUnit::removeAllModifications(Foot_Type foot){
    qDebug()<<"remove mods "<<foot;
}//settings-rx2-shellmods

void MasterControlUnit::newTypeofShellModification(UI_Shell_Modification modification){
    qDebug()<<"new modification: "<<modification.name<<"\n\touter:";
    for(int i=0;i<modification.outer_border.size();i++){
        qDebug()<<"\t\t"<<modification.outer_border.at(i);
    }
    for(int i=0;i<modification.inner_borders.size();i++){
        qDebug()<<"\tinner border "<<i;
        //for(int j=0;j<modification.inner_borders.at(i).size();j++){
        //   ;// qDebug()<<"\t\t"<<modification.inner_borders.at(i).at(j);
        //}
    }
}
void MasterControlUnit::setPosting(Posting posting, Foot_Type foot){
    qDebug() << "set posting for foot:"<<foot;
    qDebug() << "\tangle: " << posting.angle;
    qDebug() << "\tverticle: " << posting.verticle;
    qDebug() << "\tvargus: " << posting.varus_valgus;
    qDebug() << "\tside: "<<posting.for_rear;

    test3d(foot);

}//settings-rx2-posting

void MasterControlUnit::saveCurrentRx(){qDebug()<<"save rx";} //review
void MasterControlUnit::saveCurrentRxAndLoadToUSB(){qDebug()<<"save rx and load to USB";} //review
void MasterControlUnit::loadCustomerNumber(QString number){qDebug()<<"customer number is "<<number;} //setup


int randInt(int low, int high){
    return qrand() % ((high + 1) - low) + low;
}
