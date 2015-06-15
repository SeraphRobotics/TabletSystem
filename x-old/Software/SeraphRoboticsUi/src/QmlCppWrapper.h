#ifndef QMLCPPWRAPPER_H
#define QMLCPPWRAPPER_H

#include <QObject>

#include "mastercontrolunit.h"

#include "Models/UsersListModel.h"
#include "Models/PatientsListModel.h"

class ImageContoursDetector;
class XmlFileManager;

/// @brief Class used to wrap c++ interface into QtQuick.
class QmlCppWrapper : public QObject
{
    Q_OBJECT

    /// @note Take a look at m_iFrameUrl description.
    Q_PROPERTY(QString iFrameUrl READ iFrameUrl WRITE setiFrameUrl
               NOTIFY sigiFrameUrlChanged)
public:
    explicit QmlCppWrapper(QObject *parent = 0);
    ~QmlCppWrapper();

public:
    /** Import Users list from xml to QtQuick
     */
    UsersListModel* getUsersList();

    /** Import Patients list from xml to QtQuick
     */
    PatientsListModel* getPatientsList();

    /** @note take a look at m_iFrameUrl description.*/
    QString iFrameUrl() const;
    void setiFrameUrl(const QString& iFrameUrl);

    /** Returns main application path which is used in qml to save customized or created pads
     * to image folder, more details about this process are in c_ImagesFolderPath description*/
    Q_INVOKABLE QString getApplicationPath() const;

    /** Qml helper for adding file:/// correctly into image directory.
     * for some odd reason Qt.resolvedUrl() doesn't work here*/
    Q_INVOKABLE QString resolveUrl(const QString &fileName);

    /** Saves pad data to xml file.*/
    Q_INVOKABLE void saveShellModifications();

    /** Append pad from Qml View3d ui element to list, which is used to save them to xml files later
     * @param - direction(left, right).
     * @param - name(Pad name).
     * @param - depth, height, stiffness - Customized values for specific pad.
     * @param - imageData - Image that contains specific pad view area.
     * @param - startingXPos, startingYPos - Starting x,y position within foot coordinate system.
     */
    Q_INVOKABLE void appendElementToShellList(const QString &direction,
                                              const QString &name,
                                              const double &depth,
                                              const double &height,
                                              const double &stiffness,
                                              const QImage &imageData,
                                              const int &startingXPos,
                                              const int &startingYPos);

    /** Checks if image destination folder exists and creates a new one if not,
     * see description of c_ImagesFolderPath to learn more about this */
    Q_INVOKABLE void _createImagesDirectoryIfNotExist() const;


signals:
    void sigiFrameUrlChanged() const;

private:
    void _setUsersList();
    void _setPatientsList();

private:
    MasterControlUnit m_MasterControlUnit;
    XmlFileManager *m_XmlManager;

    UsersListModel* m_UsersList;
    PatientsListModel* m_PatientsList;

    ImageContoursDetector *m_ImageContoursDetector; /// @note - Class for borders detection.

    QString m_iFrameUrl; /// @note current popup showing url Activated via Tutorial button.
    ///@note Or on Page 9 via order materials and contact for support text links.

    const QString c_ImagesFolderPath;
    /// @note path for folder where application saving all custom created pads images.
    /// Currently application just save it on application working and delete or application
    ///  This way we can easily add all saved pad always on application start.If we want.

    /// @noteList which contains all pads for specific foot.List is create via  appendElementToShellList
    /// and save to xml files via saveShellModifications function.
    QList<UI_Shell_Modification> m_LeftElementShells;
    QList<UI_Shell_Modification> m_RightElementShells;
};

#endif // QMLCPPWRAPPER_H
