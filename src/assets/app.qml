import QtQuick 2.12
import QtQuick.Dialogs 1.3

import SceneGraphRendering 1.0

Item {
    Rectangle {
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent

            onWheel: {
                if (wheel.modifiers & Qt.ControlModifier) {

                }
            }
        }

        gradient: Gradient {
            GradientStop {
                position: 0;
                color: "steelblue"
            }

            GradientStop {
                position: 1;
                color: "black"
            }
        }

        View {
            id: renderer
            anchors.fill: parent
        }

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
            folder: shortcuts.home

            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                renderer.open_file(fileDialog.fileUrls)
            }

            onRejected: {
                console.log("Canceled")
            }
        }

        Text {
            color: "Silver"
            wrapMode: Text.WordWrap
            text: "Open file"
            font.pointSize: 20

            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 20

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    fileDialog.open()
                }
            }
        }
    }
}
