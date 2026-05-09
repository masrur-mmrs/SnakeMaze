// ────────────────────────────────────────────────────────────
//  PowerUpItem.qml
//  Reusable power-up icon with label, used in legend or HUD.
// ────────────────────────────────────────────────────────────
import QtQuick
import QtQuick.Layouts

RowLayout {
    id: root
    property int  powerType: 0   // 0=Star 1=Berry 2=Crystal
    property bool showLabel: true

    spacing: 6

    readonly property var icons:  ["★", "✦", "◆"]
    readonly property var labels: ["+30 pts", "+15 pts", "+8 pts"]
    readonly property var colors: ["#FFD166", "#C77DFF", "#48CAE4"]

    Rectangle {
        width:  24; height: 24
        radius: 12
        color:  Qt.rgba(
            root.colors[root.powerType].r,
            root.colors[root.powerType].g,
            root.colors[root.powerType].b,
            0.18)
        border.color: root.colors[root.powerType]
        border.width: 1.5

        Text {
            anchors.centerIn: parent
            text: root.icons[root.powerType]
            font.pixelSize: 13
            color: root.colors[root.powerType]
        }
    }

    Text {
        visible:        root.showLabel
        text:           root.labels[root.powerType]
        font.pixelSize: 11
        color:          "#6B7280"
    }
}
