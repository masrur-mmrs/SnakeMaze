#include "MazeGenerator.h"

#include <QRandomGenerator>
#include <algorithm>
#include <array>

MazeGenerator::MazeGenerator(int cols, int rows, QObject* parent)
    : QObject(parent)
    , m_cols(cols)
    , m_rows(rows)
{
    if (m_cols % 2 == 0) ++m_cols;
    if (m_rows % 2 == 0) ++m_rows;
}

void MazeGenerator::generate()
{
    initGrid();
    carvePath(1, 1);
    placeGoal();
}

void MazeGenerator::initGrid()
{
    m_grid.resize(m_rows);
    for (auto& row : m_grid) {
        row.resize(m_cols);
        row.fill(1);
    }
}

void MazeGenerator::carvePath(int x, int y)
{
    m_grid[y][x] = 0;

    std::array<int, 4> dirs = {0, 1, 2, 3};

    for (int i = 3; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(dirs[i], dirs[j]);
    }

    for (int d : dirs) {
        int nx = x + dx(d) * 2;
        int ny = y + dy(d) * 2;

        if (nx >= 0 && nx < m_cols && ny >= 0 && ny < m_rows
            && m_grid[ny][nx] == 1) {
            m_grid[y + dy(d)][x + dx(d)] = 0;
            carvePath(nx, ny);
        }
    }
}

void MazeGenerator::placeGoal()
{
    QPoint g = goalPos();
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            m_grid[g.y() + dy][g.x() + dx] = 0;

    m_grid[g.y()][g.x()] = 2;
}

QPoint MazeGenerator::goalPos() const
{
    int cx = (m_cols / 2);
    int cy = (m_rows / 2);
    if (cx % 2 == 0) --cx;
    if (cy % 2 == 0) --cy;
    return { cx, cy };
}

int MazeGenerator::dx(int dir)
{
    static const int table[] = {0, 1, 0, -1};
    return table[dir];
}

int MazeGenerator::dy(int dir)
{
    static const int table[] = {-1, 0, 1, 0};
    return table[dir];
}
