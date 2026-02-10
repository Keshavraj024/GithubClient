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
            color: searchInput.focus ? Theme.accent : searchContainer.color
            width: 2
        }

        RowLayout {
            anchors.fill: searchContainer
            spacing: 12
            anchors {
                leftMargin: 20
                rightMargin: 8
            }

            // TODO: REPLACE WITH IMAGE
            Text {
                text: "ὐ"
                font.pixelSize: 16
                color: Theme.palette.textSecondary

                ColorBehavior on color {}
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
            }
        }

    }

}
