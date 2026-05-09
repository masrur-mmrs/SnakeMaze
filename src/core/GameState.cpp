#include "GameState.h"
#include "Snake.h"

#include <QVariantMap>

GameState::GameState(QObject* parent)
    : QObject(parent)
{}

// ─────────────────────────────────────────────
void GameState::setMaze(const QVector<QVector<int>>& grid)
{
    m_maze.clear();
    for (const auto& row : grid) {
        QVariantList qrow;
        for (int v : row)
            qrow.append(v);
        m_maze.append(QVariant::fromValue(qrow));
    }
    emit mazeChanged();
}

// ─────────────────────────────────────────────
void GameState::setPowerUps(const QVector<PowerUp>& ups)
{
    m_powerUps.clear();
    for (const PowerUp& p : ups)
        m_powerUps.append(p.toVariant());
    emit powerUpsChanged();
}

QVariantList GameState::playerBody() const {
    QVariantList list;
    if (!m_playerSnake) return list;

    for (const QPoint& p : m_playerSnake->body()) {
        QVariantMap map;
        map["x"] = p.x();
        map["y"] = p.y();
        list << map;
    }
    return list;
}

QVariantList GameState::cpuBody() const {
    QVariantList list;
    if (!m_cpuSnake) return list;

    const auto& segments = m_cpuSnake->body();
    for (const QPoint& p : segments) {
        QVariantMap map;
        map["x"] = p.x();
        map["y"] = p.y();
        list.append(map);
    }
    return list;
}
// ─────────────────────────────────────────────
void GameState::addPlayerScore(int pts)
{
    m_playerScore += pts;
    emit scoreChanged();
}

void GameState::addCpuScore(int pts)
{
    m_cpuScore += pts;
    emit scoreChanged();
}

void GameState::resetScores()
{
    m_playerScore = 0;
    m_cpuScore    = 0;
    emit scoreChanged();
}
