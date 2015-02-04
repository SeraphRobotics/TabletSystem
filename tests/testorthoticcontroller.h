#ifndef TESTORTHOTICCONTOLLER_H
#define TESTORTHOTICCONTOLLER_H

#include <QtTest/QtTest>

class ScanManager;
class OrthoticManager;
class OrthoticController;

class TestOrthoticController : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    // tests


private:

    ScanManager *sm;
    OrthoticManager *om;
    OrthoticController *oc;
};


#endif



