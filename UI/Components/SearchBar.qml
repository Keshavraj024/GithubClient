import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Controls"

Item {
    id: root

    property string latestQuery: ""
    property string latestUserName: ""

    signal searchRequested(string request)
    signal searchUserRequested(string request)

    Rectangle {
        id: searchContainer
        anchors.fill: parent

        radius: 24
        color: Qt.rgba(Theme.palette.surface.r, Theme.palette.surface.g, Theme.palette.surface.b, 0.8)
        border {
            color: searchInput.focus ? Theme.accent : Theme.palette.borderLight
            width: 2
        }

        ColorBehavior on color {}
        ColorBehavior on border.color {}

        RowLayout {
            anchors.fill: searchContainer
            spacing: 12
            anchors {
                leftMargin: 20
                rightMargin: 8
            }

            TextField {
                id: searchInput
                Layout.fillWidth: true
                placeholderText: "Search repositories or username"
                placeholderTextColor: Theme.palette.textSecondary
                color: Theme.palette.textPrimary
                font.pixelSize: 14
                selectByMouse: true

                background: Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                }

                ColorBehavior on color {}

                onAccepted: {
                    var searchText = searchInput.text
                    if(searchText.trim().length > 0) {
                        if(searchText.startsWith("@")) {
                            root.latestUserName = searchText.substring(1)
                            root.searchUserRequested(searchText.substring(1))
                        } else {
                            root.latestQuery = searchText.trim()
                            root.searchRequested(searchText.trim())
                        }
                    }
                }

                Keys.onEscapePressed: {
                    searchInput.text = ""
                    searchInput.focus = false;
                }

                onFocusChanged: {
                    if(focus) selectAll();
                }

                Rectangle {
                    id: typeIndicator
                    visible: searchInput.text !== ""
                    anchors {
                        right: searchInput.right
                        verticalCenter: searchInput.verticalCenter
                        rightMargin: 10
                    }
                    width: typeIndicatorText.contentWidth + 8
                    height: typeIndicatorText.contentHeight + 8
                    radius: height * 0.2
                    color: searchInput.text.startsWith("@") ?
                               Qt.rgba(0.2, 0.7, 1, 0.2) :
                               Qt.rgba(Theme.accent.r, Theme.accent.g, Theme.accent.b, 0.2)


                    Text {
                        id: typeIndicatorText
                        text : searchInput.text.startsWith("@") ? "USER" : "REPO"
                        anchors.centerIn: parent
                        font.pixelSize: 12
                        color:  searchInput.text.startsWith("@") ? "#2196F3" : Theme.accent
                    }
                    
                }



            }

            Button {
                id: searchBtn

                hoverEnabled: true

                Layout.preferredWidth: 30
                Layout.preferredHeight: 30

                icon.source: "qrc:/qt/qml/GithubClient/Assets/icons/search.png"
                icon.color: "black"
                font.pixelSize: 30

                onClicked: searchInput.accepted()

                background: Rectangle {
                    anchors.fill: searchBtn
                    radius: searchBtn.width / 2
                    color: searchBtn.pressed ?
                               Qt.darker(Theme.accent, 1.2) :
                               searchBtn.hovered ?
                                   Theme.accent :
                                   Qt.lighter(Theme.accent, 1.3)

                }
            }


        }

    }

}
