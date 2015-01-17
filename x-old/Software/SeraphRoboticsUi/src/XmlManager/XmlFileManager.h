#ifndef XMLFILEMANAGER_H
#define XMLFILEMANAGER_H

#include <QObject>
#include <QFile>
#include "UI_structs.h"

class QXmlStreamWriter;

/**
 * @brief The XmlFileManager class
 * Class used to save pads data to xml file.
 */
class XmlFileManager : public QObject
{
    Q_OBJECT
public:
    explicit XmlFileManager(QObject *parent = 0);

public slots:
    void saveShellModifications(const QString &direction,
                                             QList<UI_Shell_Modification> &elementList);

private:
    void _saveInnerBorders(QXmlStreamWriter &writer,
                           QList< std::vector <cv::Point> > &inner_borders);
};

#endif // XMLFILEMANAGER_H
