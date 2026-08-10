#include "wall.h"
#include <QPen>

Wall::Wall(qreal x, qreal y, qreal width, qreal height, const QColor &color)
    : QGraphicsRectItem(x, y, width, height)
{
    QPen pen(Qt::white);
    pen.setWidth(2);
    setPen(pen);
    setBrush(color);
}
