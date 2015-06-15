#ifndef SLICERCONTROLLER_H
#define SLICERCONTROLLER_H

#include <QObject>
#include <QProcess>


class SlicerController : public QObject
{
    Q_OBJECT
public:
    explicit SlicerController(QString stlfile,QString inifile, float x=0, float y=0, bool isValved=false, QObject *parent = 0);

signals:
    void Success();
    void Failed(QString s);

public slots:
    void slice();
    void slicerError(QProcess::ProcessError error);

private:
    QString objfile_;
    QString inifile_;
    bool isvalved_;
    float x_;
    float y_;
};

#endif // SLICERCONTROLLER_H
