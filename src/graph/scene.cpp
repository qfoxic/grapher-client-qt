#include "scene.h"
#include "graph/tooltip.h"
#include <QGraphicsSceneHelpEvent>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QToolTip>
#include <QDebug>
#include <QWidget>


GraphScene::GraphScene(QObject *parent) : QGraphicsScene{parent} {}


QList<QGraphicsItem*> GraphScene::itemsAtPos(const QPoint &screenPos, const QPointF &scenePos, QWidget *widget) const {
    QGraphicsView *view = views().at(0);
    const QRectF pointRect(QPointF(widget->mapFromGlobal(screenPos)), QSizeF(1, 1));
    if (!view)
        return items(scenePos, Qt::IntersectsItemShape, Qt::DescendingOrder, QTransform());

    if (!view->isTransformed())
        return items(pointRect, Qt::IntersectsItemShape, Qt::DescendingOrder);

    const QTransform viewTransform = view->viewportTransform();
    if (viewTransform.type() <= QTransform::TxScale) {
        return items(viewTransform.inverted().mapRect(pointRect), Qt::IntersectsItemShape,
                     Qt::DescendingOrder, viewTransform);
    }
    return items(viewTransform.inverted().map(pointRect), Qt::IntersectsItemShape,
                 Qt::DescendingOrder, viewTransform);

}

void GraphScene::helpEvent(QGraphicsSceneHelpEvent *helpEvent) {

    QList<QGraphicsItem *> items = itemsAtPos(helpEvent->screenPos(),
                                              helpEvent->scenePos(),
                                              helpEvent->widget());
    if(items.size() >= 1) {
        Node* node = qgraphicsitem_cast<Node*>(items.at(0));
        if(node) {
            GraphToolTip::getInstance(node).move(helpEvent->screenPos());
            GraphToolTip::getInstance(node).show();
        } else {
            GraphToolTip::getInstance(node).hide();
        }
    }
    helpEvent->setAccepted(true);
}
