
import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow
{

    property string filename: "None"
    property string programmingPhase: "Waiting for user"

    initialPage: Qt.resolvedUrl("pages/Programmer.qml")

    cover: Qt.resolvedUrl("cover/CoverPage.qml")
}


