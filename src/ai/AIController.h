#pragma once

#include <QVector>
#include <QPoint>
#include <QSet>

class Snake;
class PowerUp;

class AIController
{
public:
    AIController() = default;

    void update(
        int difficulty,
        Snake* cpuSnake,
        Snake* playerSnake,
        const QVector<QVector<int>>& grid,
        const QPoint& goal,
        const QVector<PowerUp>& powerUps,
        int cols,
        int rows
    );

private:
    void easyUpdate  (Snake* cpu, const QVector<QVector<int>>& grid,
                      int cols, int rows);

    void mediumUpdate(Snake* cpu, const QVector<QVector<int>>& grid,
                      const QPoint& goal, int cols, int rows);

    void hardUpdate  (Snake* cpu, const QVector<QVector<int>>& grid,
                      const QPoint& goal,
                      const QVector<PowerUp>& powerUps,
                      int cols, int rows);

    void survivalMove(Snake* cpu, const QVector<QVector<int>>& grid,
                      int cols, int rows);

    static bool isPassable(const QVector<QVector<int>>& grid,
                            const QPoint& p, int cols, int rows);

    static bool isPassableWithBody(const QVector<QVector<int>>& grid,
                                   const QPoint& p, int cols, int rows,
                                   const QSet<int>& blocked);

    static int  toDirection(const QPoint& from, const QPoint& to);
    static int  encode(const QPoint& p) { return p.x() * 1000 + p.y(); }

    int m_ticksSinceDirectionChange = 0;
};