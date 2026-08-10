#pragma once
#include <QGraphicsObject>
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>
#include <QPointF>
#include <QPixmap>

class Ghost : public QGraphicsObject {
    Q_OBJECT
public:
    Ghost(const QColor &color, int cellSize, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

public slots:
    void moveGhost();
    void changeDirection();

private:
    QPixmap m_ghostPixmap;
    QColor m_color;
    QTimer *m_timer;
    QTimer *m_randomTimer;
    QPointF m_direction;
    int m_cellSize;
    qreal m_size;
};
