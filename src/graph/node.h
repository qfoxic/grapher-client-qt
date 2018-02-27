#ifndef NODE_H
#define NODE_H

#include <QGraphicsWidget>
#include <QPen>
#include <QList>
#include <QStaticText>
#include <QRadialGradient>
#include <QJsonObject>


class Graph;
class QGraphicsLinearLayout;


class Node : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum { Type = UserType + 1 };

    explicit Node(Graph *nodeGraph, const QJsonObject& data = QJsonObject{}, QGraphicsItem *parent = nullptr);
    virtual ~Node() override final;

    void addLinkNode(Node *child);
    void setNodeProperties(const QJsonObject& data);
    void setNodeColor(const QString& color);

    Node* getParentNode() const { return parentNode; }
    Node* clone();

    virtual QRectF boundingRect() const override final;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;
    virtual QPainterPath shape() const override final;
    virtual int type() const override final { return Type; }

public slots:
    void highlight(const QString&);

public:
    QGraphicsLinearLayout* getLayout(const QString& nType);
    QGraphicsLinearLayout* makeNewLayout();

private:
    Q_DISABLE_COPY(Node)
    Node& operator=(Node&&) = delete;
    Node(Node&&) = delete;

private:
    Graph* graph{nullptr};
    Node* parentNode{nullptr}; // Parent node in a tree.
    QGraphicsLinearLayout* rootLayout{nullptr};
    QRadialGradient gradient{-3, -3, 10};

    // Visual drawing.
    QStaticText nodeTitle{};
    QPointF topLeftPoint{-10, -10};
    QPen pen{};
    QRectF bRect{};
    QColor color{};

    // Json object data;
    QJsonObject jsonData{};

    // Layout data;
    // TODO. Make a separate class.
    QMap<QString, QGraphicsLinearLayout*> layouts;
};
#endif // NODE_H
