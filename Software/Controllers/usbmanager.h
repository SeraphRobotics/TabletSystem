#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <QObject>
#include "View/UI_structs.h"

/**
 * The Goal of this object is to scan the USBKey and find the scans, and update the list of UI_USB_Items
 **/
class USBManager : public QObject
{
    Q_OBJECT
public:
    explicit USBManager(QObject *parent = 0);

signals:

    /**
     * UI facing signal for the USB control widget
     **/
    void usbConnected();

    /**
     * UI facing signal for the USB control widget
     **/
    void usbDisconnected();

    void uiUSBItemsUpdated(QList<UI_USB_Item> f);


public slots:

    /**
     * connects to the USB minder thread and causes the system to process the usb key
     **/
    void usbAdded(QString f);

    /**
     * connects to the USB minder thread and causes the system to process the usb key removal
     **/
    void usbDisconnected(QString f);

private:
    /**
     * generates the UI_USB_Items from the USBData.xml file on usb
     **/
    void proccessUSBDrive();

private:
    QList<UI_USB_Item> items_;
    QString USBDir_;
};

UI_USB_Item makeUSBItemFromNode(QDomNode node);
#endif // USBMANAGER_H
