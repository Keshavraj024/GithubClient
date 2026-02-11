import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
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
                            // Hide the original image if you only want to see the version with the effect
                            visible: true
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


                TokenInput {
                    id:tokenInput
                    Layout.preferredWidth: 250
                    Layout.preferredHeight: 50

                    onTokenChanged: function(token) {
                        // console.log(token)
                    }
                }


                ThemeToggleSwitch {
                    id: themeToggleSwitch

                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 30
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

        StatusBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            repositoriesCount: root.repositories.length
            currentView:  root.currentView
            // lastUpdate: new Date()
            isLoading: true
        }
    }


}
