#include "ApplicationSettingsManager.h"

#include "QSettings"
#include <QtGui/QGuiApplication>
#include <QDebug>
#include <QDir>
#include <QFile>


ApplicationSettingsManager::ApplicationSettingsManager(QObject *parent) :
    QObject(parent)

{
#ifdef Q_OS_WIN
    m_ApplicationIniFilePath = static_cast<QString>
            (getenv("APPDATA"))+"/Seraph/SeraphData.ini";
#endif

#ifdef Q_OS_LINUX
    m_ApplicationIniFilePath = qApp->applicationDirPath()+"/settings.ini";
#endif

    if(QFile::exists(m_ApplicationIniFilePath))
    {
        _readSettingsFile();
    }
    else
    {
        _createSettingsFile();
    }
}

ApplicationSettingsManager& ApplicationSettingsManager::getInstance()
{
    static ApplicationSettingsManager instance;
    return instance;
}

void ApplicationSettingsManager::_createSettingsFile()
{
    qDebug()<<QString("Not found settings file in %1, create now").
              arg(m_ApplicationIniFilePath);

    QSettings settings(m_ApplicationIniFilePath, QSettings::IniFormat);
    settings.setValue("scan-extension", "scan");
    settings.setValue("ortho-extension", "ortho");
    settings.setValue("ortho-directory",qApp->applicationDirPath());

    settings.setValue("patient-file",
                      qApp->applicationDirPath().append("/patients.xml"));
    settings.setValue("users-file",
                      qApp->applicationDirPath().append("/users.xml"));

    settings.sync();
    _readSettingsFile();
}

void ApplicationSettingsManager::_readSettingsFile()
{
    QSettings settings(m_ApplicationIniFilePath, QSettings::IniFormat);

    m_PatientsFilePathName = settings.value("patient-file").toString();
    m_UsersFilePathName = settings.value("users-file").toString();
    m_ScanFilePathName = settings.value("scan-file").toString();
    m_OrderMaterialsUrl = settings.value("orderMaterialsUrl").toString();
    m_ContactSupportUrl = settings.value("contactSupportUrl").toString();
    m_PinRequire = settings.value("pinRequire").toBool();
    m_AutoAssignPatients = settings.value("autoAssignPatients").toBool();
    m_LoginByPrescriber = settings.value("loginByPrescriber").toBool();
}

const QString& ApplicationSettingsManager::usersFilePathName() const
{
    return m_UsersFilePathName;
}

const QString& ApplicationSettingsManager::patientsFilePathName() const
{
    return m_PatientsFilePathName;
}

void ApplicationSettingsManager::setStartingState() const
{
    QSettings settings(m_ApplicationIniFilePath,QSettings::IniFormat);

    if(settings.value("first-run").isNull() == true)
    {
        settings.setValue("first-run",false);
        emit setStateOnApplicationStart("welcomeScreenState");
    }
    else
    {
        emit setStateOnApplicationStart("startScreenState");
    }
}

const QString ApplicationSettingsManager::orderMaterialsUrl() const
{
    return m_OrderMaterialsUrl;
}

const QString ApplicationSettingsManager::contactSupportUrl() const
{
    return m_ContactSupportUrl;
}

const QString ApplicationSettingsManager::scanFilePathName() const
{
    return m_ScanFilePathName;
}

void ApplicationSettingsManager::saveAccountSettingsToIniFile(const bool& pinRequire,
                                                              const bool& loginByPrescriber,
                                                              const bool& autoAssignPatients)
{
    QSettings settings(m_ApplicationIniFilePath,QSettings::IniFormat);

    settings.setValue("pinRequire",pinRequire);
    settings.setValue("loginByPrescriber", loginByPrescriber);
    settings.setValue("autoAssignPatients", autoAssignPatients);

    setLoginByPrescriber(loginByPrescriber);
    setAutoAssignPatients(autoAssignPatients);
    setPinRequire(pinRequire);
}

bool ApplicationSettingsManager::autoAssignPatients() const
{
    qDebug()<<__FUNCTION__<<m_AutoAssignPatients;
    return m_AutoAssignPatients;
}

bool ApplicationSettingsManager::loginByPrescriber() const
{
    qDebug()<<__FUNCTION__<<m_LoginByPrescriber;
    return m_LoginByPrescriber;
}

bool ApplicationSettingsManager::pinRequire() const
{
    qDebug()<<__FUNCTION__<<m_PinRequire;
    return m_PinRequire;
}

void ApplicationSettingsManager::setAutoAssignPatients(const bool& autoAssign)
{
    if(m_AutoAssignPatients != autoAssign)
    {
        m_AutoAssignPatients = autoAssign;
        emit sigAutoAssignPatientsChanged();
    }
}

void ApplicationSettingsManager::setLoginByPrescriber(const bool& loginByPrescriber)
{
    if(m_LoginByPrescriber != loginByPrescriber)
    {
        m_LoginByPrescriber = loginByPrescriber;
        emit sigLoginByPrescriberChanged();
    }
}

void ApplicationSettingsManager::setPinRequire(const bool& pinRequire)
{
    if(m_PinRequire != pinRequire)
    {
        m_PinRequire = pinRequire;
        emit sigPinRequireChanged();
    }
}
