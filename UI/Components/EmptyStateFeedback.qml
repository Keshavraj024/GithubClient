import QtQuick

import "../Controls"

Item {
    id: root

    property string currentView

    Rectangle {
        anchors.centerIn: parent
        width: 300
        height: 200
        color: "transparent"

        Column {
            anchors.fill: parent
            spacing: 10

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "📁"
                font.pixelSize: 28
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: {
                    switch(root.currentView) {
                    case "search": return "No search results found"
                    case "user": return "No user repositories found"
                    default: return "No repositories loaded"
                    }
                }
                font.pixelSize: 20
                color: Theme.palette.textPrimary

                ColorBehavior on color {}
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Try searching or loading popular repositories"
                font.pixelSize: 18
                color: Theme.palette.textSecondary

                ColorBehavior on color {}
            }

        }

    }

}

