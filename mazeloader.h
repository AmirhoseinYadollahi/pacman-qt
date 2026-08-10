#pragma once
#include <QString>
#include <QList>
#include <QGraphicsScene>
#include "pacman.h"
#include "ghost.h"

class MazeLoader {
public:
    static constexpr int MAX_SIZE = 19;
    MazeLoader(const QString &filename, int cellSize);
    // Reads the maze file and places items (walls, Pac-Man, ghosts) into the scene.
    // Returns true on success. The pointer for Pac-Man (if created) is returned via pacman,
    // and ghost items are appended to the ghosts list.
    bool loadMaze(QGraphicsScene *scene, Pacman *&pacman, QList<Ghost*>& ghosts);

private:
    QString m_filename;
    int m_cellSize;
    // Returns a QColor based on the token.
    QColor getColorForToken(const QString &token);
};
