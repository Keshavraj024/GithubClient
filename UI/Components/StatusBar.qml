import QtQuick

import QtQuick.Layouts

import GithubClient 1.0

Item {
    id: root

    property int repositoriesCount
    property string currentView:  "Repositories"
    property date lastUpdate: new Date()
    property bool isLoading: false

    Rectangle {
        id: statusRect
        anchors.fill: parent
        color: Theme.palette.surface
        border {
            color: Theme.palette.borderLight
            width: 1
        }

        ColorBehavior on color {}

        RowLayout {
            anchors.fill: statusRect
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 20

            RowLayout {
                spacing: 15

                Rectangle {
                    id: loaderRect
                    Layout.preferredWidth: 8
                    Layout.preferredHeight: 8
                    radius: width / 2
                    color: {
                        if (root.isLoading) return Theme.warning
                        if (root.repositoriesCount > 0) Theme.success
                        return Theme.palette.textTertiary
                    }
                    ColorBehavior on color {}

                    SequentialAnimation {
                        running: root.isLoading
                        loops: Animation.Infinite
                        NumberAnimation {
                            target: loaderRect
                            property: "scale"
                            from: 1.0
                            to: 1.5
                            duration: Theme.slowAnimation
                        }
                        NumberAnimation {
                            target: loaderRect
                            property: "scale"
                            from: 1.5
                            to: 1.0
                            duration: Theme.slowAnimation
                        }
                    }
                }

                Text {
                    text: {
                        if(root.isLoading) return "Loading Repositories"
                        if(root.repositoriesCount === 0) return "No repositories Found"
                        return `Found ${root.repositoriesCount} ${root.repositoriesCount === 1 ?
                                    'repository' : 'repositories'}`
                    }

                    font.pixelSize: 12
                    color: Theme.palette.textSecondary

                    ColorBehavior on color {}
                }

            }


            Rectangle {
                Layout.preferredWidth: 1
                Layout.preferredHeight: statusRect.height * 0.9
                color: Theme.palette.borderLight

                ColorBehavior on color {}
            }

            Rectangle {
                id: currentViewRect

                Layout.preferredWidth: currentViewText.implicitWidth + 20
                Layout.preferredHeight: currentViewText.implicitHeight + 10
                radius: 10

                color: Qt.alpha(Theme.accent, 0.1)
                border.color: Qt.alpha(Theme.accent, 0.2)
                border.width: 1

                ColorBehavior on color {}
                ColorBehavior on border.color {}

                Text {
                    id: currentViewText
                    text: switch(root.currentView) {
                          case "search" : return "🔍 Search Repositories"
                          case "user": return "🧑 User Repositories"
                          case "myrepos": return "📦 My Repositories"
                          default: return "🔥 Popular Repositories"
                          }
                    anchors.centerIn: parent
                    font.pixelSize: 12
                    font.weight: Font.Medium
                    color: Theme.palette.textPrimary

                    ColorBehavior on color {}
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                Layout.preferredWidth: 250
                Layout.preferredHeight: 20

                visible: root.lastUpdate
                text: `Updated At : ${Qt.formatDateTime(root.lastUpdate, "dddd, d MMM yyyy - hh:mm")}`

                font.pixelSize: 12
                color: Theme.palette.textSecondary

                ColorBehavior on color {}
            }

            Rectangle {
                visible: root.lastUpdate
                Layout.preferredWidth: 1
                Layout.preferredHeight: statusRect.height * 0.9
                color: Theme.palette.borderLight

                ColorBehavior on color {}
            }

            RowLayout {
                spacing: 5
                Rectangle {
                    Layout.preferredWidth: githubServiceText.implicitWidth + 15
                    Layout.preferredHeight: githubServiceText.implicitHeight + 10
                    radius: 15
                    color: Qt.lighter(Theme.success, 1.3)
                    border {
                        color: Theme.accent
                        width: 1
                    }

                    SequentialAnimation on opacity{
                        running: true
                        loops: Animation.Infinite
                        NumberAnimation {
                            to: 0.7 ; duration: 1200
                        }
                        NumberAnimation {
                            to: 1.0 ; duration: 1200
                        }
                    }

                    Text {
                        id: githubServiceText
                        anchors.centerIn: parent
                        text: "Github API Connected"
                        font.pixelSize: 12
                        font.weight: Font.Bold
                        color: Theme.palette.textTertiary

                        ColorBehavior on color {}
                    }

                }
            }

        }


    }

}
