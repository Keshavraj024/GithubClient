import QtQuick


import "../Controls"

Item {
    id: root

    property bool isLoading

    Rectangle {
        anchors.fill: parent
        color: Qt.alpha(Theme.palette.background, 0.8)
        ColorBehavior on color {}

        Column {
            spacing: 10
            anchors.centerIn: parent

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 40
                height: width
                radius: height / 2
                color: Theme.accent

                ColorBehavior on color {}

                RotationAnimation on rotation {
                    from: 0
                    to: 360
                    running: root.isLoading
                    loops: Animation.Infinite
                    duration: 1000
                }

                Text {
                    anchors.centerIn: parent
                    text: "⚡"
                    font.pixelSize: 20
                }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Loading Repositories....."
                font.pixelSize: 30
                font.weight: Font.Medium
                color: Theme.palette.textPrimary
                ColorBehavior on color {}
            }
        }
    }
}
