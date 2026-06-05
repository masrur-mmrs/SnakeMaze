#include "DijkstraPathfinder.h"

#include <QMap>
#include <limits>

// Direction offsets — order matches Snake::Direction enum (Up/Right/Down/Left)
static const int DJK_DX[] = {0, 1, 0, -1};
static const int DJK_DY[] = {-1, 0, 1, 0};

// ─────────────────────────────────────────────────────────────
//  edgeCost
//
//  Base cost to enter cell p is 1.0.
//  Each adjacent wall or boundary adds +0.25.
//
//  Result range:
//    0 walls adjacent  →  1.00  (open intersection)
//    1 wall adjacent   →  1.25  (corridor edge)
//    2 walls adjacent  →  1.50  (corner or straight corridor)
//    3 walls adjacent  →  1.75  (dead-end approach)
//
//  This means Dijkstra routes the medium CPU through more open
//  areas rather than hugging walls, which looks more natural
//  and avoids it painting itself into dead-end corners.
// ─────────────────────────────────────────────────────────────
double DijkstraPathfinder::edgeCost(
    const QVector<QVector<int>>& grid,
    const QPoint& p,
    int cols, int rows)
{
    double cost = 1.0;

    for (int d = 0; d < 4; ++d) {
        int nx = p.x() + DJK_DX[d];
        int ny = p.y() + DJK_DY[d];
        bool isWallOrBoundary =
            (nx < 0 || nx >= cols || ny < 0 || ny >= rows)
            || grid[ny][nx] == 1;
        if (isWallOrBoundary)
            cost += 0.25;
    }

    return cost;
}

// ─────────────────────────────────────────────────────────────
bool DijkstraPathfinder::isPassable(
    const QVector<QVector<int>>& grid,
    const QPoint& p,
    int cols, int rows,
    const QSet<int>& blocked)
{
    if (p.x() < 0 || p.x() >= cols || p.y() < 0 || p.y() >= rows)
        return false;
    if (grid[p.y()][p.x()] == 1)
        return false;
    return !blocked.contains(encode(p));
}

// ─────────────────────────────────────────────────────────────
//  nextStep  —  Dijkstra with wall-proximity weights
//
//  Algorithm:
//  1. Build blocked set from snakeBody (tail excluded — vacates next tick).
//  2. Run Dijkstra: expand lowest-cost node each iteration;
//     edge costs are edgeCost() values so open corridors are cheaper.
//  3. Reconstruct path; return the first step from start.
//  4. Fallback: if no path found (body temporarily seals all routes),
//     return the open neighbour with the fewest adjacent walls
//     (most open space), so the snake keeps moving instead of freezing.
// ─────────────────────────────────────────────────────────────
QPoint DijkstraPathfinder::nextStep(
    const QPoint& start,
    const QPoint& goal,
    const QVector<QVector<int>>& grid,
    int cols, int rows,
    const QVector<QPoint>& snakeBody)
{
    if (start == goal) return goal;

    // ── 1. Build blocked set (body minus tail) ───────────────
    QSet<int> blocked;
    int bodySize = snakeBody.size();
    for (int i = 0; i < bodySize - 1; ++i)       // tail at index bodySize-1 is excluded
        blocked.insert(encode(snakeBody[i]));

    // ── 2. Dijkstra ──────────────────────────────────────────
    // openMap  : key = encoded position, value = Node with best cost so far
    // closedSet: positions fully settled (optimal cost confirmed)
    // cameFrom : child → parent for path reconstruction
    QMap<int, Node>   openMap;
    QSet<int>         closedSet;
    QMap<int, QPoint> cameFrom;

    Node startNode;
    startNode.pos  = start;
    startNode.cost = 0.0;
    openMap[encode(start)] = startNode;

    bool found = false;

    while (!openMap.isEmpty() && !found) {

        // Pop node with lowest accumulated cost
        // (In A* this is lowest f = g+h; here it's just g — no heuristic)
        int    bestKey  = -1;
        double bestCost = std::numeric_limits<double>::max();

        for (auto it = openMap.cbegin(); it != openMap.cend(); ++it) {
            if (it->cost < bestCost) {
                bestCost = it->cost;
                bestKey  = it.key();
            }
        }

        Node current = openMap.take(bestKey);
        int  curKey  = encode(current.pos);
        closedSet.insert(curKey);

        if (current.pos == goal) { found = true; break; }

        for (int d = 0; d < 4; ++d) {
            QPoint nb(current.pos.x() + DJK_DX[d],
                      current.pos.y() + DJK_DY[d]);
            int nk = encode(nb);

            if (closedSet.contains(nk)) continue;
            if (!isPassable(grid, nb, cols, rows, blocked)) continue;

            // Dijkstra cost: sum of real edge costs along the path.
            // edgeCost() penalises wall-adjacent cells so the algorithm
            // naturally prefers open corridors.
            double newCost = current.cost + edgeCost(grid, nb, cols, rows);

            if (openMap.contains(nk) && openMap[nk].cost <= newCost)
                continue;   // already have an equal-or-better route here

            Node nbNode;
            nbNode.pos  = nb;
            nbNode.cost = newCost;

            cameFrom[nk] = current.pos;
            openMap[nk]  = nbNode;
        }
    }

    // ── 3. Reconstruct first step ────────────────────────────
    if (found) {
        QPoint step = goal;
        while (cameFrom.contains(encode(step))) {
            QPoint parent = cameFrom[encode(step)];
            if (parent == start) return step;   // step is the first move
            step = parent;
        }
        return step;
    }

    // ── 4. Fallback: open neighbour with fewest wall neighbours ─
    //    No path to goal (body temporarily blocks everything).
    //    Move to the least-walled open neighbour to keep moving
    //    and give the body time to clear.
    int    bestWalls = 999;
    QPoint bestNb   { -1, -1 };

    for (int d = 0; d < 4; ++d) {
        QPoint nb(start.x() + DJK_DX[d], start.y() + DJK_DY[d]);
        if (!isPassable(grid, nb, cols, rows, blocked)) continue;

        // Count wall neighbours as a proxy for openness
        int walls = 0;
        for (int d2 = 0; d2 < 4; ++d2) {
            int nx = nb.x() + DJK_DX[d2];
            int ny = nb.y() + DJK_DY[d2];
            bool isWall = (nx < 0 || nx >= cols || ny < 0 || ny >= rows)
                          || grid[ny][nx] == 1;
            if (isWall) ++walls;
        }

        if (walls < bestWalls) {
            bestWalls = walls;
            bestNb    = nb;
        }
    }

    return bestNb;   // QPoint(-1,-1) only if completely surrounded
}