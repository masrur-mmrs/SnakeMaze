#pragma once

#include <QVector>
#include <QPoint>
#include <QSet>

class DijkstraPathfinder
{
public:
    static QPoint nextStep(
        const QPoint&                start,
        const QPoint&                goal,
        const QVector<QVector<int>>& grid,
        int                          cols,
        int                          rows,
        const QVector<QPoint>&       snakeBody = {}
    );

private:
    struct Node {
        QPoint pos;
        double cost = 0.0;
    };

    static double edgeCost(
        const QVector<QVector<int>>& grid,
        const QPoint&                p,
        int cols, int rows
    );

    static bool isPassable(
        const QVector<QVector<int>>& grid,
        const QPoint&                p,
        int cols, int rows,
        const QSet<int>&             blocked
    );

    static int encode(const QPoint& p) { return p.x() * 1000 + p.y(); }
};