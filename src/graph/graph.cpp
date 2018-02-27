#include "config/config.h"
#include "graph/graph.h"
#include "graph/node.h"
#include <QJsonDocument>
#include <QDebug>
#include <QVariant>


Graph::Graph(QObject *parent) : QObject(parent)
{}

Graph::~Graph() {
    qDebug() << "deleted graph";
}

bool Graph::addNode(QJsonObject& jsonNode) {
    QString id{jsonNode.value(Constants::NODE_ID_FIELD).toString()},
            type{jsonNode.value(Constants::NODE_TYPE_FIELD).toString()};

    if (id.isEmpty() or type.isEmpty()) {
        qDebug() << "<id> or <type> fields are missed" << jsonNode;
        return false;
    }

    Node* node = getNode(id);
    if (node != nullptr) {
        node->setNodeProperties(jsonNode);
        emit nodeUpdated(node);
    } else {
        node = new Node{this, jsonNode};
        nodes[id] = node;
        emit nodeAdded(node);
    }
    return true;
}

void Graph::clear() {
    childrenList.clear();
    childrenList = QHash<QString, QVector<Node*>>();
    parentsList.clear();
    parentsList = QHash<QString, QVector<Node*>>();
    nodes.clear();
    nodes = QHash<QString, Node*>();
}

Node* Graph::getNode(const QString& id) const {
    if (nodes.contains(id)) {
        Node* n = nodes[id];
        if (n != nullptr) {
            return n;
        }
    }
    return nullptr;
}

bool Graph::addLink(const QJsonObject& jsonNode) {
    QString id{jsonNode.value(Constants::NODE_ID_FIELD).toString()},
            pid{jsonNode.value(Constants::NODE_PID_FIELD).toString()};
    Node* parent = getNode(pid);
    Node* child = getNode(id);
    if (parent == nullptr or child == nullptr) {
        qDebug()<<"Link hasn't been created because parent of a child wasn't found"
               <<"Parent node:"<<parent<<"-- id:"<<pid<<"Child node:"<<child<<"-- id"<<id;
        return false;
    }

    if (childrenList[pid].contains(child)) {
        qDebug()<<"Duplicate link: parent - "<< parent << "- child - " << child;
    } else {
        // Update parentsList of our current node.
        parentsList[id].append(parent);

        // Add this node to parent's children.
        childrenList[pid].append(child);
        emit linkAdded(parent, child);
    }
    return true;
}
