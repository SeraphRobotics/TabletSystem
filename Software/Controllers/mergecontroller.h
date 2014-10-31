#ifndef MERGECONTROLLER_H
#define MERGECONTROLLER_H

#include <QObject>
#include <QStringList>

class MergeController : public QObject
{
    Q_OBJECT
public:
    explicit MergeController(QStringList files, QObject *parent = 0);

signals:
    void GCodeMerged(QString gcode);
    void Success();
    void Failed(QString s);

public slots:
    void mergeFiles();

private:
    QStringList files_;
};

#endif // MERGECONTROLLER_H
