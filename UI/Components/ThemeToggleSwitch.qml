import QtQuick
import QtQuick.Controls

import GithubClient 1.0

Switch {
    id: toggleSwitch
    onCheckedChanged: Theme.toggleTheme()

    indicator: Rectangle {
        id: indicatorRect

        width: 50
        height: 30
        radius: 15
        color: Theme.isDark
               ? Theme.palette.surface
               : Theme.palette.borderLight

        border.color: Theme.palette.border

        ColorBehavior on color {}
        ColorBehavior on border.color {}

        Rectangle {
            width: 30
            height: 30
            x: toggleSwitch.checked ? indicatorRect.width - width : 0
            radius: 15
            color: Theme.isDark
                   ? Theme.accent
                   : Theme.sunMode

            ColorBehavior on color {}

            Behavior on x {NumberAnimation {duration: Theme.normalAnimation}}

            Image {
                width: 15
                height: 15
                anchors.centerIn: parent
                source : toggleSwitch.checked ? "qrc:/qt/qml/GithubClient/Assets/images/moon.png" : "qrc:/qt/qml/GithubClient/Assets/images/sun.png"
            }
        }
    }
}

