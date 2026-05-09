// ────────────────────────────────────────────────────────────
//  MenuScreen.qml
//  Animated main menu with floating particle background.
// ────────────────────────────────────────────────────────────
import QtQuick
import QtQuick.Layouts
import SnakeMaze 1.0

Item {
    id: root
    signal playClicked

    // ── Animated background particles ──────────────────────
    Repeater {
        model: 24
        delegate: Rectangle {
            required property int index
            width:  4 + (index % 5) * 2
            height: width
            radius: width / 2
            color:  ["#44E5A0","#FF5E78","#FFD166","#C77DFF","#48CAE4"][index % 5]
            opacity: 0.15 + (index % 4) * 0.07

            property real startX: Math.random() * root.width
            property real startY: Math.random() * root.height

            x: startX
            y: startY

            SequentialAnimation on y {
                loops: Animation.Infinite
                NumberAnimation {
                    from: startY
                    to:   startY - (60 + index * 8)
                    duration: 3000 + index * 400
                    easing.type: Easing.InOutSine
                }
                NumberAnimation {
                    from: startY - (60 + index * 8)
                    to:   startY
                    duration: 3000 + index * 400
                    easing.type: Easing.InOutSine
                }
            }

            NumberAnimation on opacity {
                loops: Animation.Infinite
                from: 0.08
                to:   0.22
                duration: 2000 + index * 300
            }
        }
    }

    // ── Subtle grid lines ──────────────────────────────────
    Canvas {
        anchors.fill: parent
        opacity: 0.04
        onPaint: {
            var ctx = getContext("2d")
            ctx.strokeStyle = "#44E5A0"
            ctx.lineWidth   = 1
            var step = 40
            for (var x = 0; x < width; x += step) {
                ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, height); ctx.stroke()
            }
            for (var y = 0; y < height; y += step) {
                ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(width, y); ctx.stroke()
            }
        }
    }

    // ── Centered content ───────────────────────────────────
    ColumnLayout {
        anchors.centerIn: parent
        spacing: 0

        // Logo / title
        Item {
            Layout.alignment: Qt.AlignHCenter
            width:  320
            height: 140

            // Glow behind title
            Rectangle {
                anchors.centerIn: parent
                width:   260; height: 80
                radius:  40
                color:   "transparent"
                border.width: 0

                layer.enabled: true
                layer.effect: null   // Replace with QtGraphicalEffects if available

                Rectangle {
                    anchors.centerIn: parent
                    width:  200; height: 60
                    radius: 30
                    color:  "#44E5A0"
                    opacity: 0.08
                }
            }

            Text {
                anchors.centerIn: parent
                text: "SNAKE"
                font.pixelSize: 62
                font.letterSpacing: 8
                font.bold: true
                color: "#44E5A0"

                SequentialAnimation on opacity {
                    loops: Animation.Infinite
                    NumberAnimation { to: 0.7; duration: 1800; easing.type: Easing.InOutSine }
                    NumberAnimation { to: 1.0; duration: 1800; easing.type: Easing.InOutSine }
                }
            }

            Text {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: "MAZE"
                font.pixelSize: 28
                font.letterSpacing: 14
                color: "#FFD166"
            }
        }

        Item { height: 16 }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "PLAYER vs CPU • REACH THE CENTER • COLLECT POWER-UPS"
            font.pixelSize: 11
            font.letterSpacing: 2
            color: "#6B7280"
        }

        Item { height: 52 }

        // Play button
        AnimatedButton {
            Layout.alignment: Qt.AlignHCenter
            label:      "PLAY GAME"
            accentColor: "#44E5A0"
            width:  220
            height: 54
            onClicked: root.playClicked()
        }

        Item { height: 16 }

        // How to play
        Text {
            Layout.alignment: Qt.AlignHCenter
            text: "Use ← ↑ ↓ → or WASD to steer your snake"
            font.pixelSize: 12
            color: "#6B7280"
        }
    }

    // ── Version tag ────────────────────────────────────────
    Text {
        anchors.bottom: parent.bottom
        anchors.right:  parent.right
        anchors.margins: 16
        text: "v1.0"
        font.pixelSize: 11
        color: "#2A3050"
    }

    // ── Entry animation ────────────────────────────────────
    opacity: 0
    NumberAnimation on opacity {
        from: 0; to: 1
        duration: 600
        easing.type: Easing.OutCubic
        running: true
    }
}
