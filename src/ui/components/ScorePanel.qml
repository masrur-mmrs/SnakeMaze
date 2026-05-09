// ────────────────────────────────────────────────────────────
//  ScorePanel.qml
//  Compact score display for HUD.
// ────────────────────────────────────────────────────────────
import QtQuick
import QtQuick.Layouts

ColumnLayout {
    id: root
    property string label:       "PLAYER"
    property int    score:       0
    property color  accentColor: "#44E5A0"
    property bool   isLeft:      true

    spacing: 2

    Text {
        Layout.alignment: isLeft ? Qt.AlignLeft : Qt.AlignRight
        text: root.label
        font.pixelSize: 10
        font.letterSpacing: 2
        color: root.accentColor
        opacity: 0.7
    }

    Text {
        id: scoreText
        Layout.alignment: isLeft ? Qt.AlignLeft : Qt.AlignRight

        property int displayScore: root.score
        text: displayScore.toString().padStart(4, "0")

        font.pixelSize: 28
        font.bold: true
        color: root.accentColor

        Behavior on displayScore {
            NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
        }

        // Pop animation on score change
        onDisplayScoreChanged: {
            popAnim.restart()
        }

        SequentialAnimation {
            id: popAnim
            NumberAnimation { target: scoreText; property: "scale"; to: 1.25; duration: 120; easing.type: Easing.OutBack }
            NumberAnimation { target: scoreText; property: "scale"; to: 1.0;  duration: 180 }
        }
    }
}
