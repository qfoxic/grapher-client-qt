#ifndef GRAPHTOOLTIP_H
#define GRAPHTOOLTIP_H

#include <QWidget>
#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonValue>
#include "graph/node.h"

class QJsonModel;
class QTreeView;
class QGridLayout;
class Node;


class QJsonTreeItem
{
public:
    friend class QJsonModel;
    explicit QJsonTreeItem(QJsonTreeItem* parent = nullptr);
    ~QJsonTreeItem();

    void appendChild(QJsonTreeItem* item);
    QJsonTreeItem *child(int row);
    QJsonTreeItem *parent();
    int childCount() const;
    int row() const;
    static QJsonTreeItem* load(const QJsonValue& value, QJsonTreeItem * parent = 0);

private:
    QString key;
    QString value;
    QList<QJsonTreeItem*> mChilds;
    QJsonTreeItem* mParent;
};

class QJsonModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QJsonModel(QObject *parent = 0);
    virtual ~QJsonModel() override final;
    bool loadJson(const QByteArray &json);
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    QJsonTreeItem *mRootItem;
    QJsonDocument mDocument;
    QStringList mHeaders;
};


class GraphToolTip : public QWidget
{
    Q_OBJECT
public:
    static GraphToolTip& getInstance(Node *node);
    GraphToolTip& initModel(Node* node);

private:
    GraphToolTip(QWidget *parent = nullptr);
    virtual ~GraphToolTip() {}

public:
    Q_DISABLE_COPY(GraphToolTip)
    GraphToolTip& operator=(GraphToolTip&&) = delete;
    GraphToolTip(GraphToolTip&&) = delete;

private:
    QTreeView* view;
    QJsonModel* model;
    QGridLayout* gridLayout;
};

#endif // GRAPHTOOLTIP_H
