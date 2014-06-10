import QtQuick 2.0
import Sailfish.Silica 1.0
import stm32.programmer.Stm32p 1.0
import stm32.programmer.Filemodel 1.0

Dialog
{
    id: fileSelectorDialog

    canAccept: selectedFile !== "None"

    property string selectedFile: "None"

    onStatusChanged:
    {
        if (status === DialogStatus.Opened)
        {
            filemodel.filter = [ "*.hex" ]
            filemodel.sorting = false
            filemodel.path = "home"
        }
    }

    Filemodel
    {
        id: filemodel
        onCountChanged: header.title = filemodel.count > 0 ? qsTr("Select file") : qsTr("File not found")
    }

    DialogHeader
    {
        id: header
        title: qsTr("Loading...")
    }

    Loader
    {
        id: viewLoader
        anchors
        {
            fill: parent
            topMargin: header.height
        }
        sourceComponent: listComponent
    }

    Component
    {
        id: listComponent
        SilicaListView
        {
            model: filemodel
            delegate: listDelegate
            clip: true

            VerticalScrollDecorator {}
        }
    }

    Component
    {
        id: listDelegate
        BackgroundItem
        {
            id: item
            width: parent.width
            height: Theme.itemSizeSmall
            highlighted: down || selectedFile === model.path

            Image
            {
                id: icon
                source: "image://theme/icon-m-developer-mode"
                cache: true
                asynchronous: false
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingSmall
            }

            Item
            {
                height: file.height
                anchors
                {
                    left: icon.right
                    right: parent.right
                    margins: Theme.paddingLarge
                    verticalCenter: parent.verticalCenter
                }
                clip: true

                Label
                {
                    id: file
                    text: model.name
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: Theme.fontSizeMedium
                    color: item.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

            }

            onClicked:
            {
                selectedFile = selectedFile === model.path ? "None" : model.path
            }
        }
    }
}
