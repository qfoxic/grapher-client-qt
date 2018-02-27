#include "tooltip.h"
#include <QTreeView>
#include <QJsonObject>
#include <QJsonArray>
#include <QGridLayout>
#include <QDebug>


GraphToolTip::GraphToolTip(QWidget *parent) : QWidget{parent}
{
    gridLayout = new QGridLayout(this);
    model = new QJsonModel(this);
    view = new QTreeView(this);
    view->setModel(model);
    gridLayout->addWidget(view, 0, 0, 1, 1);
    resize(590, 430);
    setWindowFlags(Qt::ToolTip|Qt::WindowStaysOnTopHint);
}

GraphToolTip& GraphToolTip::getInstance(Node *node) {
    static GraphToolTip instance;
    return instance.initModel(node);
}

GraphToolTip& GraphToolTip::initModel(Node* node) {
    if (node) {
        model->loadJson(node->toolTip().toUtf8());
    } else {
        hide();
    }
    return *this;
}

QJsonTreeItem::QJsonTreeItem(QJsonTreeItem *parent)
{
    mParent = parent;
}

QJsonTreeItem::~QJsonTreeItem()
{
    qDeleteAll(mChilds);
}

void QJsonTreeItem::appendChild(QJsonTreeItem *item)
{
    mChilds.append(item);
}

QJsonTreeItem *QJsonTreeItem::child(int row)
{
    return mChilds.value(row);
}

QJsonTreeItem *QJsonTreeItem::parent()
{
    return mParent;
}

int QJsonTreeItem::childCount() const
{
    return mChilds.count();
}

int QJsonTreeItem::row() const
{
    if (mParent)
        return mParent->mChilds.indexOf(const_cast<QJsonTreeItem*>(this));

    return 0;
}

QJsonTreeItem* QJsonTreeItem::load(const QJsonValue& value, QJsonTreeItem* parent)
{
    QJsonTreeItem * rootItem = new QJsonTreeItem(parent);
    rootItem->key = "root";

    if (value.isObject()) {
        for (QString key : value.toObject().keys()) {
            QJsonValue v = value.toObject().value(key);
            QJsonTreeItem * child = load(v, rootItem);
            child->key = key;
            rootItem->appendChild(child);
        }
    }
    else if ( value.isArray()) {
        int index = 0;
        for (QJsonValue v : value.toArray()) {
            QJsonTreeItem * child = load(v, rootItem);
            child->key = QString::number(index++);
            rootItem->appendChild(child);
        }
    }
    else {
        rootItem->value = value.toVariant().toString();
    }

    return rootItem;
}

//=========================================================================

QJsonModel::QJsonModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    mRootItem = new QJsonTreeItem(nullptr);
    mHeaders << "key" << "value";
}

QJsonModel::~QJsonModel()
{
    delete mRootItem;
}

bool QJsonModel::loadJson(const QByteArray &json)
{
    mDocument = QJsonDocument::fromJson(json);

    if (!mDocument.isNull()) {
        beginResetModel();
        delete mRootItem;
        if (mDocument.isArray()) {
            mRootItem = QJsonTreeItem::load(QJsonValue(mDocument.array()));
        } else {
            mRootItem = QJsonTreeItem::load(QJsonValue(mDocument.object()));
        }
        endResetModel();
        return true;
    }
    return false;
}

QVariant QJsonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QJsonTreeItem *item = static_cast<QJsonTreeItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0)
            return item->key;

        if (index.column() == 1)
            return item->value;
    }
    return QVariant();
}

QVariant QJsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return mHeaders.value(section);

    return QVariant();
}

QModelIndex QJsonModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QJsonTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());

    QJsonTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex QJsonModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QJsonTreeItem *childItem = static_cast<QJsonTreeItem*>(index.internalPointer());
    QJsonTreeItem *parentItem = childItem->parent();

    if (parentItem == mRootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QJsonModel::rowCount(const QModelIndex &parent) const
{
    QJsonTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<QJsonTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int QJsonModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}
