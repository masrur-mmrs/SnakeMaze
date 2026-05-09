#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include <memory>

#include "GameState.h"
#include "MazeGenerator.h"
#include "Snake.h"
#include "PowerUp.h"

// Forward declarations
class AIController;

// ─────────────────────────────────────────────
//  GameEngine
//  Orchestrates all game subsystems and exposes
//  a clean QML-friendly API via Q_PROPERTY / signals.
// ─────────────────────────────────────────────
class GameEngine : public QObject
{
    Q_OBJECT

    // ── QML-visible properties ──────────────────
    Q_PROPERTY(GameState* gameState   READ gameState   NOTIFY gameStateChanged)
    Q_PROPERTY(int        tickRateMs  READ tickRateMs  NOTIFY tickRateChanged)
    Q_PROPERTY(bool       isPaused    READ isPaused    NOTIFY pauseChanged)

public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine() override;

    // ── Property accessors ──────────────────────
    GameState* gameState() const { return m_state.get(); }
    int        tickRateMs() const { return m_tickRateMs; }
    bool       isPaused()   const { return m_paused; }

    // ── Maze / grid constants ───────────────────
    static constexpr int COLS      = 21;   // must be odd for recursive-division maze
    static constexpr int ROWS      = 21;
    static constexpr int CELL_SIZE = 32;   // pixels (used by QML renderers)
    static constexpr int MAX_POWERUPS = 6;

public slots:
    // Called from QML
    void startGame(int difficulty);   // 0=Easy 1=Medium 2=Hard(A*)
    void resetGame();
    void pauseGame();
    void resumeGame();

    // Player input (direction: 0=Up 1=Right 2=Down 3=Left)
    void setPlayerDirection(int direction);

signals:
    void gameStateChanged();
    void tickRateChanged();
    void pauseChanged();

    // Game events (QML connects animations / sounds)
    void powerUpCollected(int x, int y, int type);
    void snakeGrew(bool isPlayer);
    void gameOver(bool playerWon, int playerScore, int cpuScore);
    void tick();                       // fired every game step

private slots:
    void onTick();

private:
    void initLevel();
    void spawnPowerUps();
    void checkCollisions();
    void checkGoal();
    void moveSnakes();

    std::unique_ptr<GameState>     m_state;
    std::unique_ptr<MazeGenerator> m_mazeGen;
    std::unique_ptr<Snake>         m_playerSnake;
    std::unique_ptr<Snake>         m_cpuSnake;
    std::unique_ptr<AIController>  m_aiController;
    QVector<PowerUp>               m_powerUps;

    QTimer m_tickTimer;
    int    m_tickRateMs = 150;
    bool   m_paused     = false;
    int    m_difficulty = 0;
    int    m_totalPowerUpsSpawned = 0;

    // Goal position (maze center)
    QPoint m_goalPos { COLS / 2, ROWS / 2 };
};
