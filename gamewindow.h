#pragma once
#include <QGraphicsView>
#include "pacman.h"
#include "ghost.h"

class GameWindow : public QGraphicsView {
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);
    void updateScore(int newScore);
    bool startGame(const QString& mazeFile = "maze2.txt");

private:
    QGraphicsScene* m_scene;
    Pacman* m_pacman;
    QList<Ghost*> m_ghosts;
    QTimer* m_collisionTimer;
    QTimer* m_scoreTimer;  // Added for score updates
    QGraphicsTextItem* m_scoreText;  // Score display
    int m_score;  // Current score
};
