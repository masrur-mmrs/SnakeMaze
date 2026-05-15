#pragma once

#include <QVector>
#include <QPoint>
#include <QSet>

class AStarPathfinder
{
public:
    static QPoint nextStep(
        const QPoint&               start,
        const QPoint&               goal,
        const QVector<QVector<int>>& grid,
        int                          cols,
        int                          rows,
        const QVector<QPoint>&       snakeBody = {}
    );

    static int floodFill(
        const QPoint&               origin,
        const QVector<QVector<int>>& grid,
        int                          cols,
        int                          rows,
        const QSet<int>&             blocked
    );

private:
    struct Node {
        QPoint pos;
        int    g     = 0;
        int    h     = 0;
        int    f()   const { return g + h; }
    };

    static int  heuristic(const QPoint& a, const QPoint& b);
    static bool isPassable(const QVector<QVector<int>>& grid,
                           const QPoint& p, int cols, int rows,
                           const QSet<int>& blocked);
    static int  encode(const QPoint& p) { return p.x() * 1000 + p.y(); }
};