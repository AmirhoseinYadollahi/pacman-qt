#include "gamewindow.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include "mazeloader.h"

GameWindow::GameWindow(QWidget *parent)
    : QGraphicsView(parent), m_scene(nullptr), m_pacman(nullptr), m_collisionTimer(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(820, 820);
    setWindowTitle("Pac-Man Maze Game");
    setFocusPolicy(Qt::StrongFocus);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setBackgroundBrush(Qt::black);
}

void GameWindow::updateScore(int newScore) {
    m_scoreText->setPlainText("Score: " + QString::number(newScore));
}

void GameWindow::startGame(const QString& mazeFile) {
    // Clean up previous game if any
    if (m_scene) {
        delete m_scene;
        m_scene = nullptr;
    }

    // Initialize scene
    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 800, 800);
    m_scene->setBackgroundBrush(QColor(0, 0, 80));
    setScene(m_scene);

    // Create score display
    m_score = 0;
    m_scoreText = new QGraphicsTextItem();
    m_scoreText->setDefaultTextColor(Qt::yellow);
    m_scoreText->setFont(QFont("Arial Black", 16, QFont::Bold));
    m_scoreText->setPlainText("Score: 0");
    m_scoreText->setPos(20, 20);  // Top-left position
    m_scoreText->setZValue(100);  // Ensure it's on top
    m_scene->addItem(m_scoreText);

    m_scoreTimer = new QTimer(this);
    connect(m_scoreTimer, &QTimer::timeout, [this](){
        m_score += 10;
        this->updateScore(m_score);
    });
    m_scoreTimer->start(1000);


    // Load maze
    const int cellSize = 40;
    MazeLoader loader(mazeFile, cellSize);
    if (!loader.loadMaze(m_scene, m_pacman, m_ghosts)) {
        QMessageBox::critical(this, "Error", "Failed to load maze!");
        return;
    }


    // Set up collision detection
    m_collisionTimer = new QTimer(this);
    connect(m_collisionTimer, &QTimer::timeout, [this]() {
        if (!m_pacman) return;

        for (QGraphicsItem* item : m_pacman->collidingItems()) {
            if (dynamic_cast<Ghost*>(item)) {
                // Stop score timer first
                if (m_scoreTimer) {
                    m_scoreTimer->stop();
                }

                QMessageBox::information(this, "Game Over",
                                         QString("Pac-Man was caught by a ghost!\nFinal Score: %1").arg(m_score));
                close();
                return;
            }
        }
    });
    m_collisionTimer->start(100);

    // Focus Pac-Man
    if (m_pacman) {
        m_pacman->setFocus();
        m_scene->setFocusItem(m_pacman);
    }

    // Center window
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    move((screenGeometry.width() - width()) / 2,
         (screenGeometry.height() - height()) / 2);
}
