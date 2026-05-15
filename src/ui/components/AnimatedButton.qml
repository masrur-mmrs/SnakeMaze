import QtQuick

Rectangle {
    id: root
    property string label:       "BUTTON"
    property color  accentColor: "#44E5A0"

    signal clicked

    radius: 12
    color:  area.pressed
            ? Qt.darker(accentColor, 1.5)
            : area.containsMouse
              ? Qt.rgba(accentColor.r, accentColor.g, accentColor.b, 0.18)
              : "transparent"

    border.color: accentColor
    border.width: area.containsMouse ? 2 : 1.5

    Behavior on color        { ColorAnimation { duration: 120 } }
    Behavior on border.width { NumberAnimation { duration: 120 } }

    scale: area.pressed ? 0.96 : 1.0
    Behavior on scale { NumberAnimation { duration: 100; easing.type: Easing.OutCubic } }

    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: "transparent"
        border.color: root.accentColor
        border.width: area.containsMouse ? 6 : 0
        opacity: 0.15
        Behavior on border.width { NumberAnimation { duration: 150 } }
    }

    Text {
        anchors.centerIn: parent
        text: root.label
        font.pixelSize: 13
        font.bold: true
        font.letterSpacing: 2
        color: area.containsMouse ? root.accentColor : Qt.lighter(root.accentColor, 1.3)
        Behavior on color { ColorAnimation { duration: 120 } }
    }

    MouseArea {
        id: area
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}
