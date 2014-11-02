#ifndef MERGECONTROLLER_H
#define MERGECONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QDomElement>

struct file_z_pair{
    QString file;
    float z;
};

QDomElement nodeFromPair(QString name, file_z_pair pair);

class MergeController : public QObject
{
    Q_OBJECT
public:
    explicit MergeController(file_z_pair shell, QList< file_z_pair>  pads, file_z_pair topcoat, QObject *parent = 0);

signals:
    void GCodeMerged(QString gcode);
    void Success();
    void Failed(QString s);

public slots:
    void mergeFiles();

private:
    file_z_pair shell_;
    QList< file_z_pair> pad_files_;
    file_z_pair topcoat_file_;
};

#endif // MERGECONTROLLER_H
