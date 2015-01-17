#ifndef PATIENTSLISTMODEL_H
#define PATIENTSLISTMODEL_H

#include <QObject>
#include <QQmlListProperty>

#include "PatientObject.h"

#include <QDebug>

/**
 * @brief The PatientsListModel class
 *
 */
class PatientsListModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<QObject> list READ patientsList
               NOTIFY sigPatientsListModificate())
    Q_PROPERTY(QQmlListProperty<QObject> searchPatientsList READ searchPatientsList
               NOTIFY sigSearchListModificate())
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex
               NOTIFY sigCurrentIndexChanged)

public:
    explicit PatientsListModel(QObject *parent = 0);

    QQmlListProperty<QObject> patientsList();
    QQmlListProperty<QObject> searchPatientsList();

    Q_INVOKABLE PatientObject* getSpecificItem(const int &index) const;

    Q_INVOKABLE void append(PatientObject *patient);
    Q_INVOKABLE void setSearchListViaSpecificKey(const QString &key);
    Q_INVOKABLE void restartListModel();
    Q_INVOKABLE void sortWithSpecificOrder(const QString &order);

    int currentIndex() const;
    Q_INVOKABLE int size() const;

    void setCurrentIndex(int index);

signals:
    void sigPatientsListModificate() const;
    void sigCurrentIndexChanged() const;
    void sigSearchListModificate() const;

private:
    QList<QObject*> m_PatientsList;
    QList<QObject*> m_SearchPatientsList;
    int m_CurrentIndex;

private:
    /// "Sorting routine looks painfully slow.
    ///  It's not a problem if you need to sort a small list,
    ///  but this will quickly start to be a performance hit I think.
    ///  Use SortFilterProxyModel "
    ///  But in our application, especially in first poc form it should be enough to use sorting this way instead of QAbstractItemModel.
    static bool _sortViaNameAscending(QObject *o1, QObject *o2);
    static bool _sortViaNameDescending(QObject *o1, QObject *o2);
    static bool _sortViaDateTimeAscending(QObject *o1, QObject *o2);
    static bool _sortViaDateTimeDescending(QObject *o1, QObject *o2);

    void clearSearchList();
};

#endif // PATIENTSLISTMODEL_H
