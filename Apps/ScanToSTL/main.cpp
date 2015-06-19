#include <QCoreApplication>
#include "scantostlmcu.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ScanToSTLMCU* sts = new ScanToSTLMCU();
    sts->processScan();

    return a.exec();
}
