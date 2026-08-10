#include "ghost.h"
#include "wall.h"
#include <QGraphicsScene>
#include <cmath>
Ghost::Ghost(const QColor &color, int cellSize, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_color(color), m_cellSize(cellSize), m_size(cellSize * 0.8)
{
    m_ghostPixmap.load(":Images/../Images/ghost.png");

    if (!m_ghostPixmap.isNull()) {
        m_ghostPixmap = m_ghostPixmap.scaled(m_size, m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    setZValue(1);

    int d = QRandomGenerator::global()->bounded(4);
    switch (d) {
    case 0: m_direction = QPointF(1, 0); break;
    case 1: m_direction = QPointF(-1, 0); break;
    case 2: m_direction = QPointF(0, 1); break;
    case 3: m_direction = QPointF(0, -1); break;
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Ghost::moveGhost);
    m_timer->start(80);

    m_randomTimer = new QTimer(this);
    connect(m_randomTimer, &QTimer::timeout, this, &Ghost::changeDirection);
    m_randomTimer->start(80);
}

QRectF Ghost::boundingRect() const {
    return QRectF(-m_size/2, -m_size/2, m_size, m_size);
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!m_ghostPixmap.isNull()) {
        painter->drawPixmap(-m_size/2, -m_size/2, m_ghostPixmap);
    } else {
        // Fallback to drawing if image not loaded
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_color);
    }

    painter->setBrush(Qt::white);
    qreal eyeSize = m_size/4;
    qreal eyeOffsetX = m_size/4;
    qreal eyeOffsetY = -m_size/6;

    painter->drawEllipse(QPointF(-eyeOffsetX, eyeOffsetY), eyeSize/2, eyeSize/2);
    painter->drawEllipse(QPointF(eyeOffsetX, eyeOffsetY), eyeSize/2, eyeSize/2);

    painter->setBrush(Qt::black);
    qreal pupilSize = eyeSize/3;
    QPointF pupilOffset;

    if (m_direction.x() > 0) pupilOffset = QPointF(pupilSize, 0);
    else if (m_direction.x() < 0) pupilOffset = QPointF(-pupilSize, 0);
    else if (m_direction.y() > 0) pupilOffset = QPointF(0, pupilSize);
    else pupilOffset = QPointF(0, -pupilSize);

    painter->drawEllipse(QPointF(-eyeOffsetX, eyeOffsetY) + pupilOffset, pupilSize, pupilSize);
    painter->drawEllipse(QPointF(eyeOffsetX, eyeOffsetY) + pupilOffset, pupilSize, pupilSize);
}

void Ghost::moveGhost() {
    const qreal stepSize = 2 * m_cellSize / 8;
    QPointF oldPos = pos();
    QPointF newPos = oldPos + m_direction * stepSize;

    setPos(newPos);
    QList<QGraphicsItem*> collisions = collidingItems();
    bool hitWall = false;
    for (QGraphicsItem *item : collisions) {
        if (dynamic_cast<Wall*>(item)) {
            hitWall = true;
            break;
        }
    }

    if (hitWall) {
        setPos(oldPos);
        changeDirection();
    }
}

void Ghost::changeDirection() {
    // Try all possible directions including reverse
    QVector<QPointF> directions = {
        QPointF(1, 0), QPointF(-1, 0),
        QPointF(0, 1), QPointF(0, -1)
    };

    QVector<QPointF> available;
    QPointF oldPos = pos();

    for (const QPointF &dir : directions) {
        setPos(oldPos + dir * (m_cellSize / 4));
        bool collides = false;
        QList<QGraphicsItem*> testCollisions = collidingItems();
        for (QGraphicsItem *item : testCollisions) {
            if (dynamic_cast<Wall*>(item)) {
                collides = true;
                break;
            }
        }
        if (!collides)
            available.append(dir);
    }

    setPos(oldPos);

    if (!available.isEmpty()) {
        // Prefer non-reverse directions if possible
        QVector<QPointF> nonReverse;
        for (const QPointF &dir : available) {
            if (dir != -m_direction) {
                nonReverse.append(dir);
            }
        }

        if (!nonReverse.isEmpty()) {
            int idx = QRandomGenerator::global()->bounded(nonReverse.size());
            m_direction = nonReverse.at(idx);
        } else {
            // If only reverse is available, use it
            int idx = QRandomGenerator::global()->bounded(available.size());
            m_direction = available.at(idx);
        }
        update();
    }
}
