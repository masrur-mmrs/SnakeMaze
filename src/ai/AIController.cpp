#include "AIController.h"
#include "AStarPathfinder.h"
#include "../core/Snake.h"
#include "../core/PowerUp.h"

#include <QRandomGenerator>
#include <QSet>
#include <limits>

static const int DX[] = {0, 1, 0, -1};
static const int DY[] = {-1, 0, 1, 0};

// ─────────────────────────────────────────────
void AIController::update(
    int difficulty,
    Snake* cpuSnake,
    Snake* playerSnake,
    const QVector<QVector<int>>& grid,
    const QPoint& goal,
    const QVector<PowerUp>& powerUps,
    int cols, int rows)
{
    if (!cpuSnake || !cpuSnake->isAlive()) return;

    ++m_ticksSinceDirectionChange;

    switch (difficulty) {
    case 0: easyUpdate  (cpuSnake, grid, cols, rows);                  break;
    case 1: mediumUpdate(cpuSnake, grid, goal, cols, rows);            break;
    case 2: hardUpdate  (cpuSnake, grid, goal, powerUps, cols, rows);  break;
    default: easyUpdate (cpuSnake, grid, cols, rows);
    }
}

// ─────────────────────────────────────────────
//  EASY – Random walk with wall avoidance
// ─────────────────────────────────────────────
void AIController::easyUpdate(Snake* cpu,
                               const QVector<QVector<int>>& grid,
                               int cols, int rows)
{
    QPoint head = cpu->head();

    QVector<int> valid;
    for (int d = 0; d < 4; ++d) {
        QPoint next(head.x() + DX[d], head.y() + DY[d]);
        if (isPassable(grid, next, cols, rows))
            valid.append(d);
    }
    if (valid.isEmpty()) return;

    int cur = static_cast<int>(cpu->direction());
    bool keepCurrent = valid.contains(cur)
                    && (m_ticksSinceDirectionChange < 5)
                    && (QRandomGenerator::global()->bounded(100) < 70);
    if (keepCurrent) return;

    int chosen = valid[QRandomGenerator::global()->bounded(valid.size())];
    cpu->setDesiredDirection(static_cast<Snake::Direction>(chosen));
    m_ticksSinceDirectionChange = 0;
}

// ─────────────────────────────────────────────
//  MEDIUM – Greedy Manhattan toward goal
// ─────────────────────────────────────────────
void AIController::mediumUpdate(Snake* cpu,
                                 const QVector<QVector<int>>& grid,
                                 const QPoint& goal,
                                 int cols, int rows)
{
    QPoint head = cpu->head();

    int bestDir  = -1;
    int bestDist = std::numeric_limits<int>::max();

    for (int d = 0; d < 4; ++d) {
        QPoint next(head.x() + DX[d], head.y() + DY[d]);
        if (!isPassable(grid, next, cols, rows)) continue;
        int dist = std::abs(next.x()-goal.x()) + std::abs(next.y()-goal.y());
        if (dist < bestDist) { bestDist = dist; bestDir = d; }
    }

    if (bestDir >= 0)
        cpu->setDesiredDirection(static_cast<Snake::Direction>(bestDir));
}

// ─────────────────────────────────────────────
//  HARD – Body-aware A* with dead-end avoidance
// ─────────────────────────────────────────────
void AIController::hardUpdate(Snake* cpu,
                               const QVector<QVector<int>>& grid,
                               const QPoint& goal,
                               const QVector<PowerUp>& powerUps,
                               int cols, int rows)
{
    QPoint head = cpu->head();
    const QVector<QPoint>& body = cpu->body();

    // Build blocked set for body (tail excluded -- it vacates next tick)
    QSet<int> bodyBlocked;
    for (int i = 0; i < body.size() - 1; ++i)
        bodyBlocked.insert(encode(body[i]));

    // ── 1. Choose target: goal or nearby star power-up ──────
    QPoint target = goal;

    // Measure A* distance to goal (with body awareness)
    // We use a lightweight estimate here: just Manhattan with body check.
    // Real A* is run below for the actual move.
    int goalManhattan = std::abs(head.x()-goal.x()) + std::abs(head.y()-goal.y());

    for (const PowerUp& pu : powerUps) {
        if (pu.type() != PowerUp::Type::Star) continue;
        QPoint ppos = pu.position();

        // Skip if power-up is inside our own body region
        if (bodyBlocked.contains(encode(ppos))) continue;

        int puManhattan = std::abs(head.x()-ppos.x()) + std::abs(head.y()-ppos.y());

        // Only detour if power-up is close AND it doesn't take us far from goal
        int detourCost = puManhattan
                       + std::abs(ppos.x()-goal.x())
                       + std::abs(ppos.y()-goal.y());

        if (puManhattan <= 5 && detourCost < goalManhattan * 1.4) {
            target = ppos;
            break;
        }
    }

    // ── 2. Run body-aware A* toward target ──────────────────
    QPoint nextCell = AStarPathfinder::nextStep(head, target, grid, cols, rows, body);

    // ── 3. Dead-end safety check ────────────────────────────
    //    After choosing the A* step, verify it doesn't lead
    //    into a pocket with too little space (< half our body length).
    //    If it does, prefer the neighbour with the most open space.
    if (nextCell.x() >= 0) {
        QSet<int> afterMove = bodyBlocked;
        afterMove.insert(encode(head));  // head becomes new body cell

        int pathSpace = AStarPathfinder::floodFill(
            nextCell, grid, cols, rows, afterMove);

        int minSafeSpace = qMax(4, body.size() / 2);

        if (pathSpace < minSafeSpace) {
            // A* path is unsafe -- switch to survival mode
            survivalMove(cpu, grid, cols, rows);
            return;
        }
    }

    // ── 4. Apply direction or fall back to survival ──────────
    if (nextCell.x() >= 0) {
        int dir = toDirection(head, nextCell);
        if (dir >= 0) {
            cpu->setDesiredDirection(static_cast<Snake::Direction>(dir));
            return;
        }
    }

    // A* returned nothing valid -- survive
    survivalMove(cpu, grid, cols, rows);
}

// ─────────────────────────────────────────────
//  survivalMove
//  When no safe path exists, pick the open neighbour
//  with the largest flood-fill reachable area.
//  This keeps the snake moving and avoids self-trapping.
// ─────────────────────────────────────────────
void AIController::survivalMove(Snake* cpu,
                                 const QVector<QVector<int>>& grid,
                                 int cols, int rows)
{
    QPoint head = cpu->head();
    const QVector<QPoint>& body = cpu->body();

    QSet<int> bodyBlocked;
    for (int i = 0; i < body.size() - 1; ++i)
        bodyBlocked.insert(encode(body[i]));

    int    bestSpace = -1;
    QPoint bestCell { -1, -1 };

    for (int d = 0; d < 4; ++d) {
        QPoint nb(head.x() + DX[d], head.y() + DY[d]);
        if (!isPassableWithBody(grid, nb, cols, rows, bodyBlocked)) continue;

        QSet<int> fbBlocked = bodyBlocked;
        fbBlocked.insert(encode(head));

        int space = AStarPathfinder::floodFill(nb, grid, cols, rows, fbBlocked);
        if (space > bestSpace) {
            bestSpace = space;
            bestCell  = nb;
        }
    }

    if (bestCell.x() >= 0) {
        int dir = toDirection(head, bestCell);
        if (dir >= 0)
            cpu->setDesiredDirection(static_cast<Snake::Direction>(dir));
    }
}

// ─────────────────────────────────────────────
bool AIController::isPassable(const QVector<QVector<int>>& grid,
                               const QPoint& p, int cols, int rows)
{
    if (p.x() < 0 || p.x() >= cols || p.y() < 0 || p.y() >= rows)
        return false;
    return grid[p.y()][p.x()] != 1;
}

bool AIController::isPassableWithBody(const QVector<QVector<int>>& grid,
                                       const QPoint& p, int cols, int rows,
                                       const QSet<int>& blocked)
{
    if (!isPassable(grid, p, cols, rows)) return false;
    return !blocked.contains(encode(p));
}

// ─────────────────────────────────────────────
int AIController::toDirection(const QPoint& from, const QPoint& to)
{
    QPoint d = to - from;
    if (d == QPoint( 0, -1)) return 0;  // Up
    if (d == QPoint( 1,  0)) return 1;  // Right
    if (d == QPoint( 0,  1)) return 2;  // Down
    if (d == QPoint(-1,  0)) return 3;  // Left
    return -1;
}