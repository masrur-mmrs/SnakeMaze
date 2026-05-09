#include "Snake.h"
#include <QVariantMap>

Snake::Snake(bool isPlayer, QObject* parent)
    : QObject(parent)
    , m_isPlayer(isPlayer)
{}

// ─────────────────────────────────────────────
void Snake::reset(const QPoint& startPos, Direction startDir)
{
    m_body.clear();
    m_body.append(startPos);
    m_direction        = startDir;
    m_desiredDirection = startDir;
    m_growPending      = 2;   // start with length 3
    m_alive            = true;
    m_score            = 0;

    // Build initial body behind the head
    QPoint offset;
    switch (startDir) {
    case Direction::Right: offset = {-1,  0}; break;
    case Direction::Left:  offset = { 1,  0}; break;
    case Direction::Up:    offset = { 0,  1}; break;
    case Direction::Down:  offset = { 0, -1}; break;
    }
    for (int i = 1; i <= 2; ++i)
        m_body.append(startPos + offset * i);

    emit bodyChanged();
    emit aliveChanged();
    emit scoreChanged();
}

// ─────────────────────────────────────────────
void Snake::move(const QVector<QVector<int>>& grid, int cols, int rows)
{
    if (!m_alive) return;

    // Apply desired direction (ignore 180° reversal)
    if (!isOppositeDirection(m_direction, m_desiredDirection))
        m_direction = m_desiredDirection;

    QPoint delta;
    switch (m_direction) {
    case Direction::Up:    delta = { 0, -1}; break;
    case Direction::Right: delta = { 1,  0}; break;
    case Direction::Down:  delta = { 0,  1}; break;
    case Direction::Left:  delta = {-1,  0}; break;
    }

    QPoint newHead = m_body.first() + delta;

    // Boundary check
    if (newHead.x() < 0 || newHead.x() >= cols ||
        newHead.y() < 0 || newHead.y() >= rows) {
        m_alive = false;
        emit aliveChanged();
        return;
    }

    // Wall check (grid value 1 = wall)
    if (grid[newHead.y()][newHead.x()] == 1) {
        // Snake bounces / stops rather than dying (softer gameplay)
        // Just don't move this tick
        return;
    }

    m_body.prepend(newHead);

    if (m_growPending > 0) {
        --m_growPending;  // don't remove tail
    } else {
        m_body.removeLast();
    }

    emit bodyChanged();
}

// ─────────────────────────────────────────────
void Snake::grow(int segments)
{
    m_growPending += segments;
}

// ─────────────────────────────────────────────
void Snake::setDesiredDirection(Direction dir)
{
    m_desiredDirection = dir;
}

// ─────────────────────────────────────────────
QVariantList Snake::bodyVariant() const
{
    QVariantList list;
    list.reserve(m_body.size());
    for (const QPoint& p : m_body) {
        QVariantMap m;
        m["x"] = p.x();
        m["y"] = p.y();
        list.append(m);
    }
    return list;
}

// ─────────────────────────────────────────────
bool Snake::isOppositeDirection(Direction a, Direction b) const
{
    return (static_cast<int>(a) + 2) % 4 == static_cast<int>(b);
}
