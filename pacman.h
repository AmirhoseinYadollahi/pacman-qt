#pragma once
#include <QGraphicsObject>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>

class Pacman : public QGraphicsObject {
    Q_OBJECT
public:
    Pacman(int cellSize);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void setDirection(const QPointF &dir);
    bool isMoving() const { return !m_direction.isNull(); }

public slots:
    void movePacman();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    bool m_mouthOpen;
    QTimer *m_timer;
    QPointF m_direction;
    int m_cellSize;
    QPointF m_nextDirection;
    qreal m_size;
};
