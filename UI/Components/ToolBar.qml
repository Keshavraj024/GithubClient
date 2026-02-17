import QtQuick
import QtQuick.Layouts

import "../Controls"
import "../Customs"

Item {
    id: root

    property string currentView

    signal popularButtonClicked
    signal refreshButtonClicked
    signal myrepoButtonClicked

    Rectangle {
        anchors.fill: parent

        color: Theme.palette.surface
        border.color: Theme.palette.borderLight
        border.width: 1

        ColorBehavior on color {}
        ColorBehavior on border.color {}

        RowLayout {
            anchors.fill: parent
            anchors.margins: 15

            spacing: 10

            CustomButton {
                id: popularRepoBtn

                Layout.preferredWidth: 100
                Layout.preferredHeight: 30

                buttonText: "🔥 Popular"

                onButtonClicked: root.popularButtonClicked()

            }

            CustomButton {
                id: refereshBtn

                Layout.preferredWidth: 100
                Layout.preferredHeight: 30

                buttonText: "⟳ Refresh"

                onButtonClicked: root.refreshButtonClicked()
            }

            CustomButton {
                id: myRepoBtn
                visible: githubService.authToken.length > 0

                Layout.preferredWidth: 100
                Layout.preferredHeight: 30

                buttonText: "🧑 My Repo"

                onButtonClicked: {
                    root.myrepoButtonClicked()
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

}
