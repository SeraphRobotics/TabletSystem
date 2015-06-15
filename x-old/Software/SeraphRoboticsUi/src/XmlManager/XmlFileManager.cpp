#include "XmlFileManager.h"

#include <QApplication>
#include <QDebug>
#include <QDomDocument>
#include <QDateTime>
#include <QXmlStreamWriter>

XmlFileManager::XmlFileManager(QObject *parent) :
    QObject(parent)
{
}

void XmlFileManager::saveShellModifications(const QString &direction,
                                            QList<UI_Shell_Modification> &elementList)
{
    qDebug()<<__FUNCTION__<<elementList.size();

    QString fileName(qApp->applicationDirPath());
    fileName.append(QString("/%1_%2.xml")
                    .arg(QDateTime::currentDateTime().toString("yyyy_d_M_hh_mm_ss"))
                    .arg(direction));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)){return;}

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeStartElement("Shell_Modifications");

    for(int i = 0; i <elementList.size(); i++)
    {
        UI_Shell_Modification &element = elementList[i];
        writer.writeStartElement("Pad");
        writer.writeTextElement("Name",element.name);
        writer.writeTextElement("height", QString::number(element.height));
        writer.writeTextElement("depth", QString::number(element.depth));
        writer.writeTextElement("Stiffness", QString::number(element.stiffness));

        QVector<QPointF> &borderElement = element.outer_border;

        writer.writeStartElement("OuterBorder");

        for(int i=0; i <borderElement.size(); i++)
        {
            QPointF &element = borderElement[i];

            writer.writeStartElement("point");
            writer.writeTextElement("x", QString::number(element.x()));
            writer.writeTextElement("y", QString::number(element.y()));
            writer.writeEndElement();
        }

        writer.writeEndElement();

        _saveInnerBorders(writer, element.inner_borders);
        writer.writeEndElement();
    }

    writer.writeEndElement();
    writer.writeEndDocument(); // End of document
    file.close();
}

void XmlFileManager::_saveInnerBorders(QXmlStreamWriter &writer,
                                       QList< std::vector <cv::Point> > &inner_borders)
{
    if(inner_borders.size() == 0)
        return;

    writer.writeStartElement("InnerBorders");
    for(int i=0; i <inner_borders.size(); i++)
    {
        writer.writeStartElement("loop");
        std::vector <cv::Point> &vectorElement = inner_borders[i];

        for(int i=0; i <vectorElement.size(); i++)
        {
            cv::Point &element = vectorElement[i];
            writer.writeStartElement("point");
            writer.writeTextElement("x", QString::number(element.x));
            writer.writeTextElement("y", QString::number(element.y));
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    writer.writeEndElement();

}
