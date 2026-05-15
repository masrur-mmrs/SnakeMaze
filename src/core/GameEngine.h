#pragma once

#include <QObject>
#include <QTimer>
#include <QVector>
#include <memory>

#include "GameState.h"
#include "MazeGenerator.h"
#include "Snake.h"
#include "PowerUp.h"

class AIController;

class GameEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(GameState* gameState   READ gameState   NOTIFY gameStateChanged)
    Q_PROPERTY(int        tickRateMs  READ tickRateMs  NOTIFY tickRateChanged)
    Q_PROPERTY(bool       isPaused    READ isPaused    NOTIFY pauseChanged)

public:
    explicit GameEngine(QObject* parent = nullptr);
    ~GameEngine() override;

    GameState* gameState() const { return m_state.get(); }
    int        tickRateMs() const { return m_tickRateMs; }
    bool       isPaused()   const { return m_paused; }

    static constexpr int COLS      = 21;
    static constexpr int ROWS      = 21;
    static constexpr int CELL_SIZE = 32;
    static constexpr int MAX_POWERUPS = 6;

public slots:
    void startGame(int difficulty);
    void resetGame();
    void pauseGame();
    void resumeGame();

    void setPlayerDirection(int direction);

signals:
    void gameStateChanged();
    void tickRateChanged();
    void pauseChanged();

    void powerUpCollected(int x, int y, int type);
    void snakeGrew(bool isPlayer);
    void gameOver(bool playerWon, int playerScore, int cpuScore);
    void tick();

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

    QPoint m_goalPos { COLS / 2, ROWS / 2 };
};
