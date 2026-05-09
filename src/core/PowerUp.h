#pragma once

#include <QPoint>
#include <QObject>
#include <QVariantList>
#include <QVariantMap>

// ─────────────────────────────────────────────
//  PowerUp
//  Value type representing a collectible item
//  on the maze grid.
//
//  Types:
//   Star   – high points, small grow
//   Berry  – medium points, medium grow
//   Crystal– low points, large grow
// ─────────────────────────────────────────────
class PowerUp
{
public:
    enum class Type { Star = 0, Berry = 1, Crystal = 2 };

    PowerUp() = default;
    PowerUp(const QPoint& pos, Type t);

    QPoint position()  const { return m_pos;  }
    Type   type()      const { return m_type; }
    int    points()    const;
    int    growAmount()const;

    QVariantMap toVariant() const;

private:
    QPoint m_pos;
    Type   m_type = Type::Berry;
};

// ─────────────────────────────────────────────
inline PowerUp::PowerUp(const QPoint& pos, Type t)
    : m_pos(pos), m_type(t) {}

inline int PowerUp::points() const
{
    switch (m_type) {
    case Type::Star:    return 30;
    case Type::Berry:   return 15;
    case Type::Crystal: return  8;
    }
    return 10;
}

inline int PowerUp::growAmount() const
{
    switch (m_type) {
    case Type::Star:    return 1;
    case Type::Berry:   return 2;
    case Type::Crystal: return 4;
    }
    return 1;
}

inline QVariantMap PowerUp::toVariant() const
{
    QVariantMap m;
    m["x"]    = m_pos.x();
    m["y"]    = m_pos.y();
    m["type"] = static_cast<int>(m_type);
    return m;
}
