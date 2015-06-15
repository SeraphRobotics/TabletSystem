#ifndef RECORDKEEPER_H
#define RECORDKEEPER_H

#include <QObject>
#include <QDomDocument>
#include <QTimer>
enum kTimeType{kOn,kOff,kScan};

class recordkeeper : public QObject
{
    Q_OBJECT


public:
    explicit recordkeeper(QObject *parent = 0);

private slots:
    void updateTime();
    void appendTime(kTimeType type,qint64 timeInMSSinceEpoch);
    void USBAdded(QString addr);

private:
    QDomDocument xmlrecord_;
    QTimer* timer_;
};

#endif // RECORDKEEPER_H
