import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "Components"
import "Controls"

Window {
    id: root
    width: 1400
    height: 900
    visible: true
    title: qsTr("Github Client")

    property string currentView: "repositories" // "repositories", "search", "user"

    property var repositories: [
        {
            fullName: "microsoft/vscode",
            description: "Visual Studio Code - Open source code editor",
            language: "TypeScript",
            stargazersCount: 142000,
            forksCount: 25000,
            isPrivate: false,
            updatedAt: new Date(Date.now() - 2 * 24 * 60 * 60 * 1000),
            htmlUrl: "https://github.com/microsoft/vscode"
        },
        {
            fullName: "torvalds/linux",
            description: "Linux kernel source tree",
            language: "C",
            stargazersCount: 165000,
            forksCount: 48000,
            isPrivate: false,
            updatedAt: new Date(Date.now() - 1 * 24 * 60 * 60 * 1000),
            htmlUrl: "https://github.com/torvalds/linux"
        }
    ]

    Rectangle {
        id: backgroundRect
        anchors.fill: parent
        color: Theme.palette.background

        ColorBehavior on color {}

        MouseArea {
            anchors.fill: backgroundRect
            onClicked: {
                // This takes focus away from the TextField
                backgroundRect.forceActiveFocus()
            }
        }
    }

    ColumnLayout {
        anchors.fill: backgroundRect
        spacing: 0

        Rectangle {
            id: headerRect

            Layout.fillWidth: true
            Layout.preferredHeight: 100

            color: Theme.palette.surface
            border.color: Theme.palette.borderLight
            border.width: 1

            Behavior on color {
                ColorAnimation {
                    duration: Theme.normalAnimation
                }
            }
            Behavior on border.color {
                ColorAnimation {
                    duration: Theme.normalAnimation
                }
            }

            RowLayout {
                anchors.fill: headerRect
                anchors.leftMargin: 32
                anchors.rightMargin: 32
                spacing: 3

                RowLayout {
                    spacing: 20
                    Image {
                        Layout.preferredWidth: 45
                        Layout.preferredHeight: 45
                        fillMode: Image.PreserveAspectFit
                        source: "../Assets/images/App-Logo.png"
                    }

                    Column {
                        spacing: 5
                        Text {
                            text: "GitBrowse"
                            color: Theme.palette.textPrimary
                            font.pixelSize: 28
                            font.weight: Font.Bold
                            Behavior on color {
                                ColorAnimation {
                                    duration: Theme.normalAnimation
                                }
                            }
                        }

                        Text {
                            text: "A better way to explore GitHub"
                            color: Theme.palette.textSecondary
                            font.pixelSize: 20
                            font.weight: Font.Medium
                            font.letterSpacing: 0.4
                            Behavior on color {
                                ColorAnimation {
                                    duration: Theme.normalAnimation
                                }
                            }
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
                            root.currentView = "search"
                            console.log(repositoryName)
                        }
                        onSearchUserRequested: function(userName) {
                            root.currentView = "user"
                            console.log(userName)
                        }
                    }
                }

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

                        Rectangle {
                            width: 30
                            height: 30
                            x: toggleSwitch.checked ? indicatorRect.width - width : 0
                            radius: 15
                            color: Theme.isDark
                                   ? Theme.accent
                                   : Theme.sunMode

                            Behavior on x { NumberAnimation { duration: Theme.normalAnimation } }

                            Image {
                                width: 15
                                height: 15
                                anchors.centerIn: parent
                                source : toggleSwitch.checked ? "../Assets/images/moon.png" : "../Assets/images/sun.png"
                            }
                        }
                    }
                }



            }


        }

        ScrollView {
            id: repoScrollView
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            contentWidth: availableWidth

            background: Rectangle {
                anchors.fill: repoScrollView
                color: Theme.palette.background
                ColorBehavior on color {}
            }

            GridView {
                id: repositoryGrid
                anchors.fill: parent
                anchors.margins: 20
                model: root.repositories
                cellWidth: Math.floor(repositoryGrid.width / Math.max(1, Math.floor(repositoryGrid.width / 380)))
                cellHeight: 180

                delegate: RepoCard {
                    required property var modelData
                    width: repositoryGrid.cellWidth
                    height: repositoryGrid.cellHeight
                    repositoryData: modelData
                    onClicked: {
                        console.log("Repository clicked:", repositoryData.fullName)
                        if (repositoryData.htmlUrl) {
                            Qt.openUrlExternally(repositoryData.htmlUrl)
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            id: statusRect
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: Theme.palette.surface
            border {
                color: Theme.palette.borderLight
                width: 1
            }

            Behavior on color {
                ColorAnimation {
                    duration: Theme.normalAnimation
                }
            }

            RowLayout {
                anchors.fill: statusRect
                anchors.margins: 10

                Text {
                    text: repositories.length + " Repositories"
                    font.pixelSize: 12
                    color: Theme.palette.textSecondary


                    Behavior on color {
                        ColorAnimation {
                            duration: Theme.normalAnimation
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Text {
                    text: "Ready"
                    font.pixelSize: 12
                    color: Theme.palette.textSecondary

                    ColorBehavior on color {}

                }
            }
        }

    }


}
