#include "scantostlmcu.h"
#include "globals.h"

#ifdef Q_OS_WIN
const QString SampleDataLocation(QString(QDir::currentPath()+ "/SampleData/" ));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/output/" ));
#else
const QString SampleDataLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/"));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/output"));
#endif //Q_OS_WIN




QMap<QString,int> alphabet;



ScanToSTLMCU::ScanToSTLMCU(QObject *parent) : QObject(parent)
{
    sm = new ScanManager();
    om = new OrthoticManager(sm);
    oc = new OrthoticController(om);

    QSettings settings;
    settings.setValue("scan-extension","scan");
    settings.setValue("scan-directory", SampleDataLocation);

    settings.setValue("ortho-extension","ortho");
    settings.setValue("ortho-directory", SampleDataLocation);


    settings.setValue("Generating/border",2);
    settings.setValue("Generating/slope",63.5/101);
    settings.setValue("Generating/offset",2.0);
    settings.setValue("Generating/blurtimes",10);
    settings.setValue("printing/topcoat-thickness",2.0);

    //settings.setValue("data-output-directory", SampleDataOutputLocation);
    settings.setValue("printing/directory", SampleDataOutputLocation);

#ifdef Q_OS_WIN
    settings.setValue("printing/slicer","\"C://Program Files//Repetier-Host//Slic3r//slic3r-console.exe\"");
#else
    settings.setValue("printing/slicer","/usr/bin/slic3r");
#endif //Q_OS_WIN

    settings.setValue("printing/plastic_ini", QString(SampleDataLocation) + "p.ini");
    settings.setValue("printing/inis",QString(SampleDataLocation) );

    settings.setValue("printing/valving-python-script",QString(SampleDataLocation + "toValve.py"));
    settings.setValue("printing/merge-python-script",QString(SampleDataLocation +"merge.py"));



    settings.setValue("patient-file","patients.xml");
    settings.setValue("patient-directory",QDir::currentPath());

    settings.setValue("users-file","users.xml");
    settings.setValue("users-directory",QDir::currentPath());

    settings.sync();
    qDebug() << "running tests";


    alphabet["a"]=1;
    alphabet["b"]=2;
    alphabet["c"]=3;
    alphabet["d"]=4;
    alphabet["e"]=5;
    alphabet["f"]=6;
    alphabet["g"]=7;
    alphabet["h"]=8;
    alphabet["i"]=9;
    alphabet["j"]=10;
    alphabet["k"]=11;
    alphabet["l"]=12;
    alphabet["m"]=13;
    alphabet["n"]=14;
    alphabet["o"]=15;
    alphabet["p"]=16;
    alphabet["q"]=17;
    alphabet["r"]=18;
    alphabet["s"]=19;
    alphabet["t"]=20;
    alphabet["u"]=21;
    alphabet["v"]=22;
    alphabet["w"]=23;
    alphabet["x"]=24;
    alphabet["y"]=25;
    alphabet["z"]=26;


    connect(oc,SIGNAL(boundaryLoopUpdated(FAHLoopInXYPlane*)), this, SLOT(boundaryLoopUpdated(FAHLoopInXYPlane*)));
    connect(oc,SIGNAL(scanImageGenerated(QImage)), this, SLOT(scanImageGenerated(QImage)));
}

FAHVector3 ScanToSTLMCU::pointFromValues(QString ny,int nx,float xscale,float yscale){
    FAHVector3 vec;
    if(ny.length()==1){
        vec.y=yscale*alphabet[QString(ny.at(0)).toLower()];
    }else if (ny.length()==2){
        int num = alphabet[QString(ny.at(0)).toLower()];
        vec.y=yscale*( num*26.0+alphabet[QString(ny.at(1)).toLower()] );
    }
    vec.x = xscale*nx;
    vec.z = 0;
    return vec;
}


void ScanToSTLMCU::boundaryLoopUpdated(FAHLoopInXYPlane* loop){
    writeLoopToXDFL(loop, "Loop.xdfl");
}

void ScanToSTLMCU::scanImageGenerated(QImage img){
    img.save("img.jpeg");
}

void ScanToSTLMCU::processScan(){

    float scalex =1.0;
    float scaley =1.0;

    // y
    // |
    // |
    // o------>x
    QVector< FAHVector3 > forePts;
    forePts.append(pointFromValues("BU",71,scalex,scaley));
    forePts.append(pointFromValues("CN",56,scalex,scaley));
    forePts.append(pointFromValues("FM",48,scalex,scaley));
    forePts.append(pointFromValues("GU",54,scalex,scaley));

    QVector< FAHVector3 > healPts;
    //scalex=1.0;
    healPts.append(pointFromValues("DO",132,scalex,scaley));
    healPts.append(pointFromValues("FA",146,scalex,scaley));
    healPts.append(pointFromValues("GM",131,scalex,scaley));

    Posting forpost;
    forpost.angle=0*M_PI/180.0;
    forpost.verticle=0;
    forpost.varus_valgus=Posting::kValgus;
    forpost.for_rear=Posting::kForFoot;
    Posting rearpost;
    rearpost.angle=0*M_PI/180.0;
    rearpost.verticle=0;
    rearpost.varus_valgus=Posting::kValgus;
    rearpost.for_rear=Posting::kRearFoot;

    Top_Coat tc;
    tc.density = Top_Coat::kHigh;
    tc.depth=1;
    tc.style=Top_Coat::kAuto;
    tc.thickness=5;

    oc->setScan(sm->scanIds()[0]);
    oc->setBorderPoints(healPts, forePts);
    qDebug() << "border made";
    oc->setTopCoat(tc);
    qDebug() << "topcoat set";
    oc->processBoundary();
    qDebug() << "boundary processed";
    oc->setPosting(forpost);
    qDebug() << "forpost";

    oc->setPosting(rearpost);
    qDebug() << "rearpost";
    oc->setBottomType(Orthotic::kCurved);
    qDebug() << "setBottom";


//    oc->save();
    qDebug() << "pre make stls";

    oc->makeSTLs();

    qDebug() << "post make stls";
    PrintJobController* pjc = new PrintJobController(oc->getOrthotic());
    pjc->RunPrintJob();
}

