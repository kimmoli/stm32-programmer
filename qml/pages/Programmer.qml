import QtQuick 2.0
import Sailfish.Silica 1.0
import stm32.programmer.Stm32p 1.0
import "../components"

Page
{
    id: page

    Messagebox
    {
        id: messageBox
    }

    Timer
    {
        id: vddBusy
        interval: 2000
        onTriggered: vddControlSwitch.checked = true
    }

    SilicaFlickable
    {
        anchors.fill: parent

        PullDownMenu
        {
            MenuItem
            {
                text: "About..."
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"),
                                          { "version": stm32p.version,
                                              "year": "2014",
                                              "name": "STM32 Programmer",
                                              "imagelocation": "/usr/share/icons/hicolor/86x86/apps/stm32-programmer.png"} )
            }
        }

        contentHeight: column.height

        Column
        {
            id: column

            width: page.width
            spacing: Theme.paddingMedium

            PageHeader
            {
                title: "STM32 Programmer"
            }

            Button
            {
                id: selectFileButton
                text: qsTr("Select file")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: messageBox.showMessage("Not yet implemented", 1500)
            }
            Label
            {
                id: selectedFile
                text: qsTr("File: %1").arg(filename)
                anchors.horizontalCenter: parent.horizontalCenter
            }

            SectionHeader
            {
                text: qsTr("Target controls")
            }
            Row
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge

                TextSwitch
                {
                    id: vddControlSwitch
                    text: qsTr("Vdd control")
                    width: column.width/2 - 26
                    busy: vddBusy.running
                }
                Button
                {
                    id: resetButton
                    text: qsTr("Reset")
                    onClicked: vddBusy.restart()
                }
            }

            SectionHeader
            {
                text: qsTr("Programming controls")
            }

            Row
            {
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Theme.paddingLarge

                Button
                {
                    id: programButton
                    text: qsTr("Program")
                    onClicked: messageBox.showMessage("Not yet implemented", 1500)
                }
                Button
                {
                    id: verifyButton
                    text: qsTr("Verify")
                    onClicked: messageBox.showMessage("Not yet implemented", 1500)
                }

            }
            SectionHeader
            {
                text: qsTr("Progress")
            }

            ProgressBar
            {
                id: progressIndicator
                width: parent.width
                indeterminate: true
                label: qsTr("Status: %1").arg(programmingPhase)
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }
    }

    Stm32p
    {
        id: stm32p
    }
}


