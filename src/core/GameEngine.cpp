#include "GameEngine.h"
#include "AIController.h"

#include <QDebug>
#include <QRandomGenerator>

GameEngine::GameEngine(QObject* parent)
    : QObject(parent)
    , m_state(std::make_unique<GameState>(this))
    , m_mazeGen(std::make_unique<MazeGenerator>(COLS, ROWS))
    , m_playerSnake(std::make_unique<Snake>(true))
    , m_cpuSnake(std::make_unique<Snake>(false))
    , m_aiController(std::make_unique<AIController>())
{
    connect(&m_tickTimer, &QTimer::timeout, this, &GameEngine::onTick);
}

GameEngine::~GameEngine() = default;

void GameEngine::startGame(int difficulty)
{
    m_difficulty = difficulty;

    switch (difficulty) {
    case 0: m_tickRateMs = 200; break;
    case 1: m_tickRateMs = 150; break;
    case 2: m_tickRateMs = 120; break;
    default: m_tickRateMs = 150;
    }
    emit tickRateChanged();

    initLevel();
    m_tickTimer.start(m_tickRateMs);
}

void GameEngine::resetGame()
{
    m_tickTimer.stop();
    m_paused = false;
    emit pauseChanged();
    startGame(m_difficulty);
}

void GameEngine::pauseGame()
{
    if (m_paused) return;
    m_paused = true;
    m_tickTimer.stop();
    emit pauseChanged();
}

void GameEngine::resumeGame()
{
    if (!m_paused) return;
    m_paused = false;
    m_tickTimer.start(m_tickRateMs);
    emit pauseChanged();
}

void GameEngine::setPlayerDirection(int direction)
{
    if (m_playerSnake)
        m_playerSnake->setDesiredDirection(static_cast<Snake::Direction>(direction));
}

void GameEngine::initLevel()
{
    m_mazeGen->generate();
    m_state->setMaze(m_mazeGen->grid());
    m_goalPos = m_mazeGen->goalPos();


    m_playerSnake->reset(QPoint(1, 1), Snake::Direction::Right);

    m_cpuSnake->reset(QPoint(COLS - 2, ROWS - 2), Snake::Direction::Left);

    m_state->resetScores();

    m_powerUps.clear();
    spawnPowerUps();

    m_state->setPlayerSnake(m_playerSnake.get());
    m_state->setCpuSnake(m_cpuSnake.get());
    m_state->setPowerUps(m_powerUps);

    emit gameStateChanged();
}

void GameEngine::spawnPowerUps()
{
    if (m_powerUps.size() >= MAX_POWERUPS) return;

    const auto& grid = m_mazeGen->grid();
    int needed   = MAX_POWERUPS - m_powerUps.size();
    int attempts = 0;

    QSet<QPair<int,int>> occupied;
    for (const PowerUp& p : m_powerUps)
        occupied.insert({p.position().x(), p.position().y()});

    while (needed > 0 && attempts < 400) {
        ++attempts;
        int x = QRandomGenerator::global()->bounded(1, COLS - 1);
        int y = QRandomGenerator::global()->bounded(1, ROWS - 1);

        if (grid[y][x] != 0)                         continue;
        if (QPoint(x, y) == m_goalPos)                continue;
        if (QPoint(x, y) == QPoint(1, 1))             continue;
        if (QPoint(x, y) == QPoint(COLS-2, ROWS-2))   continue;
        if (occupied.contains({x, y}))                continue;

        PowerUp::Type t = static_cast<PowerUp::Type>(
            QRandomGenerator::global()->bounded(0, 3));
        m_powerUps.append(PowerUp(QPoint(x, y), t));
        occupied.insert({x, y});
        --needed;
    }
}

void GameEngine::onTick()
{
    m_aiController->update(
        m_difficulty,
        m_cpuSnake.get(),
        m_playerSnake.get(),
        m_mazeGen->grid(),
        m_goalPos,
        m_powerUps,
        COLS, ROWS
    );

    moveSnakes();
    checkCollisions();
    checkGoal();

    m_state->notifyUpdates();
    m_state->setPowerUps(m_powerUps);
    emit gameStateChanged();
    emit tick();
}

void GameEngine::moveSnakes()
{
    m_playerSnake->move(m_mazeGen->grid(), COLS, ROWS);
    m_cpuSnake->move(m_mazeGen->grid(), COLS, ROWS);
}

void GameEngine::checkCollisions()
{
    const QPoint playerHead = m_playerSnake->head();
    const QPoint cpuHead    = m_cpuSnake->head();

    for (int i = m_powerUps.size() - 1; i >= 0; --i) {
        const QPoint& pos = m_powerUps[i].position();

        if (pos == playerHead) {
            int pts = m_powerUps[i].points();
            m_state->addPlayerScore(pts);
            m_playerSnake->grow(m_powerUps[i].growAmount());
            emit powerUpCollected(pos.x(), pos.y(), static_cast<int>(m_powerUps[i].type()));
            emit snakeGrew(true);
            m_powerUps.removeAt(i);
            spawnPowerUps();
            m_state->setPowerUps(m_powerUps);
            continue;
        }

        if (pos == cpuHead) {
            int pts = m_powerUps[i].points();
            m_state->addCpuScore(pts);
            m_cpuSnake->grow(m_powerUps[i].growAmount());
            emit powerUpCollected(pos.x(), pos.y(), static_cast<int>(m_powerUps[i].type()));
            emit snakeGrew(false);
            m_powerUps.removeAt(i);
            spawnPowerUps();
            m_state->setPowerUps(m_powerUps);
        }
    }
}

void GameEngine::checkGoal()
{
    bool playerAtGoal = (m_playerSnake->head() == m_goalPos);
    bool cpuAtGoal    = (m_cpuSnake->head()    == m_goalPos);

    if (playerAtGoal || cpuAtGoal) {
        QPoint goal = m_mazeGen->goalPos();

        bool playerAtGoal = (m_playerSnake->head() == goal);
        bool cpuAtGoal    = (m_cpuSnake->head()    == goal);

        if (!playerAtGoal && !cpuAtGoal) return;

        m_tickTimer.stop();

        int  ps = m_state->playerScore();
        int  cs = m_state->cpuScore();
        bool playerWon;

        if (playerAtGoal && cpuAtGoal) {
            playerWon = (ps >= cs);
        } else if (playerAtGoal) {
            playerWon = true;
        } else {
            playerWon = false;
        }

        emit gameOver(playerWon, ps, cs);
    }
}
