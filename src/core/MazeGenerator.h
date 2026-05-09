#pragma once

#include <QObject>
#include <QVector>
#include <QPoint>

// ─────────────────────────────────────────────
//  MazeGenerator
//  Generates a perfect maze using Recursive
//  Division. Grid values:
//    0 = open path
//    1 = wall
//    2 = goal (center cell)
// ─────────────────────────────────────────────
class MazeGenerator : public QObject
{
    Q_OBJECT
public:
    using Grid = QVector<QVector<int>>;

    explicit MazeGenerator(int cols, int rows, QObject* parent = nullptr);

    void         generate();
    const Grid&  grid() const { return m_grid; }
    QPoint       goalPos() const;

private:
    void initGrid();
    void carvePath(int x, int y);                // Recursive backtracking
    void placeGoal();

    static int dx(int dir);
    static int dy(int dir);

    Grid m_grid;
    int  m_cols;
    int  m_rows;
};
