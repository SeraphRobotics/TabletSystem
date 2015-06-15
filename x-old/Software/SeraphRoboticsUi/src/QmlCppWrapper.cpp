#include "QmlCppWrapper.h"

#include "Models/NameObject.h"
#include "Models/UserObject.h"

#include "XmlManager/XmlFileManager.h"
#include "ImageProcessingClass/ImageContoursDetector.h"

#include <QDebug>

QmlCppWrapper::QmlCppWrapper(QObject *parent) :
    QObject(parent),
    c_ImagesFolderPath(qApp->applicationDirPath().append("/customPad"))
{
    m_UsersList = new UsersListModel(this);
    m_PatientsList = new PatientsListModel(this);
    m_XmlManager = new XmlFileManager(this);
    m_ImageContoursDetector = new ImageContoursDetector(this);


    qRegisterMetaType<NameObject*>("NameObject*");
    qRegisterMetaType<UserObject*>("UserObject*");
    qRegisterMetaType<PatientObject*>("PatientObject*");
    qRegisterMetaType<PatientDataObject*>("PatientDataObject*");
    qRegisterMetaType<QQmlListProperty<UserObject>>("QQmlListProperty<UserObject>");

    _setUsersList();
    _setPatientsList();

    QObject::connect(m_UsersList, &UsersListModel::sigUsersListReset,
                     this, &QmlCppWrapper::_setUsersList);
    QObject::connect(m_UsersList, SIGNAL(sigSaveUserList(QList<QObject*> *)),
                     &m_MasterControlUnit, SLOT(updateUserList(QList<QObject*>*)));

    _createImagesDirectoryIfNotExist();
}

QmlCppWrapper::~QmlCppWrapper()
{
    QDir(c_ImagesFolderPath).removeRecursively();
}

void QmlCppWrapper::_setUsersList()
{
    int size = m_UsersList->size();

    for(int i = size; i >= 0; i--)
    {
        m_UsersList->removeAt(i);
    }

    QList< User > users = m_MasterControlUnit.getUsersList();

    for(int i=0; i<users.size(); i++)
    {
        /// @note delete { and } characters from id numbers.
        QRegExp regExp("[{}]");
        QString id = users.at(i).id;
        id.remove(regExp);

        m_UsersList->appendNewUser(new UserObject
                                   (
                                       new NameObject(users.at(i).name.first,
                                                      users.at(i).name.last,
                                                      users.at(i).name.title,
                                                      this),
                                       id,
                                       users.at(i).pwd,
                                       users.at(i).iconfile,
                                       "", //degree
                                       this
                                       )

                                   );
    }
    qDebug()<<"Users list size is "<<m_UsersList->size();
}

PatientsListModel* QmlCppWrapper::getPatientsList()
{
    return m_PatientsList;
}

void QmlCppWrapper::_setPatientsList()
{
    qDebug()<<__FUNCTION__;
    QList< UI_Patient > patients = m_MasterControlUnit.getPatientsList();

    for(int i=0; i<patients.size(); i++)
    {
        PatientObject *patientObject =  new PatientObject(
                    new NameObject(patients.at(i).name.first,
                                   patients.at(i).name.last,
                                   patients.at(i).name.title,
                                   this),
                    patients.at(i).id,
                    patients.at(i).doctorid,
                    this
                    );

        for(UI_USB_Item obj : patients.at(i).item_list)
        {
            patientObject->appendDataObject(obj);
        }
        patientObject->sortRxViaDate();
        m_PatientsList->append(patientObject);

    }
    m_PatientsList->sortWithSpecificOrder("A-Z");
    qDebug()<<"PatientsList size is "<<m_PatientsList->size();
}

UsersListModel* QmlCppWrapper::getUsersList()
{
    return m_UsersList;
}

QString QmlCppWrapper::iFrameUrl() const
{
    return m_iFrameUrl;
}

void QmlCppWrapper::setiFrameUrl(const QString& iFrameUrl)
{
    if(m_iFrameUrl != iFrameUrl)
    {
        m_iFrameUrl = iFrameUrl;
        emit sigiFrameUrlChanged();
    }
}

QString QmlCppWrapper::getApplicationPath() const
{
    return qApp->applicationDirPath();
}

void  QmlCppWrapper::saveShellModifications()
{
    if(m_LeftElementShells.size() > 0)
        m_XmlManager->saveShellModifications("left", m_LeftElementShells);
    if(m_RightElementShells.size() > 0)
        m_XmlManager->saveShellModifications("right", m_RightElementShells);

    m_LeftElementShells.clear();
    m_RightElementShells.clear();
}

void QmlCppWrapper::appendElementToShellList(const QString &direction,
                                             const QString &name,
                                             const double &depth,
                                             const double &height,
                                             const double &stiffness,
                                             const QImage &image,
                                             const int &startingXPos,
                                             const int &startingYPos)
{

    qDebug()<<__FUNCTION__<<" Direction "<<direction<<" Name "<<name;

    QVariantList outerBorder;
    UI_Shell_Modification element;
    element.name = name;
    element.depth = depth;
    element.height = height;
    element.stiffness = stiffness;

    m_ImageContoursDetector->detectContours(image,
                                            element.inner_borders,
                                            outerBorder,
                                            startingXPos,
                                            startingYPos);

    /// @note Convert qvariant to QPointF
    QListIterator<QVariant> itr(outerBorder);
    while (itr.hasNext())
    {
        element.outer_border << itr.next().toPointF();
    }

    if(direction == "left")
    {
        m_LeftElementShells.append(element);
        qDebug()<< m_LeftElementShells[m_LeftElementShells.size()-1].name;
    }
    else if(direction == "right")
    {
        m_RightElementShells.append(element);
        qDebug()<< m_RightElementShells[m_RightElementShells.size()-1].name;
    }
}

void QmlCppWrapper::_createImagesDirectoryIfNotExist() const
{
    QDir().mkdir(c_ImagesFolderPath);
}

QString QmlCppWrapper::resolveUrl(const QString &fileName)
{
    QString name = QUrl::fromLocalFile(fileName).toString();
    qDebug()<<__FUNCTION__<<"Got "<<name;
    return name;
}
