#ifndef USBTESTER_H
#define USBTESTER_H

#include <QObject>
#include "Controllers/usbmanager.h"

class USBTester : public QObject
{
    Q_OBJECT
public:
    explicit USBTester(QObject *parent = 0);

signals:
    void deleteItem(QString id);


public slots:
    void uiUSBItemsUpdated(QList<UI_USB_Item> f);
};

#endif // USBTESTER_H
