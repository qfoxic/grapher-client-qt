#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QStaticText>
#include "graph/edge.h"
#include "graph/node.h"
#include <math.h>


Edge::Edge(Node *source, Node *dest, QGraphicsItem *parent) : QGraphicsWidget{parent} {
    setAcceptedMouseButtons(0);
    setCacheMode(ItemCoordinateCache);
    pen = {Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin};
    source = source;
    dest = dest;
    adjust();
}

Edge::~Edge() {
    qDebug() << "deleted Edge";
}

QRectF Edge::boundingRect() const {
    return QRectF(
        sourcePoint,
        QSizeF{destPoint.x() - sourcePoint.x(), destPoint.y() - sourcePoint.y()}
    ).normalized().adjusted(-0.5, -0.5, 0.5, 0.5);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget)
    Q_UNUSED(option)

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    painter->setPen(pen);
    painter->drawLine(line);
}

void Edge::adjust()
{
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();
    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }
}
