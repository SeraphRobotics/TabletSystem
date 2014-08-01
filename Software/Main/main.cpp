#include <QApplication>
#include <QSettings>
#include "DataStructures/usbminder.h"
#include "DataStructures/usbmanager.h"
#include "UnitTest/usbtester.h"
#include "Controllers/processingfunctions.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("Seraph");
    QApplication::setOrganizationDomain("seraphrobotics.com");
    QApplication::setApplicationName("TabletCoreSoftware");
    QSettings::setDefaultFormat(QSettings::IniFormat);
    USBMinder um;
    USBManager manager;
    manager.connect(&um,SIGNAL(usbadded(QString)),&manager,SLOT(usbAdded(QString)));
    manager.connect(&um,SIGNAL(usbRemoved(QString)),&manager,SLOT(usbDisconnected(QString)));
    USBTester ut;

    manager.connect(&manager,SIGNAL(uiUSBItemsUpdated(QList<UI_USB_Item>)),&ut,SLOT(uiUSBItemsUpdated(QList<UI_USB_Item>)));
    manager.connect(&ut,SIGNAL(deleteItem(QString)),&manager,SLOT(deleteItem(QString)));

    qDebug()<<"result factorial: "<<factorial(5);
    qDebug()<<"result comb: "<<comb(10,3);
    qDebug()<<"result b:"<<bernstein_poly(3,4,0.5);
    QVector< FAHVector3 > points;
    points.append(FAHVector3(60.0,105.0,0));
    points.append(FAHVector3(90.0,120.0,0));
    points.append(FAHVector3(125.0,115.0,0));
    points.append(FAHVector3(140.0,85.0,0));

    qDebug()<<bezier_curve(points);

    QVector< FAHVector3 > points2;
    points2.append(FAHVector3(50.0,35.0,0));
    points2.append(FAHVector3(70.0,19.0,0));
    points2.append(FAHVector3(103.0,26.0,0));

    qDebug()<<secondOrder(points2);

    return a.exec();
}
