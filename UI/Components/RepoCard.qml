import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import GithubClient 1.0

Item {
    id: repoCard

    // height: 160

    required property var repositoryData
    readonly property QtObject theme: Theme.palette

    property string language : repositoryData?.language ?? ""

    function formatNum(num) {
        if(num >= 1000000) return +(num / 1000000).toFixed(1) + "M"
        if(num >= 1000) return +(num / 1000).toFixed(1) + "K"
        return num.toString()
    }

    function formatDate(date) {
        if(!date) return ""
        const now = new Date()
        const diffInsec = Math.floor((now - new Date(date)) / 1000)

        if(diffInsec < 0) return "Just now";
        if(diffInsec < 60) return diffInsec + "secs ago"

        const diffInMinutes = Math.floor(diffInsec / 60)
        if(diffInMinutes < 60) return diffInMinutes === 1 ? "1 minute ago" : diffInMinutes + " m ago"

        const diffInHours = Math.floor(diffInMinutes / 60)
        if(diffInHours < 24) return diffInHours === 1 ? "1 hour ago" : diffInHours + " h  ago"

        const diffInDays = Math.floor(diffInHours / 24)
        if (diffInDays === 1) return "Yesterday";
        if(diffInDays < 7) return diffInDays + " days  ago"

        const diffInWeeks = Math.floor(diffInDays / 7)
        if(diffInDays < 30) return diffInWeeks + " weeks ago"

        const diffInMonths = Math.floor(diffInDays / 30)
        if(diffInDays < 365) return diffInMonths + " months  ago"

        const diffInYears = Math.floor(diffInDays / 365)
        return diffInYears + "years ago";
    }

    MultiEffect {
        source: cardBackground
        anchors.fill: cardBackground
        shadowEnabled: true
        shadowBlur: 0.5
        shadowOpacity: mouseArea.containsMouse ? 0.8 : 0.6
        shadowColor: Theme.palette.shadowMedium
        shadowVerticalOffset: mouseArea.containsMouse ? 6 : 3
        shadowHorizontalOffset: mouseArea.containsMouse ? 6 : 3

        Behavior on shadowOpacity { NumberAnimation { duration: Theme.normalAnimation } }
        Behavior on shadowVerticalOffset { NumberAnimation { duration: Theme.normalAnimation } }
        Behavior on shadowHorizontalOffset { NumberAnimation { duration: Theme.normalAnimation } }
    }

    Rectangle {
        id: cardBackground
        anchors.margins: 10
        anchors.fill: repoCard
        color: theme.cardBackground
        border {
            color: theme.cardBorder
            width: 1
        }
        radius: 10

        Behavior on color { ColorAnimation { duration: Theme.normalAnimation } }
        Behavior on border.color { ColorAnimation { duration: Theme.normalAnimation } }

        Rectangle {
            id: hoverOverlay
            anchors.fill: cardBackground
            radius: cardBackground.radius
            color: theme.shadowMedium
            opacity: 0.0
            Behavior on opacity { NumberAnimation { duration: Theme.fastAnimation } }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: cardBackground
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onEntered: hoverOverlay.opacity = 0.5
            onExited: hoverOverlay.opacity = 0
            onPressed: hoverOverlay.opacity = 0.8
            onReleased: hoverOverlay.opacity = mouseArea.containsMouse ? 0.5 : 0
            onClicked: repoCard.clicked()
        }


        ColumnLayout {
            anchors.fill: cardBackground
            spacing: 12
            anchors.margins: 20

            RowLayout {
                Layout.fillWidth: true

                Text {
                    text: repositoryData?.fullName ?? ""
                    color: theme.textPrimary
                    font.bold: true
                    font.pixelSize: 16
                    elide: Text.ElideRight

                    Behavior on color {
                        ColorAnimation {
                            duration: Theme.normalAnimation
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Rectangle {
                    id: badgeRect
                    width: statusText.implicitWidth + 16
                    height: statusText.implicitHeight + 10
                    radius: 12
                    color: repositoryData?.isPrivate ? Theme.error : Theme.success

                    Text {
                        id: statusText
                        anchors.centerIn: badgeRect
                        text: repositoryData && repositoryData.isPrivate ? "Private" : "Public"
                        color: Theme.accent
                        font.pixelSize: 10
                        font.weight: Font.Medium
                    }

                    Behavior on color { ColorAnimation { duration: Theme.normalAnimation } }

                }
            }

            Text {
                text : repositoryData?.description ?? "No description available"
                font.pixelSize: 13
                Layout.fillWidth: true
                Layout.preferredHeight: 30
                color: theme.textSecondary
                wrapMode: Text.WordWrap
                elide: Text.ElideRight

                Behavior on color { ColorAnimation { duration: Theme.normalAnimation } }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                RowLayout {
                    spacing: 4
                    visible: repositoryData && repositoryData.language

                    Rectangle {
                        width: 12
                        height: width
                        radius: height / 2
                        color: Theme.getLanguageColor(language)

                        Behavior on color { ColorAnimation { duration: theme.normalAnimation }}
                    }

                    Text {
                        text: repositoryData.language
                        font.pixelSize: 12
                        color: theme.textSecondary
                        Behavior on color { ColorAnimation { duration: theme.normalAnimation }}
                    }
                }
                RowLayout {
                    spacing: 4
                    visible: repositoryData && repositoryData.stargazersCount

                    Text {
                      text: "★"
                      color: Theme.warning
                      font.pixelSize: 12
                      Behavior on color { ColorAnimation { duration: Theme.normalAnimation }}
                    }

                    Text {
                        // text: repoCard.formatNum(repositoryData.stargazersCount)
                        text: formatDate("2026-02-08T11:20:00Z")
                        font.pixelSize: 12
                        color: theme.textSecondary
                        Behavior on color { ColorAnimation { duration: Theme.normalAnimation }}
                    }
                }
                RowLayout {
                    spacing: 4
                    visible: repositoryData && repositoryData.forksCount

                    Text {
                      text: "⚡"
                      color: Theme.info
                      font.pixelSize: 12
                      Behavior on color { ColorAnimation { duration: Theme.normalAnimation }}
                    }

                    Text {
                        text: repoCard.formatNum(repositoryData.forksCount)
                        font.pixelSize: 12
                        color: theme.textSecondary
                        Behavior on color { ColorAnimation { duration: Theme.normalAnimation }}
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Text {
                    visible: repositoryData && repositoryData.updatedAt
                    text: repoCard.formatDate(repositoryData.updatedAt)
                    font.pixelSize: 12
                    color:theme.textTertiary
                    Behavior on color { ColorAnimation { duration: Theme.normalAnimation }}
                }
            }

        }
    }

}
