#ifndef EDGE_H
#define EDGE_H


#include "node.h"


class Edge : public Node
{

public:
    enum { Type = UserType + 2 };

    explicit Edge(Node* source, Node* dest, QGraphicsItem *parent = nullptr);
    virtual ~Edge() override final;

    void adjust();

    inline Node* destNode() const { return dest; }
    inline void incrCount() { ++count; }

    virtual QRectF boundingRect() const override final;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;
    virtual int type() const override final { return Type; }

private:
    Q_DISABLE_COPY(Edge)
    Edge& operator=(Edge&&) = delete;
    Edge(Edge&&) = delete;

private:
    Node *source, *dest;
    QPointF sourcePoint;
    QPointF destPoint;
    QRectF bRect;
    QPen pen;
    int count{1};
};

#endif // EDGE_H
