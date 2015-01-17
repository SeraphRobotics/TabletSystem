#include "PatientObject.h"

#include <QDebug>

PatientObject::PatientObject(NameObject *name,
                             const QString &id,
                             const QString &doctorId,
                             QObject *parent) :
    QObject(parent),
    m_Id(id),
    m_DoctorId(doctorId),
    m_NameObject(name)

{
}

QString PatientObject::id() const
{
    return m_Id;
}

QString PatientObject::doctorId() const
{
    return m_DoctorId;
}

NameObject* PatientObject::name()
{
    return m_NameObject;
}

void PatientObject::setId(const QString &id)
{
    if(m_Id != id)
    {
        m_Id = id;
        emit sigIdChanged();
    }
}

void PatientObject::setName(NameObject *name)
{
    if(name != m_NameObject && name != nullptr)
    {
        m_NameObject = name;
        emit sigNameChanged();
    }
}

QQmlListProperty<QObject> PatientObject::scansList()
{
    return QQmlListProperty<QObject>(this, m_ScansList);
}

QQmlListProperty<QObject> PatientObject::rxList()
{
    return QQmlListProperty<QObject>(this, m_RxList);
}

void PatientObject::appendDataObject(const UI_USB_Item &object)
{
    PatientDataObject *patientDataObject = new PatientDataObject(this);

    patientDataObject->setDataTime(object.datetime);

    patientDataObject->setComment(object.comment);
    patientDataObject->setId(object.id);
    patientDataObject->setPatientName(new NameObject(object.patient.first,
                                                     object.patient.last,
                                                     object.patient.title,
                                                     this)
                                      );

    if(object.type == UI_USB_Item::kScan)
    {
        m_ScansList.append(patientDataObject);
        qDebug()<<"Scan added, Scan list size:"<<m_ScansList.size(); /// @todo
        emit sigScansListChanged();
    }
    else if(object.type == UI_USB_Item::kRx)
    {
        m_RxList.append(patientDataObject);
        qDebug()<<"Rx added, Rx list size:"<<m_RxList.size(); /// @todo
        emit sigRxListChanged();
    }
}

int PatientObject::scansListCurrentIndex() const
{
    return m_ScansListCurrentIndex;
}

int PatientObject::rxListCurrentIndex() const
{
    return m_RxListCurrentIndex;
}

void PatientObject::setScansListCurrentIndex(const int &currentIndex)
{
    if(m_ScansListCurrentIndex != currentIndex)
        m_ScansListCurrentIndex = currentIndex;
}

void PatientObject::setRxListCurrentIndex(const int &currentIndex)
{
    if(m_RxListCurrentIndex != currentIndex)
        m_RxListCurrentIndex = currentIndex;
}

void PatientObject::sortRxViaDate()
{
    std::sort(m_RxList.begin(), m_RxList.end(), PatientObject::_dateTimeSortFunction);
}

bool PatientObject::_dateTimeSortFunction(QObject *o1, QObject *o2)
{
    PatientDataObject *first = qobject_cast<PatientDataObject*>(o1);
    PatientDataObject *second = qobject_cast<PatientDataObject*>(o2);

    return  first->dateTime()
            < second->dateTime();
}

QDateTime PatientObject::dateTime() const
{
    return qobject_cast<PatientDataObject*>(m_RxList.first())->dateTime();
}
