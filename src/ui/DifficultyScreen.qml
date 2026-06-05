import QtQuick
import QtQuick.Layouts
import SnakeMaze 1.0

Item {
    id: root
    signal difficultySelected(int difficulty)
    signal backClicked

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 0
        width: Math.min(parent.width - 64, 700)

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "SELECT DIFFICULTY"
            font.pixelSize: 30
            font.letterSpacing: 6
            font.bold: true
            color: "#F0F4FF"
        }

        Item { height: 8 }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "How clever is your opponent?"
            font.pixelSize: 13
            color: "#6B7280"
        }

        Item { height: 48 }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 20

            DifficultyCard {
                label:       "EASY"
                subtitle:    "Random Walk"
                accentColor: "#44E5A0"
                iconText:    "🌿"
                descText:    "CPU wanders aimlessly.\nPerfect for beginners."
                diffIndex:   0
                onCardClicked: (i) => root.difficultySelected(i)
            }

            DifficultyCard {
                label:       "MEDIUM"
                subtitle:    "Dijkstra Algorithm"
                accentColor: "#FFD166"
                iconText:    "⚡"
                descText:    "CPU finds shortest path,\navoiding tight corridors."
                diffIndex:   1
                onCardClicked: (i) => root.difficultySelected(i)
            }

            DifficultyCard {
                label:       "HARD"
                subtitle:    "A* Algorithm"
                accentColor: "#FF5E78"
                iconText:    "🧠"
                descText:    "CPU uses optimal\nA* pathfinding."
                diffIndex:   2
                onCardClicked: (i) => root.difficultySelected(i)
            }
        }

        Item { height: 48 }

        AnimatedButton {
            Layout.alignment: Qt.AlignHCenter
            label:       "← BACK"
            accentColor: "#6B7280"
            width:  140
            height: 42
            onClicked: root.backClicked()
        }
    }

    opacity: 0
    NumberAnimation on opacity { from: 0; to: 1; duration: 400; running: true }
}
