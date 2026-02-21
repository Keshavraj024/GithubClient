import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: saveToCollectionDialog
    modal: true
    standardButtons: Dialog.Save | Dialog.Cancel
    closePolicy: Popup.NoAutoClose

    width: 500
    height: 250

    Component.onCompleted: console.log(availableWidth)

    padding: 10

    spacing: -1

    // We store the index of the repository we want to save
    property int targetRepoIndex: -1

    background: Rectangle {
        color: Theme.palette.background
        radius: 10
        border{
            width: 5
            color: Theme.palette.borderLight
        }
    }

    header: Rectangle {
        height: 50
        color: Theme.palette.background
        radius: 10
        border{
            width: 5
            color: Theme.palette.borderLight
        }
        Label {
            anchors.centerIn: parent
            text: "Save to Collection"
            color: Theme.palette.textPrimary
            font.bold: true
            font.pixelSize: 20
        }
    }

    contentItem: ColumnLayout {
        spacing: 10
        anchors.margins: 10

        Label {
            text: "Select a Collection:"
            color: Theme.palette.textPrimary
            font.bold: true
            font.pixelSize: 18
        }

        ComboBox {
            id: collectionSelector
            Layout.fillWidth: true
            model: repositoryController.collectionModel
            textRole: "name"  // Matches the role in your CollectionModel
        }

        Label {
            text: "Or Create New:"
            color: Theme.palette.textPrimary
            font.bold: true
            font.pixelSize: 18
        }

        TextField {
            id: newCollectionInput
            Layout.fillWidth: true
            placeholderText: "Enter new collection name"
        }
    }

    onAccepted: {
        let finalCollectionId = -1;
        let newName = newCollectionInput.text.trim();

        if (newName !== "") {
            // Logic 1: Create new collection first
            // Your controller should return the ID of the newly created collection
            finalCollectionId = repositoryController.createCollection(newName);
        } else if (collectionSelector.currentIndex !== -1) {
            // Logic 2: Use existing selected ID
            // Assuming your model has an 'id' role
            finalCollectionId = collectionSelector.model.data(
                        collectionSelector.model.index(collectionSelector.currentIndex, 0),
                        Qt.UserRole + 1 // Or whatever your ID role is
                        );
        }

        if (finalCollectionId !== -1) {
            // repositoryController.toggleSave(targetRepoIndex, finalCollectionId);
            newCollectionInput.clear();
        }
    }

    onRejected: {
        newCollectionInput.clear();
    }
}
