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
        onTriggered: stm32p.vddState = true
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
                onClicked:
                {
                    var fileSelectorDialog = pageStack.push(Qt.resolvedUrl("FileSelector.qml"),
                                                            {"selectedFile": filename})

                    fileSelectorDialog.accepted.connect(function()
                    {
                        filename = fileSelectorDialog.selectedFile
                    })
                }
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
                    automaticCheck: false
                    checked: stm32p.vddState
                    onClicked: stm32p.vddState = !stm32p.vddState
                }
                Button
                {
                    id: resetButton
                    text: qsTr("Reset")
                    onClicked:
                    {
                        stm32p.vddState = false
                        vddBusy.restart()
                    }
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
                    onClicked: stm32p.startProgram()
                }
                Button
                {
                    id: verifyButton
                    text: qsTr("Verify")
                    onClicked: stm32p.startVerify()
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
                indeterminate: stm32p.progress === 0
                minimumValue: 0
                maximumValue: 100
                label: stm32p.statusMsg
                value: stm32p.progress
                anchors.horizontalCenter: parent.horizontalCenter
            }

        }
    }

    Stm32p
    {
        id: stm32p
        onErrorMsgChanged: messageBox.showMessage(errorMsg, 0)
    }
}


