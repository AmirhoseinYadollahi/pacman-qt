#include "mazeloader.h"
#include "wall.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

MazeLoader::MazeLoader(const QString &filename, int cellSize)
    : m_filename(filename), m_cellSize(cellSize) {
}

QColor MazeLoader::getColorForToken(const QString &token) {
    if (token == "X") return Qt::darkGray;
    if (token == "0") return QColor(0, 0, 139);
    if (token == "1") return Qt::blue;
    if (token == "2") return Qt::cyan;
    if (token == "3") return Qt::red;
    if (token == "4") return Qt::magenta;
    if (token == "5") return QColor(128, 0, 128);
    if (token == "6") return Qt::green;
    if (token == "7") return QColor(255, 165, 0);
    if (token == "8") return Qt::yellow;
    if (token == "9") return QColor(173, 216, 230);
    if (token == "|") return Qt::gray;
    if (token == "=") return Qt::lightGray;
    return Qt::blue;
}

bool MazeLoader::loadMaze(QGraphicsScene *scene, Pacman *&pacman, QList<Ghost*>& ghosts) {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open maze file:" << m_filename;
        return false;
    }

    QTextStream in(&file);

    int row = 0;
    int mazeWidth = -1;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);

        if (row >= MAX_SIZE || tokens.size() > MAX_SIZE)
            return false;

        if (mazeWidth == -1) {
            mazeWidth = tokens.size();
        } else if (tokens.size() != mazeWidth) {
            return false;
        }

        int col = 0;
        for (const QString &token : tokens) {
            qreal x = col * m_cellSize + m_cellSize / 2;
            qreal y = row * m_cellSize + m_cellSize / 2;

            if (token == "p" || token == "P") {
                if (!pacman) {
                    pacman = new Pacman(m_cellSize);
                    pacman->setPos(x, y);
                    scene->addItem(pacman);
                }
            } else if (token.toLower() == "g") {
                Ghost *ghost = new Ghost(Qt::red, m_cellSize);
                ghost->setPos(x, y);
                scene->addItem(ghost);
                ghosts.append(ghost);
            } else if (token == ".") {

            } else {
                QColor wallColor = getColorForToken(token);
                Wall *wall = new Wall(
                    x - m_cellSize / 2,
                    y - m_cellSize / 2,
                    m_cellSize,
                    m_cellSize,
                    wallColor
                    );
                scene->addItem(wall);
            }
            col++;
        }
        row++;
    }
    file.close();

    if (row == 0)
        return false;
    return true;
}
