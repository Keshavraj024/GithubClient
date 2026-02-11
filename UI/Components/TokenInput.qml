import QtQuick
import QtQuick.Controls

import GithubClient 1.0

Item {
    id: root

    signal tokenChanged(string token)

    property alias tokenText: tokenField.text
    property alias placeHolderText: tokenField.placeholderText

    Rectangle {
        id: tokenInputRect

        anchors.fill: parent
        radius: 25
        color: Theme.palette.surface
        border {
            color: tokenField.focus ? Theme.accent : Theme.palette.borderLight
            width: tokenField.focus ? 2 : 1
        }

        ColorBehavior on color {}
        ColorBehavior on border.color {}

        TextField {
            id: tokenField

            anchors.fill: tokenInputRect
            anchors.margins: 10

            selectByMouse: true

            font.pixelSize: 14
            color: Theme.palette.textPrimary
            echoMode: TextInput.Password

            placeholderText: "Enter the Github Token...."
            placeholderTextColor: Theme.palette.textSecondary

            background: Rectangle {
                anchors.fill: parent
                color: "transparent"
            }

            ColorBehavior on color {}
            ColorBehavior on placeholderTextColor {}

            onTextChanged: function() {
                root.tokenChanged(tokenField.text)
            }
        }
    }
}
