#pragma once

#include <QObject>
#include <QVariantList>
#include "PowerUp.h"

class Snake;

// ─────────────────────────────────────────────
//  GameState
//  Read-only mirror of game world state for QML.
//  GameEngine writes; QML reads via Q_PROPERTY.
// ─────────────────────────────────────────────
class GameState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList maze        READ maze        NOTIFY mazeChanged)
    Q_PROPERTY(QVariantList playerBody  READ playerBody  NOTIFY bodyChanged)
    Q_PROPERTY(QVariantList cpuBody     READ cpuBody     NOTIFY bodyChanged)
    Q_PROPERTY(QVariantList powerUps    READ powerUps    NOTIFY powerUpsChanged)
    Q_PROPERTY(int          playerScore READ playerScore NOTIFY scoreChanged)
    Q_PROPERTY(int          cpuScore    READ cpuScore    NOTIFY scoreChanged)
    Q_PROPERTY(int          goalX       READ goalX       CONSTANT)
    Q_PROPERTY(int          goalY       READ goalY       CONSTANT)
    Q_PROPERTY(int playerDirAngle READ playerDirAngle NOTIFY bodyChanged)
    Q_PROPERTY(int cpuDirAngle    READ cpuDirAngle    NOTIFY bodyChanged)

public:
    explicit GameState(QObject* parent = nullptr);

    // ── Accessors (QML-facing) ──────────────────
    QVariantList maze()        const { return m_maze;        }
    QVariantList playerBody()  const;
    QVariantList cpuBody()     const;
    QVariantList powerUps()    const { return m_powerUps;    }
    int          playerScore() const { return m_playerScore; }
    int          cpuScore()    const { return m_cpuScore;    }
    int          goalX()       const { return 10; }  // COLS/2
    int          goalY()       const { return 10; }  // ROWS/2
    int playerDirAngle() const;
    int cpuDirAngle()    const;

    // ── Setters (called by GameEngine) ──────────
    void setMaze(const QVector<QVector<int>>& grid);
    void setPlayerSnake(Snake* s) { m_playerSnake = s; }
    void setCpuSnake(Snake* s)    { m_cpuSnake    = s; }
    void setPowerUps(const QVector<PowerUp>& ups);

    void addPlayerScore(int pts);
    void addCpuScore(int pts);
    void resetScores();
    void notifyUpdates() { emit bodyChanged(); }

signals:
    void mazeChanged();
    void bodyChanged();
    void powerUpsChanged();
    void scoreChanged();

private:
    QVariantList m_maze;
    QVariantList m_powerUps;
    Snake*       m_playerSnake = nullptr;
    Snake*       m_cpuSnake    = nullptr;
    int          m_playerScore = 0;
    int          m_cpuScore    = 0;
};
