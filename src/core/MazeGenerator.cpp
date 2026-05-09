#include "MazeGenerator.h"

#include <QRandomGenerator>
#include <algorithm>
#include <array>

MazeGenerator::MazeGenerator(int cols, int rows, QObject* parent)
    : QObject(parent)
    , m_cols(cols)
    , m_rows(rows)
{
    // Ensure odd dimensions for proper maze generation
    if (m_cols % 2 == 0) ++m_cols;
    if (m_rows % 2 == 0) ++m_rows;
}

// ─────────────────────────────────────────────
void MazeGenerator::generate()
{
    initGrid();
    carvePath(1, 1);
    placeGoal();
}

// ─────────────────────────────────────────────
void MazeGenerator::initGrid()
{
    m_grid.resize(m_rows);
    for (auto& row : m_grid) {
        row.resize(m_cols);
        row.fill(1);   // all walls
    }
}

// ─────────────────────────────────────────────
//  Recursive backtracking maze carver.
//  Operates on odd-coordinate cells only.
// ─────────────────────────────────────────────
void MazeGenerator::carvePath(int x, int y)
{
    m_grid[y][x] = 0;

    // Directions: 0=Up 1=Right 2=Down 3=Left
    std::array<int, 4> dirs = {0, 1, 2, 3};
    // Fisher-Yates shuffle using Qt's RNG
    for (int i = 3; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(dirs[i], dirs[j]);
    }

    for (int d : dirs) {
        int nx = x + dx(d) * 2;
        int ny = y + dy(d) * 2;

        if (nx >= 0 && nx < m_cols && ny >= 0 && ny < m_rows
            && m_grid[ny][nx] == 1) {
            // Knock down the wall between current and neighbor
            m_grid[y + dy(d)][x + dx(d)] = 0;
            carvePath(nx, ny);
        }
    }
}

// ─────────────────────────────────────────────
void MazeGenerator::placeGoal()
{
    QPoint g = goalPos();
    // Force center area open
    for (int dy = -1; dy <= 1; ++dy)
        for (int dx = -1; dx <= 1; ++dx)
            m_grid[g.y() + dy][g.x() + dx] = 0;

    m_grid[g.y()][g.x()] = 2;  // 2 = goal marker
}

// ─────────────────────────────────────────────
QPoint MazeGenerator::goalPos() const
{
    // Center, snapped to odd coordinate
    int cx = (m_cols / 2);
    int cy = (m_rows / 2);
    if (cx % 2 == 0) --cx;
    if (cy % 2 == 0) --cy;
    return { cx, cy };
}

// ─────────────────────────────────────────────
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
