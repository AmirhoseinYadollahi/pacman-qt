#include "pacman.h"
#include "wall.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QtMath>

Pacman::Pacman(int cellSize)
    : m_mouthOpen(true), m_direction(0, 0), m_cellSize(cellSize), m_nextDirection(0, 0), m_size(cellSize * 0.8)
{
    setFlag(ItemIsFocusable, true);
    setFocus();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Pacman::movePacman);
    m_timer->start(50);
}

QRectF Pacman::boundingRect() const {
    return QRectF(-m_size/2, -m_size/2, m_size, m_size);
}

void Pacman::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::yellow);

    if (isMoving()) {
        if (m_mouthOpen) {
            painter->drawPie(boundingRect(), 30 * 16, 300 * 16);
        } else {
            painter->drawEllipse(boundingRect());
        }
    } else {
        painter->drawPie(boundingRect(), 30 * 16, 300 * 16);
    }
}

void Pacman::setDirection(const QPointF &dir) {
    m_direction = dir;
    update();
}

void Pacman::movePacman() {
    if (!m_nextDirection.isNull()) {
        // FIX: Calculate cell position correctly
        int col = qRound((pos().x() - m_cellSize/2) / m_cellSize);
        qreal xCenter = col * m_cellSize + m_cellSize/2;
        int row = qRound((pos().y() - m_cellSize/2) / m_cellSize);
        qreal yCenter = row * m_cellSize + m_cellSize/2;

        const qreal tolerance = 2.0;

        if (qAbs(pos().x() - xCenter) < tolerance &&
            qAbs(pos().y() - yCenter) < tolerance) {

            setPos(xCenter, yCenter);
            QPointF testPos = pos() + m_nextDirection * (m_cellSize / 4);
            setPos(testPos);

            bool canMove = true;
            QList<QGraphicsItem *> testCollisions = collidingItems();
            for (QGraphicsItem *item : testCollisions) {
                if (dynamic_cast<Wall*>(item)) {
                    canMove = false;
                    break;
                }
            }

            setPos(xCenter, yCenter);

            if (canMove) {
                m_direction = m_nextDirection;
                if (m_direction.x() < 0) setRotation(180);
                else if (m_direction.x() > 0) setRotation(0);
                else if (m_direction.y() < 0) setRotation(270);
                else if (m_direction.y() > 0) setRotation(90);
            }
        }
    }

    if (m_direction.isNull()) {
        m_mouthOpen = false;
        update();
        return;
    }

    const qreal stepSize = 2 * m_cellSize / 8;
    QPointF oldPos = pos();
    QPointF newPos = oldPos + m_direction * stepSize;

    setPos(newPos);
    QList<QGraphicsItem *> collisions = collidingItems();
    bool hitWall = false;
    for (QGraphicsItem *item : collisions) {
        if (dynamic_cast<Wall*>(item)) {
            hitWall = true;
            break;
        }
    }

    if (hitWall) {
        setPos(oldPos);
    }

    m_mouthOpen = !m_mouthOpen;
    update();
}

void Pacman::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Left:
        m_nextDirection = QPointF(-1, 0);
        break;
    case Qt::Key_Right:
        m_nextDirection = QPointF(1, 0);
        break;
    case Qt::Key_Up:
        m_nextDirection = QPointF(0, -1);
        break;
    case Qt::Key_Down:
        m_nextDirection = QPointF(0, 1);
        break;
    default:
        QGraphicsObject::keyPressEvent(event);
    }
}

void Pacman::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        QGraphicsObject::keyReleaseEvent(event);
    }
}
