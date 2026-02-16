import QtQuick
import QtQuick.Controls

import "../Controls"

Item {
    id: root

    property string buttonText: ""

    signal buttonClicked

    Button {
        id: customButton

        flat: true
        hoverEnabled: true
        anchors.fill: parent

        background: Rectangle {
            anchors.fill: parent
            radius: 15
            color: customButton.hovered ? Theme.palette.surfaceHover : "transparent"
            border {
                color: Theme.accent
                width:  1
            }

            ColorBehavior on color {}
            ColorBehavior on border.color {}
        }

        contentItem: Text {
            text:  root.buttonText
            color: Theme.accent
            font.pixelSize: 13
            font.weight: Font.Medium
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            ColorBehavior on color {}
        }

        onClicked: buttonClicked()
    }
}
