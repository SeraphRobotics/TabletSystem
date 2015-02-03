#ifndef SCANSYSTEMTESTER_H
#define SCANSYSTEMTESTER_H

#include <QObject>
#include "DataStructures/usbminder.h"
#include "DataStructures/usbmanager.h"
#include "DataStructures/scanmanager.h"
#include "Controllers/scandataprocesser.h"

class ScanSystemTester : public QObject
{
    Q_OBJECT
public:
    explicit ScanSystemTester(QObject *parent = 0);


public slots:
//    void USBConnected();
    void USBItems(QList<UI_USB_Item> f);

public:
    void processScanItem(UI_USB_Item item);

private:
//    USBMinder* um_;
//    USBManager* manager_;
    ScanManager* sm_;

};

#endif // SCANSYSTEMTESTER_H
