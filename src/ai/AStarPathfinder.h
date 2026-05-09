#pragma once

#include <QVector>
#include <QPoint>
#include <QSet>

// ─────────────────────────────────────────────
//  AStarPathfinder
//  A* implementation for grid-based maze navigation.
//
//  Key improvements over naive A*:
//   • Snake body cells are treated as temporary walls
//   • flood-fill reachability check prevents committing
//     to paths that lead into sealed-off pockets
//   • Safe-space scoring breaks ties toward moves that
//     leave more open space ahead
// ─────────────────────────────────────────────
class AStarPathfinder
{
public:
    // Returns the immediate next cell to move to from `start`
    // heading toward `goal` through the given `grid`.
    //
    // grid[y][x]: 0/2 = passable, 1 = wall.
    // snakeBody:  cells occupied by the snake (treated as walls,
    //             except the tail which will vacate next tick).
    //
    // Returns QPoint(-1,-1) if no path exists.
    static QPoint nextStep(
        const QPoint&               start,
        const QPoint&               goal,
        const QVector<QVector<int>>& grid,
        int                          cols,
        int                          rows,
        const QVector<QPoint>&       snakeBody = {}
    );

    // Flood-fill: counts reachable open cells from `origin`.
    // Used to score candidate moves by available space.
    static int floodFill(
        const QPoint&               origin,
        const QVector<QVector<int>>& grid,
        int                          cols,
        int                          rows,
        const QSet<int>&             blocked   // encoded as x*1000+y
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