#include "testscannercontroller.h"

#define TEST_PRIVATE_METHODS

#ifdef TEST_PRIVATE_METHODS
// ugly hack to TDD private methods, use carefully, avoid if possible
  #define private public
  #include "Controllers/scannerwatcher.h"
  #include "Controllers/scannercontroller.h"
  #undef private
#else
  #include "Controllers/scannerwatcher.h"
  #include "Controllers/scannercontroller.h"
#endif

TestScannerController::TestScannerController(QObject *parent): QObject(parent){}

void TestScannerController::initTestCase(){
    QSettings s;

    int cn = s.value("scanner/camNumber",-1).toInt();
    if(cn ==-1){
        s.setValue("scanner/camNumber",1);
    }
    s.value("scanner/directory",QDir::currentPath());
    ScannerWatcher* sw = new ScannerWatcher();
    ScannerController* sc = new ScannerController();


    sc->connect(sw,SIGNAL(scannerPort(QString)),sc,SLOT(portSelected(QString)));
    sc->connect(sw,SIGNAL(disconnected()),sc,SLOT(disconnected()));
}
void TestScannerController::cleanupTestCase(){}
void TestScannerController::init(){}
void TestScannerController::cleanup(){}
