import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import GithubClient 1.0

Item {
    id: repoCard

    height: 160



    required property var repositoryData
    readonly property QtObject theme: Theme.palette

    MultiEffect {
        source: cardBackground
        anchors.fill: cardBackground
        shadowEnabled: true
        shadowBlur: 0.5
        shadowOpacity: mouseArea.containsMouse ? 0.8 : 0.6
        shadowColor: Theme.palette.shadowMedium
        shadowVerticalOffset: mouseArea.containsMouse ? 6 : 3
        shadowHorizontalOffset: mouseArea.containsMouse ? 6 : 3

        Behavior on shadowOpacity { NumberAnimation { duration: Theme.normalAnimation } }
        Behavior on shadowVerticalOffset { NumberAnimation { duration: Theme.normalAnimation } }
        Behavior on shadowHorizontalOffset { NumberAnimation { duration: Theme.normalAnimation } }
    }

    Rectangle {
        id: cardBackground
        anchors.margins: 10
        anchors.fill: repoCard
        color: theme.cardBackground
        border {
            color: theme.cardBorder
            width: 1
        }
        radius: 10

        Behavior on color { ColorAnimation { duration: Theme.normalAnimation } }
        Behavior on border.color { ColorAnimation { duration: Theme.normalAnimation } }

        Rectangle {
            id: hoverOverlay
            anchors.fill: cardBackground
            radius: cardBackground.radius
            color: theme.shadowMedium
            opacity: 0.0
            Behavior on opacity { NumberAnimation { duration: Theme.fastAnimation } }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: cardBackground
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onEntered: hoverOverlay.opacity = 0.5
            onExited: hoverOverlay.opacity = 0
            onPressed: hoverOverlay.opacity = 0.8
            onReleased: hoverOverlay.opacity = mouseArea.containsMouse ? 0.5 : 0
            onClicked: repoCard.clicked()
        }


        // ColumnLayout {
        //     anchors.fill: cardBackground
        //     spacing: 12
        //     anchors.margins: 20

        //     Rectangle {
        //         color: "red"
        //         Layout.fillWidth: true
        //         Layout.fillHeight: true
        //     }
        // }
    }

}
