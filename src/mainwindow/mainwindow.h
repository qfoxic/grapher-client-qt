#ifndef MWINDOW_H
#define MWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>


class GraphView;
class QToolBox;
class QWidget;


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow() override final;
    void initMainWindow(QList<GraphView*> &graphViews);
    GraphView* getCurrentView() const;

public slots:
    void updateViewTabIcon(const ushort &itemId, QAbstractSocket::SocketState state);
    void disableExecButton();
    void enableExecButton();

private:
    Q_DISABLE_COPY(MainWindow)
    MainWindow& operator=(MainWindow&&) = delete;
    MainWindow(MainWindow&&) = delete;

private:
    QWidget* createToolPage(QToolBox *parent, GraphView *view);
    void createLoader();

private:
    Ui::MainWindow *ui;
    GraphView* currentView{nullptr};
};

#endif // MWINDOW_H
