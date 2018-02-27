#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>


class QGraphicsSceneHelpEvent;


class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = nullptr);

protected:
    virtual void helpEvent(QGraphicsSceneHelpEvent *helpEvent) override final;

private:
    QList<QGraphicsItem*> itemsAtPos(const QPoint &screenPos, const QPointF &scenePos, QWidget *widget) const;

};
#endif // SCENE_H
