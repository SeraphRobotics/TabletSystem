#ifndef APPLICATIONSETTINGSMANAGER_H
#define APPLICATIONSETTINGSMANAGER_H

#include <QObject>

/**
   @brief The ApplicationSettingsManager class
   Class to store all application data, settings, file paths.
 */

class ApplicationSettingsManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool pinRequire READ pinRequire
               NOTIFY sigPinRequireChanged())
    Q_PROPERTY(bool loginByPrescriber READ loginByPrescriber
               NOTIFY sigLoginByPrescriberChanged())
    Q_PROPERTY(bool autoAssignPatients READ autoAssignPatients
               NOTIFY sigAutoAssignPatientsChanged())
public:
    static ApplicationSettingsManager& getInstance();

public slots:
    const QString& usersFilePathName() const;
    const QString& patientsFilePathName() const;
    const QString orderMaterialsUrl() const;
    const QString scanFilePathName() const;
    const QString contactSupportUrl() const;
    bool autoAssignPatients() const;
    bool loginByPrescriber() const;
    bool pinRequire() const;
    void setAutoAssignPatients(const bool&);
    void setLoginByPrescriber(const bool&);
    void setPinRequire(const bool&);
    void setStartingState() const;
    Q_INVOKABLE void saveAccountSettingsToIniFile(const bool& pinRequire,
                                                  const bool& loginByPrescriber,
                                                  const bool& autoAssignPatients);

signals:

    void setStateOnApplicationStart(const QString& startingState) const;
    void sigPinRequireChanged() const;
    void sigLoginByPrescriberChanged() const;
    void sigAutoAssignPatientsChanged() const;

private:
    explicit ApplicationSettingsManager(QObject *parent = 0);
    ApplicationSettingsManager(const ApplicationSettingsManager &);
    ApplicationSettingsManager& operator=(const ApplicationSettingsManager&);
    ~ApplicationSettingsManager() {}

    void _createSettingsFile();
    void _readSettingsFile();

private:
    QString m_CurrentUserName;
    QString m_ScanFilePathName;
    QString m_PatientsFilePathName;
    QString m_UsersFilePathName;
    QString m_OrthoticFilePathName;
    QString m_OrderMaterialsUrl;
    QString m_ContactSupportUrl;
    QString m_ApplicationIniFilePath;

    bool m_PinRequire;
    bool m_LoginByPrescriber;
    bool m_AutoAssignPatients;
};

#endif // APPLICATIONSETTINGSMANAGER_H
