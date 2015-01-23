#ifndef TEST_H
#define TEST_H
#include <QDebug>
#include "manipulations.h"
#include "orthotic.h"

void manipulationsTest();

void orthoticTest();

void orthoticTest2();

void scanTest();

void scanTest2();

void testScanAndOrthotic();

void patientRxTest();

void userTest();

void userManagerTest();

//PATIENT MANAGER

void patientManagerWriteTest();

void patientManagerReadTest();

void patientManagerSecurityTest();

void patientManagerUpdateTest();

void patientManagerRemoveTest();

void patientManagerOrthoTest();

//SCAN MANAGER

void ScanManagerReadTest();

void ScanManagerWriteTest();

// OrthoManager
void orthoManagerReadTest();

void orthoManagerWriteTest();

//Complete run
void newScanTest();

void newPatientTest();
#endif // TEST_H
