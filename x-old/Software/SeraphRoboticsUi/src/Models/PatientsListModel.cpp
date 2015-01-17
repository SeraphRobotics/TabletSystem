#include "PatientsListModel.h"

#include <QDebug>

PatientsListModel::PatientsListModel(QObject *parent) :
    QObject(parent),
    m_CurrentIndex(0)
{
}

QQmlListProperty<QObject> PatientsListModel::patientsList()
{
    return QQmlListProperty<QObject>(this, m_PatientsList);
}

QQmlListProperty<QObject> PatientsListModel::searchPatientsList()
{
    return QQmlListProperty<QObject>(this, m_SearchPatientsList);
}

void PatientsListModel::setSearchListViaSpecificKey(const QString &key)
{
    clearSearchList();

    if(key == "")
    {
        return;
    }

    for(int i = 0; i < m_PatientsList.size(); i++)
    {
        PatientObject *obj = qobject_cast<PatientObject*> (m_PatientsList.at(i));

        if(obj->name()->firstName().contains(key, Qt::CaseInsensitive)
                || obj->name()->lastName().contains(key, Qt::CaseInsensitive)
                || obj->name()->title().contains(key, Qt::CaseInsensitive))
        {
            m_SearchPatientsList.append(obj);
            emit sigSearchListModificate();
        }
    }
}

void PatientsListModel::clearSearchList()
{
    //// @todo we need to create one class for list model and store this function in this class
    ///  or at least use qqmllistproperty.html and add speciics function. ClearFunction,etc
    ///
    m_SearchPatientsList.clear();
    emit sigSearchListModificate();
}

void PatientsListModel::restartListModel()
{
    clearSearchList();
    sortWithSpecificOrder("A-Z");
}

void PatientsListModel::append(PatientObject *patient)
{
    m_PatientsList.append(patient);
    emit sigPatientsListModificate();
}

PatientObject* PatientsListModel::getSpecificItem(const int &index) const
{
    if(index < 0 || index > m_PatientsList.size()-1)
    {
        qDebug()<<"Out of list range";
        return 0;
    }
    return qobject_cast<PatientObject*>(m_PatientsList.at(index));
}

int PatientsListModel::currentIndex() const
{
    return m_CurrentIndex;
}

void PatientsListModel::setCurrentIndex(int index)
{
    qDebug()<<__FUNCTION__<<index;
    m_CurrentIndex = index;
    emit sigCurrentIndexChanged();
}

/**
   @brief PatientsListModel::sortWithSpecificOrder
   @param order - "A-Z" - ascending , "Z-A" - descending sort
 */
void PatientsListModel::sortWithSpecificOrder(const QString &order)
{
    if(order == "A-Z")
        std::sort(m_PatientsList.begin(), m_PatientsList.end(), PatientsListModel::_sortViaNameAscending);
    else if(order == "Z-A")
        std::sort(m_PatientsList.begin(), m_PatientsList.end(), PatientsListModel::_sortViaNameDescending);
    else if(order == "newest-oldest")
        std::sort(m_PatientsList.begin(), m_PatientsList.end(), PatientsListModel::_sortViaDateTimeDescending);
    else if(order == "oldest-newest")
        std::sort(m_PatientsList.begin(), m_PatientsList.end(), PatientsListModel::_sortViaDateTimeAscending);

    emit sigPatientsListModificate();
}

bool PatientsListModel::_sortViaNameAscending(QObject *o1, QObject *o2)
{
    PatientObject *first = qobject_cast<PatientObject*>(o1);
    PatientObject *second = qobject_cast<PatientObject*>(o2);

    return  first->name()->firstName().toLower()
            < second->name()->firstName().toLower();
}

bool PatientsListModel::_sortViaNameDescending(QObject *o1, QObject *o2)
{
    PatientObject *first = qobject_cast<PatientObject*>(o1);
    PatientObject *second = qobject_cast<PatientObject*>(o2);

    return  first->name()->firstName().toLower()
            > second->name()->firstName().toLower();
}

bool PatientsListModel::_sortViaDateTimeAscending(QObject *o1, QObject *o2)
{
    qDebug()<<__FUNCTION__;
    PatientObject *first = qobject_cast<PatientObject*>(o1);
    PatientObject *second = qobject_cast<PatientObject*>(o2);

    return  first->dateTime()
            < second->dateTime();
}

bool PatientsListModel::_sortViaDateTimeDescending(QObject *o1, QObject *o2)
{
    PatientObject *first = qobject_cast<PatientObject*>(o1);
    PatientObject *second = qobject_cast<PatientObject*>(o2);

    return  first->dateTime()
            > second->dateTime();
}

int PatientsListModel::size() const
{
    return m_PatientsList.size();
}


