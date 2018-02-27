#include <QDebug>
#include <QGraphicsGridLayout>
#include "graph/view.h"
#include "graph/node.h"
#include "graph/graph.h"
#include "graph/scene.h"
#include "config/config.h"
#include "server/client.h"


// select('node.state!="running" && node.type!="elb"', 'node.highlight("red")')
// dataClient.command("data types=ec2,sg&&links=sg:ec2&&ec2=i['id']=='i-03d515dee10bdd286'")


GraphView::GraphView(const QScriptValue& conf, const QString& name, QWidget *parent):
        QGraphicsView{parent}, config{conf} {
    scriptNodes = scriptEngine.newObject();
    scriptEngine.globalObject().setProperty("nodes", scriptNodes);
    setObjectName(name);
    setScene(new GraphScene(this));
    setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(AnchorUnderMouse);
    scale(1.0, 1.0);

    __initJsonClient();

    Config::include_js(scriptEngine, Constants::COMMON_JS_FILE);
    Config::include_js(scriptEngine, Constants::VIEW_JS_FILE.arg(name));

    graph = new Graph(this);
    __initRootViewItem();

    connect(graph, &Graph::nodeAdded, this, &GraphView::displayNewNode);
    connect(graph, &Graph::linkAdded, this, &GraphView::displayLink);
}

void GraphView::mouseDoubleClickEvent(QMouseEvent *event) {
    scale(1.3, 1.3);
    event->ignore();
}

void GraphView::__initRootViewItem() {
    row = 0;
    col = 0;
    rootItem = new QGraphicsWidget();
    layout = new QGraphicsGridLayout(rootItem);
    layout->setSpacing(10);
    scene()->addItem(rootItem);
}

void GraphView::clearView() {
    scriptNodes = scriptEngine.newObject();
    scriptEngine.globalObject().setProperty("nodes", scriptNodes);
    graph->clear();
    scene()->clear();
    scene()->invalidate();
    __initRootViewItem();
}

QString GraphView::getViewHost() const {
    return config.property("host").toString();
}

int GraphView::getViewPort() const {
    return config.property("port").toInteger();
}

QString GraphView::getNodeColorByType(const QString& ntype) const {
    return config.property("types").property(ntype).property("color").toString();
}

GraphView::~GraphView() {
    qDebug() << "Removed graph view" << objectName();
}

void GraphView::__initJsonClient() {
    client = new JSONClient(getViewHost(), getViewPort(), this);

    qScriptRegisterMetaType(&scriptEngine, jsonClientToScript, jsonClientFromScript);

    scriptEngine.globalObject().setProperty("dataClient", scriptEngine.newQObject(client));
    connect(client, &JSONClient::jsonReady, this, &GraphView::processJSONNode);
}

void GraphView::processJSONNode(QJsonObject& jsonNode) {
    QString type{jsonNode.value(Constants::NODE_TYPE_FIELD).toString()};
    if(type == Constants::LINK_NODE) {
        graph->addLink(jsonNode);
    } else {
        graph->addNode(jsonNode);
    }
}

void GraphView::displayLink(Node* parent, Node* child) {
    parent->addLinkNode(child);
}

void GraphView::displayNewNode(Node *node) {
    static const unsigned short rotationIndex{100}, colMinWidth{200}, rowMaxHeight{200};
    unsigned short cur_row = (row++) % rotationIndex;

    if (cur_row <= 0) {
        layout->setColumnMinimumWidth(col++, colMinWidth);
    }
    // No need to repeat the same operation on existing  rows.
    if (row <= rotationIndex) {
        layout->setRowMaximumHeight(cur_row, rowMaxHeight);
    }

    // Add node to a script engine.
    scriptNodes.setProperty(node->property(Constants::NODE_ID_FIELD.toLatin1().constData()).toString(), scriptEngine.newQObject(node));
    node->setNodeColor(getNodeColorByType(node->property(Constants::NODE_TYPE_FIELD.toLatin1().constData()).toString()));
    layout->addItem(node, cur_row, col);
}

void GraphView::evaluateJs(const QString& js) {
    if (js.isEmpty()) {
        return;
    }
    QScriptValue result = scriptEngine.evaluate(js);
    qDebug()<<result.toString();
    if (scriptEngine.hasUncaughtException()) {
        qDebug() << "uncaught exception at line" << scriptEngine.uncaughtExceptionLineNumber()
                 << ":" << result.toString();
    }
}
