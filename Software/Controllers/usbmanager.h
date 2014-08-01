#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <QObject>
#include "View/UI_structs.h"
#include <QMap>

/**
 * The Goal of this object is to scan the USBKey and find the scans, and update the list of UI_USB_Items
 **/
class USBManager : public QObject
{
    Q_OBJECT
public:
    explicit USBManager(QObject *parent = 0);

    QString getLocation(QString id);

signals:

    /**
     * UI facing signal for the USB control widget
     **/
    void usbConnected();

    /**
     * UI facing signal for the USB control widget
     **/
    void usbDisconnected();

    /**
     * UI facing signal for the USB control widget
     **/
    void uiUSBItemsUpdated(QList<UI_USB_Item> f);


public slots:
    /**
     * UI facing signal for the USB control widget
     * Deletes the file or folder associated with the id if it exists
     **/
    void deleteItem(QString id);


private slots:

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

    /**
     * writes UI_USB_Items into USBData.xml files
     **/
    void updateUSBData();

private:
    QMap < QString, UI_USB_Item > items_;
    QString USBDir_;
    QString Rx_extension;

};


QDomNode nodeFromUSBItem(UI_USB_Item i);
UI_USB_Item makeUSBItemFromNode(QDomNode node);
bool removeDir(const QString & dirName);
#endif // USBMANAGER_H
