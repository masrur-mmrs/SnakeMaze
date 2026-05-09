// ────────────────────────────────────────────────────────────
//  ParticleEffect.qml
//  Short-lived burst of particles spawned at power-up
//  collection point. Auto-destroys when animation ends.
// ────────────────────────────────────────────────────────────
import QtQuick

Item {
    id: root
    property color burstColor: "#FFD166"

    width:  1; height: 1   // zero-size anchor point

    Component.onCompleted: killTimer.start()

    Timer {
        id: killTimer
        interval: 700
        onTriggered: root.destroy()
    }

    Repeater {
        model: 8
        delegate: Rectangle {
            required property int index
            id: particle

            width:  4 + (index % 3) * 2
            height: width
            radius: width / 2
            color:  root.burstColor
            opacity: 1

            property real angle: index * (Math.PI * 2 / 8)
            property real dist:  28 + (index % 3) * 12

            x: -width / 2
            y: -height / 2

            ParallelAnimation {
                running: true

                NumberAnimation {
                    target: particle
                    property: "x"
                    from: -particle.width / 2
                    to:   Math.cos(particle.angle) * particle.dist - particle.width / 2
                    duration: 550
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    target: particle
                    property: "y"
                    from: -particle.height / 2
                    to:   Math.sin(particle.angle) * particle.dist - particle.height / 2
                    duration: 550
                    easing.type: Easing.OutCubic
                }
                NumberAnimation {
                    target: particle
                    property: "opacity"
                    from: 1; to: 0
                    duration: 550
                    easing.type: Easing.InCubic
                }
                NumberAnimation {
                    target: particle
                    property: "scale"
                    from: 1.4; to: 0.4
                    duration: 550
                }
            }
        }
    }
}
