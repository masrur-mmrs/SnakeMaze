#pragma once

#include <QObject>
#include <QVector>
#include <QPoint>

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
    void carvePath(int x, int y);
    void placeGoal();

    static int dx(int dir);
    static int dy(int dir);

    Grid m_grid;
    int  m_cols;
    int  m_rows;
};
