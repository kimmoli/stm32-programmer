import QtQuick 2.0
import Sailfish.Silica 1.0
import stm32-programmer.Stm32p 1.0

Page
{
    id: page

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
            spacing: Theme.paddingLarge
            PageHeader
            {
                title: "Stm32-programmer"
            }
            Label
            {
                x: Theme.paddingLarge
                text: "Hello you"
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraLarge
            }
        }
    }

    Stm32p
    {
        id: stm32p
    }
}


