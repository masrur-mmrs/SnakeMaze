import QtQuick
import QtQuick.Layouts
import SnakeMaze 1.0

Item {
    id: root

    required property GameEngine engine

    signal pauseClicked
    signal quitClicked

    focus: true
    Keys.onPressed: (event) => {
        switch (event.key) {
        case Qt.Key_Up:    case Qt.Key_W: engine.setPlayerDirection(0); break
        case Qt.Key_Right: case Qt.Key_D: engine.setPlayerDirection(1); break
        case Qt.Key_Down:  case Qt.Key_S: engine.setPlayerDirection(2); break
        case Qt.Key_Left:  case Qt.Key_A: engine.setPlayerDirection(3); break
        case Qt.Key_Escape: case Qt.Key_P: root.pauseClicked(); break
        }
        event.accepted = true
    }

    Component.onCompleted: root.forceActiveFocus()

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        HUDOverlay {
            id: hud
            Layout.fillWidth: true
            height: 72
            playerScore: engine.gameState ? engine.gameState.playerScore : 0
            cpuScore:    engine.gameState ? engine.gameState.cpuScore    : 0
            isPaused:    engine.isPaused
            onPauseClicked: root.pauseClicked()
            onQuitClicked:  root.quitClicked()
        }

        Item {
            Layout.fillWidth:  true
            Layout.fillHeight: true

            MazeRenderer {
                id: mazeView
                anchors.centerIn: parent

                gameState:  engine.gameState
                cellSize:   engine.CELL_SIZE

                SnakeRenderer {
                    cellSize: mazeView.cellSize
                    body: engine.gameState ? engine.gameState.playerBody : []
                    headColor: "#44E5A0"
                    bodyColor: "#2AC87A"
                }

                SnakeRenderer {
                    cellSize: mazeView.cellSize
                    body: engine.gameState ? engine.gameState.cpuBody : []
                    headColor: "#FF5555"
                    bodyColor: "#AA3333"
                }

                Connections {
                    target: engine.gameState
                    function onPowerUpCollected(x, y, type) {
                        mazeView.spawnBurst(x, y, type)
                    }
                }
            }

            Rectangle {
                anchors.fill: parent
                color: "#0D0F1A"
                opacity: engine.isPaused ? 0.75 : 0
                visible: opacity > 0
                Behavior on opacity { NumberAnimation { duration: 200 } }

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 12

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "PAUSED"
                        font.pixelSize: 42
                        font.bold: true
                        font.letterSpacing: 8
                        color: "#44E5A0"
                    }

                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Press P or ESC to resume"
                        font.pixelSize: 14
                        color: "#6B7280"
                    }
                }
            }
        }
    }
}
