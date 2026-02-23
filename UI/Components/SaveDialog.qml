import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Customs"

Popup {
    id: saveToCollectionPopup
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // Centering the popup in the parent window
    anchors.centerIn: Overlay.overlay

    width: 400
    height: 325
    padding: 20

    // Properties inherited from your previous logic
    property int targetRepoIndex: -1

    background: Rectangle {
        color: Theme.palette.background
        radius: 12
        border {
            width: 2
            color: Theme.palette.borderLight
        }
        // Shadow effect (Optional, adds depth)
        layer.enabled: true
    }

    contentItem: ColumnLayout {
        spacing: 10

        Label {
            text: "Save to Collection"
            font.pixelSize: 22
            font.bold: true
            color: Theme.palette.textPrimary
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 10
        }

        // --- Selection Logic ---
        ColumnLayout {
            spacing: 5
            Layout.fillWidth: true

            Label {
                text: "Select a Collection:"
                color: Theme.palette.textSecondary
                font.pixelSize: 18
            }

            ComboBox {
                id: collectionSelector
                Layout.fillWidth: true
                Layout.preferredHeight: 28
                model: repositoryController.collectionModel
                textRole: "name"

                background: Rectangle {
                    color: Theme.palette.background
                    radius: 2
                    border {
                        width: 2
                        color: Theme.palette.borderLight
                    }
                }
            }
        }

        Label {
            text: "— OR —"
            Layout.alignment: Qt.AlignHCenter
            color: Theme.palette.textSecondary
            font.pixelSize: 18
        }

        ColumnLayout {
            spacing: 5
            Layout.fillWidth: true

            Label {
                text: "Create New Collection:"
                color: Theme.palette.textSecondary
                font.pixelSize: 18
            }

            TextField {
                id: newCollectionInput
                Layout.fillWidth: true
                Layout.preferredHeight: 28
                placeholderText: "Enter name..."
                selectByMouse: true
                color: Theme.palette.textPrimary

                background: Rectangle {
                    color: Theme.palette.background
                    radius: 2
                    border {
                        width: 2
                        color: Theme.palette.borderLight
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            CustomButton {
                buttonText: "Save"
                Layout.preferredWidth: 65
                Layout.preferredHeight: 30
                enabled: newCollectionInput.text.trim() !== "" || collectionSelector.currentIndex !== -1
                onButtonClicked: handleSave()
            }

            CustomButton {
                buttonText: "Cancel"
                Layout.preferredWidth: 65
                Layout.preferredHeight: 30
                onButtonClicked: {
                    newCollectionInput.clear();
                    saveToCollectionPopup.close();
                }
            }
        }
    }

    function handleSave() {
        let finalCollectionId = -1;
        let newName = newCollectionInput.text.trim();

        if (newName !== "") {
            finalCollectionId = repositoryController.createCollection(newName);
        } else if (collectionSelector.currentIndex !== -1) {
            // Updated access logic for cleaner reading
            finalCollectionId = collectionSelector.model.data(
                        collectionSelector.model.index(collectionSelector.currentIndex, 0),
                        Qt.UserRole + 1
                        );
        }

        if (finalCollectionId !== -1) {
            repositoryController.toggleSave(targetRepoIndex, finalCollectionId);
            newCollectionInput.clear();
            saveToCollectionPopup.close();
        }
    }
}
