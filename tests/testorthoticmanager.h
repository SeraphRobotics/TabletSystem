#ifndef TESTORTHOTICAMANAGER_H
#define TESTORTHOTICAMANAGER_H

#include <QtTest/QtTest>

class ScanManager;
class OrthoticManager;

class TestOrthoticManager : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    // tests
    void testIfSampleDataFound();
    void testGetOrthotic();

private:
    ScanManager *sm;
    OrthoticManager *om;
};








#endif
