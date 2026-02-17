import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

import "../Controls"

Item {
    id: root

    signal searchRequested(string repositoryName)
    signal searchUserRequested(string userName)
    signal  tokenChanged(string token)

    Rectangle {
        anchors.fill: parent

        color: Theme.palette.surface
        border.color: Theme.palette.borderLight
        border.width: 1

        ColorBehavior on color {}
        ColorBehavior on border.color {}

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 32
            anchors.rightMargin: 32
            spacing: 10

            RowLayout {
                spacing: 20
                Item {
                    Layout.preferredWidth: 45
                    Layout.preferredHeight: 45

                    Image {
                        id: appLogo
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/qt/qml/GithubClient/Assets/images/App-Logo.png"
                    }

                    MultiEffect {
                        source: appLogo
                        anchors.fill: appLogo
                        shadowEnabled: true
                        shadowBlur: 1.0
                        shadowScale: 1.2
                        shadowColor: "#7000ff"
                        shadowOpacity: 0.6
                    }

                    MultiEffect {
                        source: appLogo
                        anchors.fill: appLogo
                        shadowEnabled: true
                        shadowBlur: 0.5
                        shadowColor: "#00d2ff"
                        shadowOpacity: 1.0
                    }
                }

                Column {
                    spacing: 5
                    Text {
                        text: "GitBrowse"
                        color: Theme.palette.textPrimary
                        font.pixelSize: 28
                        font.weight: Font.Bold
                        ColorBehavior on color {}
                    }

                    Text {
                        text: "A better way to explore GitHub"
                        color: Theme.palette.textSecondary
                        font.pixelSize: 20
                        font.weight: Font.Medium
                        font.letterSpacing: 0.4
                        ColorBehavior on color {}
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            RowLayout {
                id: searchBarLayout

                Layout.preferredWidth: 300
                spacing: 20

                SearchBar {
                    Layout.preferredWidth: 250
                    Layout.preferredHeight: 50

                    onSearchRequested: function(repositoryName) {
                        root.searchRequested(repositoryName)
                    }
                    onSearchUserRequested: function(userName) {
                        root.searchUserRequested(userName)
                    }
                }
            }

            TokenInput {
                id:tokenInput
                Layout.preferredWidth: 250
                Layout.preferredHeight: 50

                onTokenChanged: function(token) {
                    root.tokenChanged(token)
                }
            }


            ThemeToggleSwitch {
                id: themeToggleSwitch

                Layout.preferredWidth: 60
                Layout.preferredHeight: 30
            }


        }


    }

}
