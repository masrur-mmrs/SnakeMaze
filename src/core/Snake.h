#pragma once

#include <QObject>
#include <QPoint>
#include <QVector>

// ─────────────────────────────────────────────
//  Snake
//  Represents either the player or CPU snake.
//  Exposed to QML through GameState as a simple
//  value container (body positions + metadata).
// ─────────────────────────────────────────────
class Snake : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList body    READ bodyVariant  NOTIFY bodyChanged)
    Q_PROPERTY(int          score   READ score        NOTIFY scoreChanged)
    Q_PROPERTY(bool         isAlive READ isAlive      NOTIFY aliveChanged)
    Q_PROPERTY(bool         isPlayer READ isPlayer   CONSTANT)

public:
    enum class Direction { Up = 0, Right = 1, Down = 2, Left = 3 };
    Q_ENUM(Direction)

    explicit Snake(bool isPlayer, QObject* parent = nullptr);

    // ── State management ────────────────────────
    void reset(const QPoint& startPos, Direction startDir);
    void move(const QVector<QVector<int>>& grid, int cols, int rows);
    void grow(int segments = 1);
    void setDesiredDirection(Direction dir);

    // ── Accessors ───────────────────────────────
    QPoint              head()       const { return m_body.isEmpty() ? QPoint{} : m_body.first(); }
    const QVector<QPoint>& body()   const { return m_body; }
    QVariantList        bodyVariant() const;
    int                 score()     const { return m_score; }
    bool                isAlive()   const { return m_alive; }
    bool                isPlayer()  const { return m_isPlayer; }
    Direction           direction() const { return m_direction; }

    // Score helpers (used by GameEngine)
    void addScore(int pts) { m_score += pts; emit scoreChanged(); }
    void resetScore()      { m_score = 0;    emit scoreChanged(); }

signals:
    void bodyChanged();
    void scoreChanged();
    void aliveChanged();

private:
    bool isOppositeDirection(Direction a, Direction b) const;

    QVector<QPoint> m_body;
    Direction       m_direction        = Direction::Right;
    Direction       m_desiredDirection = Direction::Right;
    int             m_score            = 0;
    int             m_growPending      = 0;
    bool            m_alive            = true;
    bool            m_isPlayer         = true;
};
