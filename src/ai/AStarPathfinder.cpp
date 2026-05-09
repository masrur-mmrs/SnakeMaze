#include "AStarPathfinder.h"

#include <QMap>
#include <algorithm>
#include <cmath>
#include <limits>

// ─────────────────────────────────────────────
int AStarPathfinder::heuristic(const QPoint& a, const QPoint& b)
{
    return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
}

// ─────────────────────────────────────────────
bool AStarPathfinder::isPassable(
    const QVector<QVector<int>>& grid,
    const QPoint& p, int cols, int rows,
    const QSet<int>& blocked)
{
    if (p.x() < 0 || p.x() >= cols || p.y() < 0 || p.y() >= rows)
        return false;
    if (grid[p.y()][p.x()] == 1)
        return false;
    return !blocked.contains(encode(p));
}

// ─────────────────────────────────────────────
//  floodFill
//  BFS from `origin`, counting all reachable cells not
//  in `blocked` and not walls. Capped at 200 to keep
//  it fast -- we only need relative comparisons.
// ─────────────────────────────────────────────
int AStarPathfinder::floodFill(
    const QPoint& origin,
    const QVector<QVector<int>>& grid,
    int cols, int rows,
    const QSet<int>& blocked)
{
    static const int DX[] = {0, 1, 0, -1};
    static const int DY[] = {-1, 0, 1, 0};

    QSet<int>       visited;
    QVector<QPoint> queue;
    queue.reserve(64);
    queue.append(origin);
    visited.insert(encode(origin));

    int count = 0;
    const int CAP = 200;

    for (int qi = 0; qi < queue.size() && count < CAP; ++qi) {
        QPoint cur = queue[qi];
        ++count;

        for (int d = 0; d < 4; ++d) {
            QPoint nb(cur.x() + DX[d], cur.y() + DY[d]);
            int nk = encode(nb);
            if (visited.contains(nk)) continue;
            if (!isPassable(grid, nb, cols, rows, blocked)) continue;
            visited.insert(nk);
            queue.append(nb);
        }
    }

    return count;
}

// ─────────────────────────────────────────────
//  nextStep  --  body-aware A* with safe-space tiebreaking
//
//  Algorithm:
//  1. Build a `blocked` set from snakeBody (excluding the
//     tail, which will vacate next tick -- so it's safe to
//     move into).
//  2. Run A* on the effective grid (walls + blocked body).
//  3. Reconstruct the first step of the found path.
//  4. If no path exists, fall back to the safest open
//     neighbour ranked by flood-fill reachable space.
// ─────────────────────────────────────────────
QPoint AStarPathfinder::nextStep(
    const QPoint& start,
    const QPoint& goal,
    const QVector<QVector<int>>& grid,
    int cols, int rows,
    const QVector<QPoint>& snakeBody)
{
    if (start == goal) return goal;

    // -- 1. Build blocked set (body minus tail) ---------------
    QSet<int> blocked;
    int bodySize = snakeBody.size();
    // Tail (last element) vacates next tick, so we skip it.
    for (int i = 0; i < bodySize - 1; ++i)
        blocked.insert(encode(snakeBody[i]));

    // -- 2. A* ------------------------------------------------
    QMap<int, Node>   openMap;
    QSet<int>         closedSet;
    QMap<int, QPoint> cameFrom;

    Node startNode;
    startNode.pos = start;
    startNode.g   = 0;
    startNode.h   = heuristic(start, goal);
    openMap[encode(start)] = startNode;

    static const int DX[] = {0, 1, 0, -1};
    static const int DY[] = {-1, 0, 1, 0};

    bool found = false;

    while (!openMap.isEmpty() && !found) {
        // Pop node with lowest f
        int bestKey = -1;
        int bestF   = std::numeric_limits<int>::max();
        for (auto it = openMap.cbegin(); it != openMap.cend(); ++it) {
            int f = it->f();
            if (f < bestF) { bestF = f; bestKey = it.key(); }
        }

        Node current = openMap.take(bestKey);
        closedSet.insert(encode(current.pos));

        if (current.pos == goal) { found = true; break; }

        for (int d = 0; d < 4; ++d) {
            QPoint nb(current.pos.x() + DX[d], current.pos.y() + DY[d]);
            int    nk = encode(nb);

            if (closedSet.contains(nk)) continue;
            if (!isPassable(grid, nb, cols, rows, blocked)) continue;

            int tentativeG = current.g + 1;
            if (openMap.contains(nk) && openMap[nk].g <= tentativeG)
                continue;

            Node nbNode;
            nbNode.pos = nb;
            nbNode.g   = tentativeG;
            nbNode.h   = heuristic(nb, goal);

            cameFrom[nk] = current.pos;
            openMap[nk]  = nbNode;
        }
    }

    // -- 3. Reconstruct first step ----------------------------
    if (found) {
        QPoint step = goal;
        while (cameFrom.contains(encode(step))) {
            QPoint parent = cameFrom[encode(step)];
            if (parent == start) return step;
            step = parent;
        }
        return step;
    }

    // -- 4. Fallback: safest open neighbour by flood-fill -----
    //    When A* can't reach the goal (temporarily blocked by
    //    body), pick the neighbour with the most reachable
    //    open space so the snake keeps moving and doesn't freeze.
    int    bestSpace = -1;
    QPoint bestNb   { -1, -1 };

    for (int d = 0; d < 4; ++d) {
        QPoint nb(start.x() + DX[d], start.y() + DY[d]);
        if (!isPassable(grid, nb, cols, rows, blocked)) continue;

        QSet<int> fbBlocked = blocked;
        fbBlocked.insert(encode(start));   // head will move away

        int space = floodFill(nb, grid, cols, rows, fbBlocked);
        if (space > bestSpace) {
            bestSpace = space;
            bestNb    = nb;
        }
    }

    return bestNb;   // QPoint(-1,-1) only if truly boxed in
}