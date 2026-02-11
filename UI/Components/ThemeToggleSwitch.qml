import QtQuick
import QtQuick.Controls
import QtQuick.Effects

import GithubClient 1.0

Item {
    id: root

    Switch {
        id: toggleSwitch

        onCheckedChanged: function() {
            Theme.toggleTheme()
            indicatorRect.scale = 0.9
            scaleBackAnimation.start()
        }

        NumberAnimation {
            id: scaleBackAnimation
            target: indicatorRect
            property: "scale"
            from: 0.9
            to: 1.0
            duration: Theme.fastAnimation
            easing.type: Easing.OutBack
        }


        indicator: Rectangle {
            id: indicatorRect

            implicitWidth: 60
            implicitHeight: 30

            radius: 15
            color: Theme.isDark
                   ? Theme.palette.surface
                   : Theme.palette.borderLight

            border.color: Theme.palette.border

            ColorBehavior on color {}
            ColorBehavior on border.color {}

            Rectangle {
                id: thumb
                width: 22
                height: width
                anchors.verticalCenter: indicatorRect.verticalCenter

                x: toggleSwitch.checked ? indicatorRect.width - width - 3 : 3
                radius: height / 2
                color: Theme.isDark
                       ? Theme.accent
                       : Theme.sunMode

                ColorBehavior on color {}

                Behavior on x {NumberAnimation {duration: Theme.normalAnimation; easing.type: Easing.OutCubic}}

                Image {
                    width: 15
                    height: 15
                    anchors.centerIn: parent
                    source : toggleSwitch.checked ? "qrc:/qt/qml/GithubClient/Assets/images/moon.png" : "qrc:/qt/qml/GithubClient/Assets/images/sun.png"
                }
            }

            MultiEffect {
                source: thumb
                anchors.fill: thumb

                shadowEnabled: true
                shadowColor: thumb.color
                shadowBlur: 1.0
                shadowScale: 1.1
                shadowHorizontalOffset: 0
                shadowVerticalOffset: 0

                shadowOpacity: 1.0

                Behavior on shadowOpacity { NumberAnimation { duration: Theme.normalAnimation } }
            }

            // Mouse area for the cursor shape
            MouseArea{
                anchors.fill: indicatorRect
                acceptedButtons: Qt.NoButton
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

}


