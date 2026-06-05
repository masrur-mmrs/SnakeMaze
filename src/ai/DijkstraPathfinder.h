#pragma once

#include <QVector>
#include <QPoint>
#include <QSet>

// ─────────────────────────────────────────────────────────────
//  DijkstraPathfinder
//
//  Finds the lowest-cost path from start to goal using
//  Dijkstra's algorithm with wall-proximity edge weights.
//
//  KEY DIFFERENCE FROM A*:
//    A* uses a heuristic (Manhattan distance) to guide search
//    toward the goal quickly.  Dijkstra has no heuristic — it
//    expands nodes purely by accumulated real cost, so it
//    explores more of the maze before committing to a route.
//
//  WALL-PROXIMITY WEIGHTING:
//    Each passable cell has a base cost of 1.0.  Every adjacent
//    wall adds +0.25, so dead-end cells cost up to 1.75.
//    This makes Dijkstra prefer open corridors over tight passages,
//    giving the medium CPU a different movement "feel" from the
//    hard CPU's heuristic-driven straight-line approach.
//
//  BODY AWARENESS:
//    Snake body cells (minus the tail that vacates next tick) are
//    treated as temporary walls — same technique as AStarPathfinder.
// ─────────────────────────────────────────────────────────────
class DijkstraPathfinder
{
public:
    // Returns the immediate next cell to move toward goal.
    // snakeBody: occupied cells treated as walls (tail excluded).
    // Returns QPoint(-1,-1) if no path exists.
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
        double cost = 0.0;   // accumulated real cost — no heuristic term
    };

    // Cost to enter cell `p`: base 1.0 + 0.25 per adjacent wall.
    // Penalises tight corridors and dead ends; rewards open space.
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