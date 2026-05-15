#include "AStarPathfinder.h"

#include <QMap>
#include <algorithm>
#include <cmath>
#include <limits>

int AStarPathfinder::heuristic(const QPoint& a, const QPoint& b)
{
    return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
}

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

QPoint AStarPathfinder::nextStep(
    const QPoint& start,
    const QPoint& goal,
    const QVector<QVector<int>>& grid,
    int cols, int rows,
    const QVector<QPoint>& snakeBody)
{
    if (start == goal) return goal;

    QSet<int> blocked;
    int bodySize = snakeBody.size();
    for (int i = 0; i < bodySize - 1; ++i)
        blocked.insert(encode(snakeBody[i]));

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

    if (found) {
        QPoint step = goal;
        while (cameFrom.contains(encode(step))) {
            QPoint parent = cameFrom[encode(step)];
            if (parent == start) return step;
            step = parent;
        }
        return step;
    }

    int    bestSpace = -1;
    QPoint bestNb   { -1, -1 };

    for (int d = 0; d < 4; ++d) {
        QPoint nb(start.x() + DX[d], start.y() + DY[d]);
        if (!isPassable(grid, nb, cols, rows, blocked)) continue;

        QSet<int> fbBlocked = blocked;
        fbBlocked.insert(encode(start));

        int space = floodFill(nb, grid, cols, rows, fbBlocked);
        if (space > bestSpace) {
            bestSpace = space;
            bestNb    = nb;
        }
    }

    return bestNb;
}