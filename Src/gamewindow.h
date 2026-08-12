#pragma once
#include <QGraphicsView>
#include "pacman.h"
#include "ghost.h"

class GameWindow : public QGraphicsView {
    Q_OBJECT
public:
    static constexpr int CELL_SIZE = 40;
    static constexpr int GRID_SIZE = 20;
    static constexpr int GAME_SIZE = CELL_SIZE * GRID_SIZE;

    explicit GameWindow(QWidget *parent = nullptr);
    void updateScore(int newScore);
    bool startGame(const QString& mazeFile = "maze2.txt");


private:
    void clearGame();
    void createScene();
    void createScoreDisplay();
    void handleScoreTimer(int scorePerSecond, int timerStep);
    void resizeEvent(QResizeEvent* event);

    QGraphicsScene* m_scene;
    Pacman* m_pacman;
    QList<Ghost*> m_ghosts;
    QTimer* m_collisionTimer;
    QTimer* m_scoreTimer;  // Added for score updates
    QGraphicsTextItem* m_scoreText;  // Score display
    int m_score;  // Current score
};
