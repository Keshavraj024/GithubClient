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
    property date lastUpdated : new Date()

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

            model: repositoryController.model
            cellWidth: Math.floor(repositoryGrid.width / Math.max(1, Math.floor(repositoryGrid.width / 380)))
            cellHeight: 180

            delegate: RepoCard {
                required property var modelData

                width: repositoryGrid.cellWidth
                height: repositoryGrid.cellHeight
                repositoryData: modelData

                // visible: !modelData.isSaved
                onClicked: {
                    if (repositoryData.htmlUrl) {
                        Qt.openUrlExternally(repositoryData.htmlUrl)
                    }
                }

                onSaveButtonClicked: function(index) {
                    saveDialog.targetRepoIndex = index
                    if(!modelData.isSaved)
                        saveDialog.open()
                    else
                        repositoryController.toggleSave(index)
                }
            }
        }
    }

    Component {
        id: loadingComponent
        LoadingOverlay {
            isLoading: repositoryController.isLoading
        }
    }

    Component {
        id: popupComponent
        ErrorPopup {
            message: repositoryController.errorMessage
        }
    }

    RepositoryController {
        id: repositoryController
        onModelCountChanged: {
            root.lastUpdated = new Date()
        }
    }

    SaveDialog {
        anchors.centerIn: parent
        id: saveDialog
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
                repositoryController.fetchRemoteRepositories(repositoryName)
                root.currentView = "search"
            }
            onSearchUserRequested: function(userName) {
                root.latestUserName = userName
                repositoryController.fetchUserRepositories(userName)
                root.currentView = "user"
            }

            onTokenChanged: function(token) {
                repositoryController.authToken = token
            }


        }

        ToolBar {
            id: toolbarRect
            Layout.fillWidth: true
            Layout.preferredHeight: 60

            currentView: root.currentView

            onPopularButtonClicked: {
                root.currentView = "repositories"
                repositoryController.fetchRemoteRepositories("stars:>10000", "stars", "desc")
            }

            onRefreshButtonClicked : {

                switch (root.currentView) {
                case "user":
                    if (root.latestUserName.length > 0)
                        repositoryController.fetchUserRepositories(root.latestUserName)
                    break
                case "search":
                    if (root.latestQuery.length > 0)
                        repositoryController.fetchRemoteRepositories(root.latestQuery)
                    break
                case "myrepos":
                    repositoryController.fetchAuthenticatedUserRepositories()
                    break
                default:
                    repositoryController.fetchRemoteRepositories("stars:>10000", "stars", "desc")

                }
            }

            onMyrepoButtonClicked : {
                repositoryController.fetchAuthenticatedUserRepositories()
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
                id: myLoader
                anchors.fill: contentArea
                sourceComponent: {
                    if (repositoryController.isLoading)
                        return loadingComponent
                    else if(repositoryController.errorMessage.length > 0 && !repositoryController.isLoading)
                        return popupComponent
                    else if (repositoryController.modelCount === 0)
                        return emptyStateComponent
                    else
                        return gridViewComponent
                }

            }
        }

        StatusBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            repositoriesCount: repositoryController.modelCount
            currentView:  root.currentView
            lastUpdate: root.lastUpdated
            isLoading: repositoryController.isLoading
        }
    }

}
