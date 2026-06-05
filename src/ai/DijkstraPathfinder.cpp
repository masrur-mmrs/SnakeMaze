#include "DijkstraPathfinder.h"

#include <QMap>
#include <limits>

static const int DJK_DX[] = {0, 1, 0, -1};
static const int DJK_DY[] = {-1, 0, 1, 0};

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

QPoint DijkstraPathfinder::nextStep(
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
    startNode.pos  = start;
    startNode.cost = 0.0;
    openMap[encode(start)] = startNode;

    bool found = false;

    while (!openMap.isEmpty() && !found) {

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

            double newCost = current.cost + edgeCost(grid, nb, cols, rows);

            if (openMap.contains(nk) && openMap[nk].cost <= newCost)
                continue;

            Node nbNode;
            nbNode.pos  = nb;
            nbNode.cost = newCost;

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

    int    bestWalls = 999;
    QPoint bestNb   { -1, -1 };

    for (int d = 0; d < 4; ++d) {
        QPoint nb(start.x() + DJK_DX[d], start.y() + DJK_DY[d]);
        if (!isPassable(grid, nb, cols, rows, blocked)) continue;

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

    return bestNb;
}