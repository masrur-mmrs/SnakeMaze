// ────────────────────────────────────────────────────────────
//  Main.qml
//  Root application window. Manages screen transitions
//  between Menu → Difficulty → Game → GameOver.
// ────────────────────────────────────────────────────────────
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material
import SnakeMaze 1.0

ApplicationWindow {
    id: root
    width:  900
    height: 720
    visible: true
    title:   "SnakeMaze"

    // ── Color palette (flat, vivid) ─────────────────────────
    readonly property color bgDeep:        "#0D0F1A"
    readonly property color bgCard:        "#151828"
    readonly property color accentPlayer:  "#44E5A0"   // mint green
    readonly property color accentCPU:     "#FF5E78"   // coral red
    readonly property color accentGoal:    "#FFD166"   // golden yellow
    readonly property color accentStar:    "#FFD166"
    readonly property color accentBerry:   "#C77DFF"
    readonly property color accentCrystal: "#48CAE4"
    readonly property color textPrimary:   "#F0F4FF"
    readonly property color textMuted:     "#6B7280"
    readonly property color wallColor:     "#1E2340"
    readonly property color pathColor:     "#111428"

    color: bgDeep

    // ── Central game engine instance ────────────────────────
    GameEngine {
        id: engine

        onGameOver: (playerWon, ps, cs) => {
            gameOverScreen.playerWon   = playerWon
            gameOverScreen.playerScore = ps
            gameOverScreen.cpuScore    = cs
            screenStack.currentIndex   = 3  // show game over
        }

        onPowerUpCollected: (x, y, type) => {
            // Burst particle is created by GameScreen watching this signal
        }
    }

    // ── Screen stack ────────────────────────────────────────
    StackLayout {
        id: screenStack
        anchors.fill: parent
        currentIndex: 0   // 0=Menu, 1=Difficulty, 2=Game, 3=GameOver

        // 0 ── Main Menu
        MenuScreen {
            id: menuScreen
            onPlayClicked:    screenStack.currentIndex = 1
        }

        // 1 ── Difficulty selection
        DifficultyScreen {
            id: diffScreen
            onDifficultySelected: (diff) => {
                engine.startGame(diff)
                screenStack.currentIndex = 2
            }
            onBackClicked: screenStack.currentIndex = 0
        }

        // 2 ── Gameplay
        GameScreen {
            id: gameScreen
            engine:    engine
            onPauseClicked: engine.isPaused ? engine.resumeGame() : engine.pauseGame()
            onQuitClicked: {
                engine.pauseGame()
                screenStack.currentIndex = 0
            }
        }

        // 3 ── Game Over
        GameOverScreen {
            id: gameOverScreen
            onPlayAgainClicked: {
                engine.resetGame()
                screenStack.currentIndex = 2
            }
            onMenuClicked: screenStack.currentIndex = 0
        }
    }

    // ── Screen transition overlay ───────────────────────────
    Rectangle {
        id: transitionOverlay
        anchors.fill: parent
        color: root.bgDeep
        opacity: 0
        visible: opacity > 0
        z: 99

        Connections {
            target: screenStack
            function onCurrentIndexChanged() {
                fadeAnim.restart()
            }
        }

        SequentialAnimation {
            id: fadeAnim
            NumberAnimation { target: transitionOverlay; property: "opacity"; to: 1; duration: 120 }
            NumberAnimation { target: transitionOverlay; property: "opacity"; to: 0; duration: 200 }
        }
    }
}
