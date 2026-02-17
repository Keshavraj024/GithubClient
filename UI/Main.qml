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

    Component {
        id: loadingComponent
        LoadingOverlay {
            isLoading: githubService.isLoading
        }
    }

    Component {
        id: popupComponent
        ErrorPopup {
            message: githubService.errorMessage
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

        Header {
            id: headerRect

            Layout.fillWidth: true
            Layout.preferredHeight: 100

            onSearchRequested: function(repositoryName) {
                root.latestQuery = repositoryName
                githubService.searchRepositories(repositoryName)
                root.currentView = "search"
            }
            onSearchUserRequested: function(userName) {
                root.latestUserName = userName
                githubService.fetchUserRepositories(userName)
                root.currentView = "user"
            }

            onTokenChanged: function(token) {
                githubService.authToken = token
            }


        }

        ToolBar {
            id: toolbarRect
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            currentView: root.currentView

            onPopularButtonClicked: {
                root.currentView = "repositories"
                githubService.searchRepositories("stars:>10000", "stars", "desc")
            }

            onRefreshButtonClicked : {

                switch (root.currentView) {
                case "user":
                    if (root.latestUserName.length > 0)
                        githubService.fetchUserRepositories(root.latestUserName)
                    break
                case "search":
                    if (root.latestQuery.length > 0)
                        githubService.searchRepositories(root.latestQuery)
                    break
                case "myrepos":
                    githubService.fetchAuthenticatedUserRepositories()
                    break
                default:
                    githubService.searchRepositories("stars:>10000", "stars", "desc")

                }
            }

            onMyrepoButtonClicked : {
                githubService.fetchAuthenticatedUserRepositories()
                root.currentView = "myrepos"
                root.latestQuery = ""
                root.latestUserName = ""
            }

        }

        Item {
            id: contentArea
            Layout.fillWidth: true
            Layout.fillHeight: true

            Loader {
                anchors.fill: contentArea
                sourceComponent: {
                    if (githubService.isLoading)
                        return loadingComponent
                    else if(githubService.errorMessage.length > 0 && !githubService.isLoading)
                        return popupComponent
                    else if ((githubService.repositories.length) === 0)
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
