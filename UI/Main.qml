import QtQuick
import "Components"

Window {
    width: 1400
    height: 900
    visible: true
    title: qsTr("Github Client")

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
        }
    ]

    RepoCard{
      width: 500
      height: 200
      repositoryData: repositories[0]
    }




}
