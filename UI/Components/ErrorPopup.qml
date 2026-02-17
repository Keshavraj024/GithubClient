import QtQuick
import QtQuick.Controls
import "../Controls"


Popup {
    id: root

    modal: false
    focus: true
    width: contentItem.implicitWidth + 50
    height: contentItem.implicitHeight + 50
    padding: 15

    anchors.centerIn: parent

    property string message

    background: Rectangle {
        radius: 12
        color: Theme.palette.background
        border.color: Theme.palette.border
        border.width: 1

        ColorBehavior on color {}
        ColorBehavior on border.color {}

    }

    contentItem: Column {
        spacing: 10
        anchors.centerIn: parent
        Text {
            text: "⚠ Network Error"
            color: Theme.palette.textPrimary
            font.bold: true
            font.pixelSize: 18
            anchors.horizontalCenter: parent.horizontalCenter

            ColorBehavior on color {}
        }

        Text {
            text: root.message
            color: Theme.palette.textSecondary
            wrapMode: Text.Wrap
            font.pixelSize: 20
            anchors.horizontalCenter: parent.horizontalCenter

            ColorBehavior on color {}
        }
    }

    Component.onCompleted : {
        root.open()
    }
}
