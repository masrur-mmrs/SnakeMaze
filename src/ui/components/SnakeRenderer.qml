import QtQuick

Canvas {
    id: root

    property var   body:       []
    property color headColor:  "#44E5A0"
    property color bodyColor:  "#2AC87A"
    property int   cellSize:   24

    onBodyChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        var cs  = cellSize
        ctx.clearRect(0, 0, width, height)

        if (!body || body.length === 0) return

        for (var i = 0; i < body.length; i++) {
            var seg = body[i]
            var cx  = seg.x * cs + cs / 2
            var cy  = seg.y * cs + cs / 2
            var r   = cs * 0.38 * (i === 0 ? 1.15 : (1.0 - i / body.length * 0.4))

            ctx.globalAlpha = i === 0 ? 1.0 : (0.9 - i / body.length * 0.5)
            ctx.fillStyle   = i === 0 ? headColor : bodyColor

            ctx.beginPath()
            ctx.arc(cx, cy, Math.max(r, 2), 0, Math.PI * 2)
            ctx.fill()

            if (i < body.length - 1) {
                var next = body[i + 1]
                ctx.strokeStyle = bodyColor
                ctx.lineWidth   = r * 1.6
                ctx.lineCap     = "round"
                ctx.beginPath()
                ctx.moveTo(cx, cy)
                ctx.lineTo(next.x * cs + cs/2, next.y * cs + cs/2)
                ctx.stroke()
            }
        }
        ctx.globalAlpha = 1
    }
}
