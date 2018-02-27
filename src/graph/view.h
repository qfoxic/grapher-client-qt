#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QScriptEngine>
#include <QJsonObject>


class Node;
class Graph;
class JSONClient;
class QGraphicsGridLayout;
class QGraphicsWidget;


class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphView(const QScriptValue& config, const QString& name, QWidget *parent=nullptr);
    virtual ~GraphView() override final;

    void evaluateJs(const QString&);

    QScriptEngine& getEngine() { return scriptEngine; }
    QString getViewHost() const;
    JSONClient* getJsonClient() const { return client; }
    int getViewPort() const;
    QString getNodeColorByType(const QString& ntype) const;

public slots:
    void processJSONNode(QJsonObject &);
    void displayNewNode(Node*);
    void displayLink(Node* parent, Node* child);
    void clearView();

signals:
    void requestSuccess() const;
    void requestFailed() const;
    void requestCompleted() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override final;

private:
    Q_DISABLE_COPY(GraphView)
    GraphView& operator=(GraphView&&) = delete;
    GraphView(GraphView&&) = delete;

    void __initJsonClient();
    void __initRootViewItem();

private:
    Graph* graph{nullptr};
    JSONClient* client{nullptr};
    QScriptValue config;
    QScriptEngine scriptEngine;
    QScriptValue scriptNodes;
    QGraphicsGridLayout* layout{nullptr};
    QGraphicsWidget* rootItem{nullptr};

    unsigned short row{0}, col{0};
};

#endif // GRAPHVIEW_H
