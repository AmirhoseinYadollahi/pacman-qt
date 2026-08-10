#pragma once
#include <QGraphicsRectItem>
#include <QColor>

class Wall : public QGraphicsRectItem {
public:
    // Draws a wall rectangle at the given position, size and with the given fill color.
    Wall(qreal x, qreal y, qreal width, qreal height, const QColor &color);
};
