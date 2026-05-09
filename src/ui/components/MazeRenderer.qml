// ────────────────────────────────────────────────────────────
//  MazeRenderer.qml
//  Canvas-based renderer for the maze grid.
//  Draws: walls, paths, goal, power-ups, and both snakes.
// ────────────────────────────────────────────────────────────
import QtQuick

Item {
    id: root

    property var  gameState: null
    property int  cellSize:  32

    // Derived dimensions
    readonly property int cols: 21
    readonly property int rows: 21

    width:  cols * cellSize
    height: rows * cellSize

    // ── Burst particles layer ──────────────────────────────
    property var burstPool: []

    function spawnBurst(cellX, cellY, type) {
        var colors = ["#FFD166", "#C77DFF", "#48CAE4"]
        var color  = colors[type] || "#FFD166"
        var comp   = Qt.createComponent("ParticleEffect.qml")
        if (comp.status === Component.Ready) {
            var obj = comp.createObject(root, {
                x:          cellX * cellSize + cellSize / 2,
                y:          cellY * cellSize + cellSize / 2,
                burstColor: color
            })
        }
    }

    // ── Connections to game state ─────────────────────────
    Connections {
        target: root.gameState
        function onMazeChanged()     { mazeCanvas.requestPaint() }
        function onBodyChanged()     { snakeCanvas.requestPaint() }
        function onPowerUpsChanged() { powerUpCanvas.requestPaint() }
    }

    // ── Layer 1: Static maze (walls + paths + goal) ───────
    Canvas {
        id: mazeCanvas
        anchors.fill: parent
        renderStrategy: Canvas.Threaded

        onPaint: {
            if (!root.gameState) return
            var ctx  = getContext("2d")
            var maze = root.gameState.maze
            var cs   = root.cellSize

            ctx.clearRect(0, 0, width, height)

            for (var r = 0; r < maze.length; r++) {
                var row = maze[r]
                for (var c = 0; c < row.length; c++) {
                    var val = row[c]
                    var x   = c * cs
                    var y   = r * cs

                    if (val === 1) {
                        // Wall: layered dark blue rectangle
                        ctx.fillStyle = "#0F1225"
                        ctx.fillRect(x, y, cs, cs)
                        // Subtle highlight edge
                        ctx.fillStyle = "#1A2040"
                        ctx.fillRect(x, y, cs, 2)
                        ctx.fillRect(x, y, 2, cs)
                    } else if (val === 2) {
                        // Goal cell: golden glow
                        ctx.fillStyle = "#111428"
                        ctx.fillRect(x, y, cs, cs)
                        // Inner glow circle
                        var grd = ctx.createRadialGradient(
                            x + cs/2, y + cs/2, 2,
                            x + cs/2, y + cs/2, cs * 0.7)
                        grd.addColorStop(0,   "rgba(255,209,102,0.55)")
                        grd.addColorStop(0.5, "rgba(255,209,102,0.15)")
                        grd.addColorStop(1,   "rgba(255,209,102,0)")
                        ctx.fillStyle = grd
                        ctx.fillRect(x, y, cs, cs)
                        // Star symbol
                        ctx.fillStyle = "#FFD166"
                        ctx.font = Math.floor(cs * 0.65) + "px serif"
                        ctx.textAlign    = "center"
                        ctx.textBaseline = "middle"
                        ctx.fillText("★", x + cs/2, y + cs/2)
                    } else {
                        // Open path: very dark navy
                        ctx.fillStyle = "#111428"
                        ctx.fillRect(x, y, cs, cs)
                    }
                }
            }

            // Outer border
            ctx.strokeStyle = "#44E5A0"
            ctx.lineWidth   = 2
            ctx.strokeRect(1, 1, width - 2, height - 2)
        }
    }

    // ── Layer 2: Power-ups ─────────────────────────────────
    Canvas {
        id: powerUpCanvas
        anchors.fill: parent

        onPaint: {
            if (!root.gameState) return
            var ctx    = getContext("2d")
            var cs     = root.cellSize
            var ups    = root.gameState.powerUps

            ctx.clearRect(0, 0, width, height)

            var icons   = ["★", "✦", "◆"]
            var colors  = ["#FFD166", "#C77DFF", "#48CAE4"]
            var glows   = ["rgba(255,209,102,", "rgba(199,125,255,", "rgba(72,202,228,"]

            for (var i = 0; i < ups.length; i++) {
                var up = ups[i]
                var px = up.x * cs + cs/2
                var py = up.y * cs + cs/2
                var t  = up.type

                // Glow
                var grd = ctx.createRadialGradient(px, py, 1, px, py, cs * 0.55)
                grd.addColorStop(0,   glows[t] + "0.4)")
                grd.addColorStop(1,   glows[t] + "0)")
                ctx.fillStyle = grd
                ctx.beginPath()
                ctx.arc(px, py, cs * 0.55, 0, Math.PI * 2)
                ctx.fill()

                // Circle bg
                ctx.fillStyle = "#151828"
                ctx.beginPath()
                ctx.arc(px, py, cs * 0.30, 0, Math.PI * 2)
                ctx.fill()

                // Icon
                ctx.fillStyle    = colors[t]
                ctx.font         = Math.floor(cs * 0.38) + "px serif"
                ctx.textAlign    = "center"
                ctx.textBaseline = "middle"
                ctx.fillText(icons[t], px, py)
            }
        }
    }

    // ── Layer 3: Snakes ────────────────────────────────────
    Canvas {
        id: snakeCanvas
        anchors.fill: parent

        onPaint: {
            if (!root.gameState) return
            var ctx  = getContext("2d")
            var cs   = root.cellSize

            ctx.clearRect(0, 0, width, height)

            drawSnake(ctx, root.gameState.playerBody, "#44E5A0", "#2AC87A", cs, true)
            drawSnake(ctx, root.gameState.cpuBody,    "#FF5E78", "#CC2244", cs, false)
        }

        function drawSnake(ctx, body, headColor, bodyColor, cs, isPlayer) {
            if (!body || body.length === 0) return

            var radius = cs * 0.38

            // Draw body segments (back to front)
            for (var i = body.length - 1; i >= 0; i--) {
                var seg = body[i]
                var cx  = seg.x * cs + cs/2
                var cy  = seg.y * cs + cs/2

                var t = i / (body.length - 1)   // 0 = head, 1 = tail

                if (i === 0) {
                    // Head: larger, glowing circle
                    var grd = ctx.createRadialGradient(cx, cy, 1, cx, cy, cs * 0.55)
                    grd.addColorStop(0,   headColor + "55")
                    grd.addColorStop(1,   headColor + "00")
                    ctx.fillStyle = grd
                    ctx.beginPath()
                    ctx.arc(cx, cy, cs * 0.55, 0, Math.PI * 2)
                    ctx.fill()

                    ctx.fillStyle = headColor
                    ctx.beginPath()
                    ctx.arc(cx, cy, radius * 1.15, 0, Math.PI * 2)
                    ctx.fill()

                    // Eyes
                    drawEyes(ctx, body, i, cx, cy, cs, isPlayer)

                } else {
                    // Body: fades toward tail
                    var alpha = 0.85 - t * 0.45
                    ctx.globalAlpha = alpha
                    ctx.fillStyle   = bodyColor

                    var segRadius = radius * (1.0 - t * 0.35)
                    ctx.beginPath()
                    ctx.arc(cx, cy, Math.max(segRadius, 3), 0, Math.PI * 2)
                    ctx.fill()

                    // Segment connector line to next
                    if (i < body.length - 1) {
                        var next = body[i + 1]
                        ctx.strokeStyle = bodyColor
                        ctx.lineWidth   = segRadius * 1.6
                        ctx.lineCap     = "round"
                        ctx.beginPath()
                        ctx.moveTo(cx, cy)
                        ctx.lineTo(next.x * cs + cs/2, next.y * cs + cs/2)
                        ctx.stroke()
                    }

                    ctx.globalAlpha = 1
                }
            }
        }

        function drawEyes(ctx, body, i, cx, cy, cs, isPlayer) {
            // Determine direction for eye placement
            var dx = 0, dy = 0
            if (body.length > 1) {
                dx = body[0].x - body[1].x
                dy = body[0].y - body[1].y
            }

            var perpX = -dy, perpY = dx
            var eyeOffset = cs * 0.14
            var fwdOffset  = cs * 0.10

            var ex1 = cx + perpX * eyeOffset + dx * fwdOffset
            var ey1 = cy + perpY * eyeOffset + dy * fwdOffset
            var ex2 = cx - perpX * eyeOffset + dx * fwdOffset
            var ey2 = cy - perpY * eyeOffset + dy * fwdOffset
            var er  = cs * 0.10

            ctx.fillStyle = "#FFFFFF"
            ctx.beginPath()
            ctx.arc(ex1, ey1, er, 0, Math.PI * 2)
            ctx.fill()
            ctx.beginPath()
            ctx.arc(ex2, ey2, er, 0, Math.PI * 2)
            ctx.fill()

            ctx.fillStyle = isPlayer ? "#0D2A1A" : "#2A0010"
            ctx.beginPath()
            ctx.arc(ex1 + dx*er*0.4, ey1 + dy*er*0.4, er*0.55, 0, Math.PI * 2)
            ctx.fill()
            ctx.beginPath()
            ctx.arc(ex2 + dx*er*0.4, ey2 + dy*er*0.4, er*0.55, 0, Math.PI * 2)
            ctx.fill()
        }
    }

    // ── Power-up pulse animation ───────────────────────────
    Timer {
        interval: 60
        running:  true
        repeat:   true
        property real phase: 0
        onTriggered: {
            phase += 0.12
            powerUpCanvas.requestPaint()
        }
    }
}
