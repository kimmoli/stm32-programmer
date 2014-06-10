import QtQuick 2.0
import Sailfish.Silica 1.0

Rectangle
{
    id: messagebox
    z: 20
    width: parent.width
    height: Theme.itemSizeSmall
    opacity: 0.0
    anchors.top: parent.top
    color: Theme.highlightBackgroundColor

    signal msgBoxAccepted()

    function showMessage(message, delay)
    {
        messageboxText.text = message
        messagebox.opacity = 1.0
        if (delay > 0) /* Delay = 0  makes message persistent until clicked */
        {
            messageboxVisibility.interval = delay
            messageboxVisibility.restart()
        }
    }

    Label
    {
        id: messageboxText
        text: ""
        anchors.centerIn: parent
        color: "black"
    }

    Behavior on opacity
    {
        FadeAnimation {}
    }

    Timer
    {
        id: messageboxVisibility
        interval: 3000
        onTriggered: messagebox.opacity = 0.0
    }

    BackgroundItem
    {

        anchors.fill: parent
        onClicked:
        {
            messageboxVisibility.stop()
            messagebox.opacity = 0.0
            msgBoxAccepted()
        }
    }
}

