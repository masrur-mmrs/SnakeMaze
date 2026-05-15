import QtQuick
import QtQuick.Layouts
import SnakeMaze 1.0

Rectangle {
    id: root
    color: "#0D0F1A"

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width; height: 1
        color: "#1E2340"
    }

    property int  playerScore: 0
    property int  cpuScore:    0
    property bool isPaused:    false

    signal pauseClicked
    signal quitClicked

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin:  16
        anchors.rightMargin: 16
        spacing: 0

        ScorePanel {
            label:      "YOU"
            score:      root.playerScore
            accentColor: "#44E5A0"
            isLeft:     true
        }

        Item { Layout.fillWidth: true }

        ColumnLayout {
            spacing: 2
            Layout.alignment: Qt.AlignVCenter

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "★ REACH CENTER ★"
                font.pixelSize: 11
                font.letterSpacing: 2
                color: "#FFD166"
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "highest score wins"
                font.pixelSize: 10
                color: "#3A4060"
            }
        }

        Item { Layout.fillWidth: true }

        RowLayout {
            spacing: 8

            Rectangle {
                width: 32; height: 32
                radius: 8
                color: pauseBtn.containsMouse ? "#1E2340" : "transparent"
                Behavior on color { ColorAnimation { duration: 120 } }

                Text {
                    anchors.centerIn: parent
                    text: root.isPaused ? "▶" : "⏸"
                    font.pixelSize: 14
                    color: "#9BA3B8"
                }

                MouseArea {
                    id: pauseBtn
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.pauseClicked()
                }
            }

            Rectangle {
                width: 32; height: 32
                radius: 8
                color: quitBtn.containsMouse ? "#2A1020" : "transparent"
                Behavior on color { ColorAnimation { duration: 120 } }

                Text {
                    anchors.centerIn: parent
                    text: "✕"
                    font.pixelSize: 14
                    color: quitBtn.containsMouse ? "#FF5E78" : "#9BA3B8"
                    Behavior on color { ColorAnimation { duration: 120 } }
                }

                MouseArea {
                    id: quitBtn
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: root.quitClicked()
                }
            }
        }

        Item { width: 12 }

        ScorePanel {
            label:       "CPU"
            score:       root.cpuScore
            accentColor: "#FF5E78"
            isLeft:      false
        }
    }
}
