#include "usbtester.h"
#include <QTimer>
#include <QDebug>

USBTester::USBTester(QObject *parent) :
    QObject(parent)
{
}

void USBTester::uiUSBItemsUpdated(QList<UI_USB_Item> f){
    qDebug()<<"called: "<<f.size();
    if (f.size()>0){
        emit deleteItem( f[0].id);
    }
}
