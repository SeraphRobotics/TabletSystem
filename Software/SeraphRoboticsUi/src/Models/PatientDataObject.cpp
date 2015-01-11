#include "PatientDataObject.h"

#include <QDebug>

PatientDataObject::PatientDataObject(QObject *parent) :
    QObject(parent)
{

}

QDateTime PatientDataObject::dateTime() const
{
    return m_DateTime;
}

QString PatientDataObject::id() const
{
    return m_Id;
}

QString PatientDataObject::comment() const
{
    return m_Comment;
}

NameObject* PatientDataObject::patientName() const
{
    return m_PatientName;
}

void PatientDataObject::setDataTime(const QDateTime &dateTime)
{
    if(m_DateTime != dateTime)
    {
        m_DateTime = dateTime;
        emit sigDateTimeChanged();
    }
}

void PatientDataObject::setId(const QString &id)
{
    if(m_Id != id)
        m_Id = id;
}

void PatientDataObject::setComment(const QString &comment)
{
    if(m_Comment != comment)
    {
        m_Comment = comment;
        emit sigCommentChanged();
    }
}

void PatientDataObject::setPatientName(NameObject *name)
{
    if(m_PatientName != name && name != nullptr)
    {
        m_PatientName = name;
        emit sigPatientNameChanged();
    }
}

