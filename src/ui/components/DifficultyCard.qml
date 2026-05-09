// ────────────────────────────────────────────────────────────
//  DifficultyCard.qml
//  A single difficulty selection card with hover animation.
// ────────────────────────────────────────────────────────────

import QtQuick
import QtQuick.Layouts

Rectangle {
    id: card

    property string label:       ""
    property string subtitle:    ""
    property color  accentColor: "#44E5A0"
    property string iconText:    ""
    property string descText:    ""
    property int    diffIndex:   0

    signal cardClicked(int index)

    width:  190
    height: 240
    radius: 16
    color:  hovered ? Qt.lighter("#151828", 1.15) : "#151828"
    border.color: hovered ? accentColor : "#1E2340"
    border.width: 2

    property bool hovered: false

    scale: hovered ? 1.04 : 1.0
    Behavior on scale       { NumberAnimation { duration: 150; easing.type: Easing.OutCubic } }
    Behavior on color       { ColorAnimation  { duration: 150 } }
    Behavior on border.color{ ColorAnimation  { duration: 150 } }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: card.iconText
            font.pixelSize: 36
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: card.label
            font.pixelSize: 18
            font.bold: true
            font.letterSpacing: 3
            color: card.accentColor
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: card.subtitle
            font.pixelSize: 11
            color: "#6B7280"
            font.letterSpacing: 1
        }

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            width: 120; height: 1
            color: card.accentColor
            opacity: 0.3
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: card.descText
            font.pixelSize: 12
            color: "#9BA3B8"
            horizontalAlignment: Text.AlignHCenter
            lineHeight: 1.4
            wrapMode: Text.WordWrap
            width: 160
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered:  card.hovered = true
        onExited:   card.hovered = false
        onClicked:  card.cardClicked(card.diffIndex)
        cursorShape: Qt.PointingHandCursor
    }
}
