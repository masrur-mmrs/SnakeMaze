// ────────────────────────────────────────────────────────────
//  GameOverScreen.qml
//  Results screen with winner announcement and replay option.
// ────────────────────────────────────────────────────────────
import QtQuick
import QtQuick.Layouts
import SnakeMaze 1.0

Item {
    id: root

    property bool playerWon:    true
    property int  playerScore:  0
    property int  cpuScore:     0

    signal playAgainClicked
    signal menuClicked

    // Animated background flash on entry
    Rectangle {
        anchors.fill: parent
        color: root.playerWon ? "#44E5A0" : "#FF5E78"
        opacity: 0

        NumberAnimation on opacity {
            id: flashAnim
            from: 0.18; to: 0; duration: 800
            easing.type: Easing.OutCubic
            running: false
        }

        Connections {
            target: root
            function onPlayerWonChanged() { flashAnim.restart() }
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 0
        width: 400

        // Trophy / skull
        Text {
            Layout.alignment: Qt.AlignHCenter
            text:  root.playerWon ? "🏆" : "💀"
            font.pixelSize: 72

            SequentialAnimation on scale {
                running: true; loops: 1
                NumberAnimation { from: 0.5; to: 1.15; duration: 400; easing.type: Easing.OutBack }
                NumberAnimation { from: 1.15; to: 1.0;  duration: 200 }
            }
        }

        Item { height: 16 }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text:  root.playerWon ? "YOU WIN!" : "CPU WINS"
            font.pixelSize: 48
            font.bold: true
            font.letterSpacing: 6
            color: root.playerWon ? "#44E5A0" : "#FF5E78"
        }

        Item { height: 6 }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: root.playerWon
                  ? "Well played! You reached the center first."
                  : "The AI beat you to the center. Try again!"
            font.pixelSize: 13
            color: "#6B7280"
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        Item { height: 40 }

        // Score breakdown
        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width:  340; height: 80
            radius: 16
            color:  "#151828"
            border.color: "#1E2340"
            border.width: 1

            RowLayout {
                anchors.centerIn: parent
                spacing: 48

                ColumnLayout {
                    spacing: 4
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: root.playerScore
                        font.pixelSize: 32
                        font.bold: true
                        color: "#44E5A0"
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "YOU"
                        font.pixelSize: 11
                        font.letterSpacing: 2
                        color: "#6B7280"
                    }
                }

                Text {
                    text: "VS"
                    font.pixelSize: 16
                    color: "#3A4060"
                    font.bold: true
                }

                ColumnLayout {
                    spacing: 4
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: root.cpuScore
                        font.pixelSize: 32
                        font.bold: true
                        color: "#FF5E78"
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "CPU"
                        font.pixelSize: 11
                        font.letterSpacing: 2
                        color: "#6B7280"
                    }
                }
            }
        }

        Item { height: 40 }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 16

            AnimatedButton {
                label:      "PLAY AGAIN"
                accentColor: "#44E5A0"
                width:  180; height: 50
                onClicked: root.playAgainClicked()
            }

            AnimatedButton {
                label:      "MENU"
                accentColor: "#6B7280"
                width:  120; height: 50
                onClicked: root.menuClicked()
            }
        }
    }

    // Entry animation
    opacity: 0
    NumberAnimation on opacity { from: 0; to: 1; duration: 500; running: true }
}
