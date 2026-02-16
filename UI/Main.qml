import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import GithubClient 1.0
import "Components"
import "Controls"
import "Customs"

Window {
    id: root
    width: 1400
    height: 900
    visible: true
    title: qsTr("Github Client")

    property string currentView: "repositories" // "repositories", "search", "user", "myrepos"
    property string latestQuery: ""
    property string latestUserName: ""
    property date lastUpdated

    Component {
        id: emptyStateComponent
        EmptyStateFeedback {
            currentView: root.currentView
        }
    }

    Component {
        id: gridViewComponent
        GridView {
            id: repositoryGrid

            Layout.margins: 10
            clip: true

            model: githubService.repositories
            cellWidth: Math.floor(repositoryGrid.width / Math.max(1, Math.floor(repositoryGrid.width / 380)))
            cellHeight: 180

            delegate: RepoCard {
                required property var modelData
                width: repositoryGrid.cellWidth
                height: repositoryGrid.cellHeight
                repositoryData: modelData
                onClicked: {
                    if (repositoryData.htmlUrl) {
                        Qt.openUrlExternally(repositoryData.htmlUrl)
                    }
                }
            }
        }
    }

    GitHubService {
        id: githubService
        onRepositoriesChanged: {
            lastUpdated = new Date()
        }
    }

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
                            root.latestQuery = repositoryName
                            githubService.searchRepositories(repositoryName)
                            root.currentView = "search"
                            // console.log(repositoryName)
                        }
                        onSearchUserRequested: function(userName) {
                            root.latestUserName = userName
                            githubService.fetchUserRepositories(userName)
                            root.currentView = "user"
                            // console.log(userName)
                        }
                    }
                }

                TokenInput {
                    id:tokenInput
                    Layout.preferredWidth: 250
                    Layout.preferredHeight: 50

                    onTokenChanged: function(token) {
                        githubService.authToken = token
                    }
                }


                ThemeToggleSwitch {
                    id: themeToggleSwitch

                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 30
                }


            }
        }

        Rectangle {
            id: toolbarRect
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            color: Theme.palette.surface
            border.color: Theme.palette.borderLight
            border.width: 1

            ColorBehavior on color {}
            ColorBehavior on border.color {}

            RowLayout {
                anchors.fill: toolbarRect
                anchors.margins: 15

                spacing: 10

                CustomButton {
                    id: popularRepoBtn

                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 30

                    buttonText: "🔥 Popular"

                    onButtonClicked: {
                        currentView = "repositories"
                        githubService.searchRepositories("stars:>10000", "stars", "desc")
                    }
                }

                CustomButton {
                    id: refereshBtn

                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 30

                    buttonText: "⟳ Refresh"

                    onButtonClicked: {
                        switch (root.currentView) {
                        case "user" : (root.latestUserName.length > 0) ?
                                          githubService.fetchUserRepositories(root.latestUserName) :
                                          qDebug() << "Username is Empty while refreshing"
                            break
                        case "search": (root.latestQuery.length > 0) ?
                                           githubService.searchRepositories(root.latestQuery) :
                                           qDebug() << "Query is Empty while refreshing"
                            break
                        case "myrepos": githubService.fetchAuthenticatedUserRepositories()
                            break
                        default:
                            githubService.searchRepositories("stars:>10000", "stars", "desc")
                        }
                    }
                }

                CustomButton {
                    id: myRepoBtn
                    visible: githubService.authToken.length > 0

                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 30

                    buttonText: "🧑 My Repo"

                    onButtonClicked: {
                        githubService.fetchAuthenticatedUserRepositories()
                        root.currentView = "myrepos"
                        root.latestQuery = ""
                        root.latestUserName = ""

                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Text {
                    text: {
                        switch(root.currentView) {
                        case "search": return "🔍 Search Results"
                        case "user": return "🧑 User Repositories"
                        case "myrepos": return "📦 My Repositories"
                        default: return "🔥 Popular Repositories"
                        }
                    }
                    color: Theme.accent
                    font.pixelSize: 13
                    ColorBehavior on color {}
                }
            }
        }

        Item {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true

            Loader {
                anchors.fill: contentArea
                sourceComponent: {
                    // if (githubService.isLoading)
                    //     return loadingComponent
                    if ((githubService.repositories.length) === 0)
                        return emptyStateComponent
                    else
                        return gridViewComponent
                }
            }
        }

        StatusBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            repositoriesCount: githubService.repositories.length
            currentView:  root.currentView
            lastUpdate: root.lastUpdated
            isLoading: githubService.isLoading
        }
    }

}
