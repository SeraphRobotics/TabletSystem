#include "test.h"
#include "scan.h"
#include "patientdata.h"
#include "user.h"
#include "usermanager.h"
#include "patientmanager.h"
#include "scanmanager.h"
#include "orthoticmanager.h"


void manipulationsTest(){
    FAHVector3 a(0,0,0), b(0,10,0), c(10,10,0), d(10,0,0) ;
    FAHLoopInXYPlane* l= new FAHLoopInXYPlane();
    l->add(a);
    l->add(b);
    l->add(c);
    l->add(d);


    FAHLoopInXYPlane* il= new FAHLoopInXYPlane();
    FAHVector3 aa(2,2,0), ab(2,5,0), ac(5,5,0), ad(5,2,0) ;
    il->add(aa);
    il->add(ab);
    il->add(ac);
    il->add(ad);


    Manipulation m;
    m.depth=1;
    m.location=c;
    m.stiffness=20;
    m.type=Met_Bar_Pad;
    m.outerloop=l;
    m.innerloops.append(il);
    QDomNode n = m.toNode();

    Manipulation m2(n);
    QDomNode n2 = m2.toNode();

    QDomDocument doc("Manip");
    doc.appendChild(n2);

    qDebug()<< doc.toString();
}

void orthoticTest(){
    FAHVector3 a(0,0,0), b(0,10,0), c(10,10,0), d(10,0,0) ;
    FAHLoopInXYPlane* l= new FAHLoopInXYPlane();
    l->add(a);
    l->add(b);
    l->add(c);
    l->add(d);


    FAHLoopInXYPlane* il= new FAHLoopInXYPlane();
    FAHVector3 aa(2,2,0), ab(2,5,0), ac(5,5,0), ad(5,2,0) ;
    il->add(aa);
    il->add(ab);
    il->add(ac);
    il->add(ad);


    Manipulation m;
    m.depth=1;
    m.location=c;
    m.stiffness=20;
    m.type=Met_Bar_Pad;
    m.outerloop=l;
    m.innerloops.append(il);

    Posting p1,p2;
    p1.angle=0;
    p2.angle=10;
    p1.for_rear=Posting::kForFoot;
    p2.for_rear=Posting::kRearFoot;
    p1.verticle=1;
    p2.verticle=2;
    p1.varus_valgus=Posting::kValgus;
    p2.varus_valgus=Posting::kVargus;

    Orthotic o;
    o.setFootType(Orthotic::kRight);
    o.setPosting(p1);
    o.setPosting(p2);
    o.addManipulation(m);
    o.setBoundary(l);
    o.writeToDisk();

}

void orthoticTest2(){
    QString filename = "{dcab990f-9242-44da-9666-8214384ff3f6}.xml";
    Orthotic o(filename);
    o.setFootType(Orthotic::kLeft);
    o.writeToDisk();
}


void scanTest(){
    XYGrid<float> g(10,10);
    g.setStepSize(1);
    g(5,5)=10;

    Scan s;
    s.setInitialData(&g);
    s.writeToDisk();
}

void scanTest2(){
    QString filename = "{19491125-6e69-49d2-a074-fff69012bbec}.scan";
    Scan s(filename);
    XYGrid<float>* g =s.getXYGrid();
    g->operator ()(5,5)=20;
    s.writeToDisk();

}

void testScanAndOrthotic(){
    QString sfilename = "{19491125-6e69-49d2-a074-fff69012bbec}.scan";
    Scan s(sfilename);

    FAHVector3 a(0,0,0), b(0,10,0), c(10,10,0), d(10,0,0) ;
    FAHLoopInXYPlane* l= new FAHLoopInXYPlane();
    l->add(a);
    l->add(b);
    l->add(c);
    l->add(d);


    FAHLoopInXYPlane* il= new FAHLoopInXYPlane();
    FAHVector3 aa(2,2,0), ab(2,5,0), ac(5,5,0), ad(5,2,0) ;
    il->add(aa);
    il->add(ab);
    il->add(ac);
    il->add(ad);


    Manipulation m;
    m.depth=1;
    m.location=c;
    m.stiffness=20;
    m.type=Met_Bar_Pad;
    m.outerloop=l;
    m.innerloops.append(il);

    Posting p1,p2;
    p1.angle=0;
    p2.angle=10;
    p1.for_rear=Posting::kForFoot;
    p2.for_rear=Posting::kRearFoot;
    p1.verticle=1;
    p2.verticle=2;
    p1.varus_valgus=Posting::kValgus;
    p2.varus_valgus=Posting::kVargus;

    Orthotic o;
    o.setFootType(Orthotic::kRight);
    o.setPosting(p1);
    o.setPosting(p2);
    o.addManipulation(m);
    o.setBoundary(l);
    o.setScan(&s);
    o.writeToDisk();

}

void patientRxTest(){
    Rx r;
    r.date=QDateTime(QDate(2013,1,2));
    r.docId=QUuid::createUuid().toString();
    r.leftOrthoticId=QUuid::createUuid().toString();
    r.rightOrthoticId=QUuid::createUuid().toString();
    r.leftScanId=QUuid::createUuid().toString();
    r.rightScanId=QUuid::createUuid().toString();
    r.shoesize=9.5;
    r.note=QString("testing");

    QDomNode node = nodeFromRx(r);
    Rx r2 = rxFromNode(node);


    Patient p;
    Name n;
    n.first="john";
    n.last="smith";
    n.title="mr.";
    p.name=n;
    p.gender=Patient::kMale;
    p.rxs.append(r2);

    Patient p2 = patientFromNode(nodeFromPatient(p));
    QDomDocument doc("Manip");
    doc.appendChild(nodeFromPatient(p2));
//    doc.appendChild(nodeFromRx(r2));
    qDebug()<< doc.toString();



}

void userTest(){
    User u;
    u.name = Name("mr.","test","person");
    u.pwd="buttface";
    u.iconfile="icon.jpg";

    QDomNode n = nodeFromUsers(u);

    User u2 = userFromNode(n);
    QDomNode n2 = nodeFromUsers(u2);
    QDomDocument doc("Manip");
    doc.appendChild(n2);
    qDebug()<< doc.toString();
}

void userManagerTest(){
    UserDataManager UM;
    User U = UM.getUserByPwd("test");

    QDomDocument doc("Manip");
    doc.appendChild(nodeFromUsers(U));

    User U2;
    U2.name = Name("Mrs.","Foot","Problems");
    U2.pwd = "dumb";
    UM.addUser(U2);
    UM.removeUser(U);
//    User U2 = UM.getUserByID("{64844aae-619c-47f1-ae93-72ca93b5ab9a}");
//    doc.appendChild(nodeFromUsers(U2));
    qDebug()<< doc.toString();
}



/**
<!DOCTYPE PatientFile>
<Patients>
 <patient id="{ac67227f-697e-4997-b716-1a680ae26c0e}">
  <name>
   <title>mr.</title>
   <first>john</first>
   <last>smith</last>
  </name>
  <gender>0</gender>
  <Rxs>
   <Rx>
    <note>testing</note>
    <shoesize>9.5</shoesize>
    <date>
     <day>2</day>
     <month>1</month>
     <year>2013</year>
    </date>
    <doctor id="{8555548f-617e-43fb-91bc-32a630326ff5}"/>
    <scan id="{19d55b94-360e-40f0-aeb2-d9526072fad2}" foot="0"/>
    <scan id="{57783584-8889-40f9-afe9-3e7d0fec73d2}" foot="1"/>
    <orthotic id="{42cf4869-9515-41a0-9dd4-0c8ef0f0b842}" foot="0"/>
    <orthotic id="{ddeaaf80-2a2e-4cde-84e5-9ecfadd81f9e}" foot="1"/>
   </Rx>
  </Rxs>
 </patient>
 <patient id="{25aae1eb-61e7-4de0-8702-415d8caf3fc0}">
  <name>
   <title>mrs.</title>
   <first>apple</first>
   <last>bottom</last>
  </name>
  <gender>1</gender>
  <Rxs>
   <Rx>
    <note>second</note>
    <shoesize>9.5</shoesize>
    <date>
     <day>1</day>
     <month>1</month>
     <year>2014</year>
    </date>
    <doctor id="{d360ca94-50b6-4b8b-9127-32f1300bd030}"/>
    <scan id="{e8f41c53-4141-4195-bfb5-d8860acb4740}" foot="0"/>
    <scan id="{aa9b4325-fd98-4005-8b46-426e2d75ff40}" foot="1"/>
    <orthotic id="{3d462036-78a9-4dc5-9499-ce30642db3d0}" foot="0"/>
    <orthotic id="{56a1c2ac-7137-47c8-bbe4-96025afc2e9b0" foot="1"/>
   </Rx>
  </Rxs>
 </patient>
</Patients>
**/

void patientManagerWriteTest(){
    PatientManager PM;

    Rx r;
   // r.date=QDate(2013,1,2);
    r.docId=QUuid::createUuid().toString();
    r.leftOrthoticId=QUuid::createUuid().toString();
    r.rightOrthoticId=QUuid::createUuid().toString();
    r.leftScanId=QUuid::createUuid().toString();
    r.rightScanId=QUuid::createUuid().toString();
    r.shoesize=9.5;
    r.note=QString("testing");


    Patient p;
    Name n;
    n.first="john";
    n.last="smith";
    n.title="mr.";
    p.name=n;
    p.gender=Patient::kMale;
    p.rxs.append(r);

    PM.addPatient(p);
}



void patientManagerReadTest(){
    PatientManager PM;
    Patient p=PM.getByID(QString("{ac67227f-697e-4997-b716-1a680ae26c0e}"));

    QDomDocument doc("Manip");
    doc.appendChild(nodeFromPatient(p));
    qDebug()<<doc.toString();

//    p.rxs[0].leftScanId="0";
//    PM.updateRx(p.id,p.r);
}

void patientManagerSecurityTest(){
    PatientManager PM;
    Patient p=PM.getByID(QString("{ac67227f-697e-4997-b716-1a680ae26c0e}"));
    if(p.rxs.size()>0){
        p.rxs[0].note="Whaaaaaaa";
    }
    Patient p2 = PM.getByID(QString("{ac67227f-697e-4997-b716-1a680ae26c0e}"));
    QDomDocument doc("Manip");
    doc.appendChild(nodeFromPatient(p2));
    qDebug()<<doc.toString();
}

void patientManagerUpdateTest(){
    PatientManager PM;
    Patient p=PM.getByID(QString("{ac67227f-697e-4997-b716-1a680ae26c0e}"));
    if(p.rxs.size()>0){
        p.rxs[0].note="Whaaaaaaa";
        qDebug()<<"Called";
    }
    PM.updateRx(p.id,p.rxs[0]);
    PM.writeToDisk();
}

void patientManagerRemoveTest(){
    PatientManager PM;
    PM.removePatient("{25aae1eb-61e7-4de0-8702-415d8caf3fc0}");
}

void patientManagerOrthoTest(){
    PatientManager PM;
    Patient p = PM.getByOrthoticsID("{56a1c2ac-7137-47c8-bbe4-96025afc2e9b0");
    QDomDocument doc("Manip");
    doc.appendChild(nodeFromPatient(p));
    qDebug()<<doc.toString();
}

void ScanManagerReadTest(){
    ScanManager SM;
    Scan* S = SM.getScan("{19491125-6e69-49d2-a074-fff69012bbec}");
    qDebug()<<S->getID();
    qDebug()<<S->getXYGrid()->toCSV();
}

void ScanManagerWriteTest(){
    ScanManager SM;

    XYGrid<float> g(10,10);
    g.setStepSize(1);
    g(5,5)=10;

    Scan* s = new Scan();
    s->setInitialData(&g);

    SM.addScan(s);
}

void orthoManagerReadTest(){
    ScanManager SM;
    OrthoticManager OM(&SM);
    Orthotic* o = OM.getOrthotic("{9ee72db4-2260-41fb-a857-1408d667125c}");
    Scan* s = o->getScan();
    qDebug()<<s->getXYGrid()->toCSV();
    qDebug()<<o->getScanID();

}

void orthoManagerWriteTest(){
    ScanManager SM;
    OrthoticManager OM(&SM);

    QString sId = "{19491125-6e69-49d2-a074-fff69012bbec}";

    FAHVector3 a(0,0,0), b(0,10,0), c(10,10,0), d(10,0,0) ;
    FAHLoopInXYPlane* l= new FAHLoopInXYPlane();
    l->add(a);
    l->add(b);
    l->add(c);
    l->add(d);


    FAHLoopInXYPlane* il= new FAHLoopInXYPlane();
    FAHVector3 aa(2,2,0), ab(2,5,0), ac(5,5,0), ad(5,2,0) ;
    il->add(aa);
    il->add(ab);
    il->add(ac);
    il->add(ad);


    Manipulation m;
    m.depth=1;
    m.location=c;
    m.stiffness=20;
    m.type=Met_Bar_Pad;
    m.outerloop=l;
    m.innerloops.append(il);

    Posting p1,p2;
    p1.angle=0;
    p2.angle=10;
    p1.for_rear=Posting::kForFoot;
    p2.for_rear=Posting::kRearFoot;
    p1.verticle=1;
    p2.verticle=2;
    p1.varus_valgus=Posting::kValgus;
    p2.varus_valgus=Posting::kVargus;

    Orthotic* o = OM.newOrthotic(sId);
    o->setFootType(Orthotic::kRight);
    o->setPosting(p1);
    o->setPosting(p2);
    o->addManipulation(m);
    o->setBoundary(l);
    o->writeToDisk();
}

void newScanTest(){
    UserDataManager UM;
    PatientManager PM;
    ScanManager SM;
    OrthoticManager OM(&SM);

    //LOG IN
    User u = UM.getUserByPwd("test");
    qDebug()<<"User: "<<u.name.toQString();

    //PATIENT SEARCH SCREEN
    QList<PatientSearch> PS = PM.getPatients();

    //PATIENT DATA SCREEN
    Patient p = PM.getByID(PS.at(0).id);

    //NEW  SCAN
    Rx r;

    r.docId=u.id;

    Scan* s = new Scan();

    XYGrid<float> g(10,10);
    g.setStepSize(1);
    g(5,5)=10;

    s->setInitialData(&g);



    //       SAVE SCAN
    SM.addScan(s);

    r.leftScanId=s->getID();


    PM.updateRx(p.id,r);

    //New Orthotics
    FAHVector3 a(0,0,0), b(0,10,0), c(10,10,0), d(10,0,0) ;
    FAHLoopInXYPlane* l= new FAHLoopInXYPlane();
    l->add(a);
    l->add(b);
    l->add(c);
    l->add(d);


    FAHLoopInXYPlane* il= new FAHLoopInXYPlane();
    FAHVector3 aa(2,2,0), ab(2,5,0), ac(5,5,0), ad(5,2,0) ;
    il->add(aa);
    il->add(ab);
    il->add(ac);
    il->add(ad);


    Manipulation m;
    m.depth=1;
    m.location=c;
    m.stiffness=20;
    m.type=Met_Bar_Pad;
    m.outerloop=l;
    m.innerloops.append(il);

    Posting p1,p2;
    p1.angle=0;
    p2.angle=10;
    p1.for_rear=Posting::kForFoot;
    p2.for_rear=Posting::kRearFoot;
    p1.verticle=1;
    p2.verticle=2;
    p1.varus_valgus=Posting::kValgus;
    p2.varus_valgus=Posting::kVargus;

    Orthotic* o = OM.newOrthotic(s->getID());
    o->setFootType(Orthotic::kLeft);
    o->setPosting(p1);
    o->setPosting(p2);
    o->addManipulation(m);
    o->setBoundary(l);
    o->writeToDisk();

    //       save orthotic
    r.leftOrthoticId=o->getId();
    PM.updateRx(p.id,r);

}

void newPatientTest(){
    UserDataManager UM;
    PatientManager PM;
    ScanManager SM;
    OrthoticManager OM(&SM);

    //LOG IN
    User u = UM.getUserByPwd("test");
    qDebug()<<"User: "<<u.name.toQString();

    //New Patient
    Patient p;
    p.name = Name("ms.","New","Patient");
    p.gender=Patient::kFemale;

    PM.addPatient(p);

    //NEW  SCAN
    Rx r;

    r.docId=u.id;

    Scan* s = new Scan();

    XYGrid<float> g(10,10);
    g.setStepSize(1);
    g(5,5)=10;

    s->setInitialData(&g);



    //       SAVE SCAN
    SM.addScan(s);

    r.leftScanId=s->getID();


    PM.updateRx(p.id,r);

    //New Orthotics
    FAHVector3 a(0,0,0), b(0,10,0), c(10,10,0), d(10,0,0) ;
    FAHLoopInXYPlane* l= new FAHLoopInXYPlane();
    l->add(a);
    l->add(b);
    l->add(c);
    l->add(d);


    FAHLoopInXYPlane* il= new FAHLoopInXYPlane();
    FAHVector3 aa(2,2,0), ab(2,5,0), ac(5,5,0), ad(5,2,0) ;
    il->add(aa);
    il->add(ab);
    il->add(ac);
    il->add(ad);


    Manipulation m;
    m.depth=1;
    m.location=c;
    m.stiffness=20;
    m.type=Met_Bar_Pad;
    m.outerloop=l;
    m.innerloops.append(il);

    Posting p1,p2;
    p1.angle=0;
    p2.angle=10;
    p1.for_rear=Posting::kForFoot;
    p2.for_rear=Posting::kRearFoot;
    p1.verticle=1;
    p2.verticle=2;
    p1.varus_valgus=Posting::kValgus;
    p2.varus_valgus=Posting::kVargus;

    Orthotic* o = OM.newOrthotic(s->getID());
    o->setFootType(Orthotic::kLeft);
    o->setPosting(p1);
    o->setPosting(p2);
    o->addManipulation(m);
    o->setBoundary(l);
    o->writeToDisk();

    //       save orthotic
    r.leftOrthoticId=o->getId();
    PM.updateRx(p.id,r);
}
