#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsLinearLayout>
#include <QGraphicsEffect>
#include <QPainterPath>
#include <QJsonObject>
#include <QJsonDocument>
#include <QPointer>
#include "config/config.h"
#include "graph/node.h"
#include "graph/graph.h"


Node::Node(Graph *nodeGraph, const QJsonObject &data, QGraphicsItem *parent):
        QGraphicsWidget{parent}, graph{nodeGraph}, jsonData{data} {

    nodeTitle = jsonData.value(Constants::NODE_ID_FIELD).toString();
    bRect = {topLeftPoint, nodeTitle.size()};
    pen = {Qt::black, 0};

    setObjectName(jsonData.value(Constants::NODE_ID_FIELD).toString());
    setToolTip(QJsonDocument{jsonData}.toJson());
    setNodeProperties(jsonData);
    setFlags(ItemSendsGeometryChanges|ItemIsSelectable|ItemIsMovable);
    setCacheMode(ItemCoordinateCache);
    setZValue(-1);
    setMinimumWidth(bRect.width() * 0.7);
    setMinimumHeight(bRect.height() * 0.4);

    rootLayout = new QGraphicsLinearLayout(this);
    rootLayout->setOrientation(Qt::Vertical);
    rootLayout->setMaximumWidth(50);
    rootLayout->setMaximumHeight(50);
}

Node::~Node() {}

QGraphicsLinearLayout* Node::getLayout(const QString& nType)  {
    QGraphicsLinearLayout* layout = layouts[nType];

    if (layout == nullptr) {
        layout = makeNewLayout();
        rootLayout->addItem(layout);
        layouts[nType] = layout;
    }
    return layout;
}

QGraphicsLinearLayout* Node::makeNewLayout()  {
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(
        rootLayout->orientation() == Qt::Vertical ? Qt::Horizontal : Qt::Vertical,
        rootLayout
    );
    layout->setMaximumWidth(500);
    layout->setMaximumHeight(500);
    return layout;
}

void Node::addLinkNode(Node* child) {
    // TODO. Rewrite it with a Gridlayout.

    // We have to check if the item is already in some layout, otherwise when re-arranging it whole application just hungs.
    if (child->parentNode != nullptr) {
        child = child->clone();
    }

    child->parentNode = this;
    if (parentNode) {
        rootLayout->setOrientation(parentNode->rootLayout->orientation() == Qt::Vertical ? Qt::Horizontal : Qt::Vertical);
    }
    QString nType{child->property("type").toString()};
    getLayout(nType)->addItem(child);
}

Node* Node::clone() {
    Node* n = new Node(graph, jsonData);
    n->setNodeColor(color.name());
    return n;
}

void Node::setNodeProperties(const QJsonObject& data) {
    for (QString &key: data.keys()) {
        setProperty(key.toLatin1().constData(), data.value(key).toVariant());
    }
}

void Node::setNodeColor(const QString& nodeColor) {
    color = {nodeColor};
    color.setAlphaF(0.7);
}

QPainterPath Node::shape() const {
    QPainterPath path;
    path.addRect(bRect);
    return path;
}

QRectF Node::boundingRect() const {
    return bRect;
}

void Node::highlight(const QString& highlightColor) {
    QPointer<QGraphicsDropShadowEffect> effect = new QGraphicsDropShadowEffect{};
    effect->setBlurRadius(15);
    effect->setColor(QColor(highlightColor));
    effect->setOffset(0);
    setGraphicsEffect(effect);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(widget)
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(0, color.lighter(170));
    } else if (option->state & QStyle::State_Selected){
        gradient.setColorAt(0, color.lighter(170));
    } else {
        gradient.setColorAt(0, color);
    }

    painter->setBrush(gradient);
    painter->setPen(pen);
    painter->drawRect(bRect);
    painter->drawStaticText(topLeftPoint, nodeTitle);
}
