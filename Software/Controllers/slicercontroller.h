#ifndef SLICERCONTROLLER_H
#define SLICERCONTROLLER_H

#include <QObject>

class SlicerController : public QObject
{
    Q_OBJECT
public:
    explicit SlicerController(QString stlfile,QString inifile,bool isValved, QObject *parent = 0);

signals:
    void Success();
    void Failed(QString s);

public slots:
    void slice();

private:
    QString objfile_;
    QString inifile_;
    bool isvalved_;
};

#endif // SLICERCONTROLLER_H
