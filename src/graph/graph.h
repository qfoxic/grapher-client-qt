#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QHash>
#include <QVector>
#include <QPoint>

#include <QDebug>


class Node;


class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject* parent = nullptr);
    virtual ~Graph();

    bool addNode(QJsonObject &document);
    bool addLink(const QJsonObject &jsonNode);
    Node* getNode(const QString& id) const;
    void clear();

signals:
    void nodeAdded(Node* node) const;
    void nodeUpdated(Node* node) const;
    void linkAdded(Node* from, Node* to) const;

private:
    // Map of all accepted nodes. key==node_id.
    QHash<QString, Node*> nodes;

    // Map of all child links, key==node_id, value=list of all child links of a node.
    // Node may have doubled or tripled links.
    QHash<QString, QVector<Node*>> childrenList; // return nChildren[node.nodeId()];

    // Map of all parent links, key==node_id, value=list of all parent links of a node.
    // Node may have doubled or tripled links.
    QHash<QString, QVector<Node*>> parentsList; // return parents[node.nodeId()];
};

#endif // GRAPH_H
