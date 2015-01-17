import QtQuick 2.4

import "RegularScreens"
import "RegularScreens/ChoosePatientScreenPrivateComponents" 1.0
import "./Components" 1.0
import "./RegularScreens/SettingsPagePrivateComponents/" 1.0

/*
 Main QtQuick Item which is our root item, here we switch beetwen all screens in applications.
*/
Loader {
    id : mainApplicationObject
    objectName : "mainApplicationObject"

    // We need to keep starting size due to the canvas issue.
    width : 1280
    height : 800

    //Value used to keep height for bottom right corner logo element.
    property int logoHeight: 70

    anchors
    {
        fill : parent
        centerIn: parent
    }

    // Page 20-30 from pdf. Item used for showing pads.
    // We don't want to reload page again and again, that's why we keep it created.
    SettingsPageScreen {
        id : settingsPage
        objectName: "settingsPage"

        opacity : mainApplicationObject.state == "settingsPageScreen" ? 1 : 0
        enabled : mainApplicationObject.state == "settingsPageScreen" ? true : false
    }

    // Page manager (state machine), gets state from stateManager cpp
    // and switches to proper screen.
    Connections {
        target : stateManager

        onSigSwitchToSpecificScreen:
        {
            console.log("From qml page state manager function, got state "+pageName)

            source = ""
            mainApplicationObject.state = pageName

            if(pageName == "summaryPage")
                source = "InitialSetupWizard/SummaryPage.qml"
            else if(pageName == "saveSettings")
                source = "InitialSetupWizard/SaveSettingsPage.qml"
            else if(pageName == "noInternetConnection")
                source = "InitialSetupWizard/NoInternetConnectionPage.qml"
            else if(pageName == "customerNumberPageState")
                source = "InitialSetupWizard/CustomerNumberPage.qml"
            else if(pageName == "accountSetupState")
                source = "InitialSetupWizard/AccountSetupPage.qml"
            else if(pageName == "welcomeScreenState")
                source = "InitialSetupWizard/WelcomePage.qml"
            else if(pageName == "editaccountSetupPage")
                source = "InitialSetupWizard/EditAccountSetupPage.qml"
            else if(pageName == "editaccountSettingsPage")
                source = "InitialSetupWizard/EditAccountSettingsPage.qml"
            else if(pageName == "startScreenState")
                source = "RegularScreens/StartPage.qml"
            else if(pageName == "choosePatientState")
                source = "RegularScreens/ChoosePatientPage.qml"
            else if(pageName == "patientHistoryScreen")
                source = "RegularScreens/PatientHistoryPage.qml"
            else if(pageName == "main3dView")
                source = "RegularScreens/SettingsPagePrivateComponents/ReviewSubpage/Main3dView.qml"
        }
    }

    Component.onCompleted:
    {
        SettingsPageComponentsSettings.m_MainRootObject = mainApplicationObject

        // Set up parent property for singleton popups.
        FramePopup.parent = mainApplicationObject
        TutorialWindowPopup.parent = mainApplicationObject
        GrayPopup.parent = mainApplicationObject
    }
}
