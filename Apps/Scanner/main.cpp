#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include "scanningmcu.h"


#ifdef Q_OS_WIN
const QString SampleDataLocation(QString(QDir::currentPath()+ "/SampleData/" ));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/output/" ));
#else
const QString SampleDataLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/"));
const QString SampleDataOutputLocation(QString(QDir::currentPath() + "/../seraphLibs/SampleData/output"));
#endif //Q_OS_WIN

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Seraph");
    QCoreApplication::setOrganizationDomain("seraphrobotics.com");
    QCoreApplication::setApplicationName("SeraphScannerTest");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QSettings settings;
    settings.setValue("scan-extension","scan");
    settings.setValue("scan-directory", SampleDataLocation);

    QString default_path = QFileInfo(settings.fileName()).absolutePath();
    QString writelocation = default_path+"//"+"summed.jpeg";
    settings.setValue("scanner/noisefile",writelocation);
    settings.sync();

    ScanningMCU* smcu = new ScanningMCU();
    smcu->processNow();

    return a.exec();
}
