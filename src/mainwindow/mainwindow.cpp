#include <QIcon>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_toolpage.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "config/config.h"
#include "graph/view.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Stupid workaround to remove one default page from UI toolBox.
    ui->toolBox->removeItem(0);

}

void MainWindow::updateViewTabIcon(const ushort &itemId, QAbstractSocket::SocketState state) {
    switch (state) {
        case QAbstractSocket::UnconnectedState:
            ui->toolBox->setItemIcon(itemId, QIcon(":/icons/disconnected.png"));
            break;
        case QAbstractSocket::ClosingState:
            break;
        case QAbstractSocket::HostLookupState:
        case QAbstractSocket::ConnectingState:
            ui->toolBox->setItemIcon(itemId, QIcon(":/icons/connecting.png"));
            break;
        case QAbstractSocket::ConnectedState:
        case QAbstractSocket::BoundState:
        case QAbstractSocket::ListeningState:
            ui->toolBox->setItemIcon(itemId, QIcon(":/icons/connected.png"));
            break;
        default:
            ui->toolBox->setItemIcon(itemId, QIcon(":/icons/disconnected.png"));
            break;
    }
}

QWidget* MainWindow::createToolPage(QToolBox* parent, GraphView* view) {
    QWidget *widget = new QWidget(parent);
    Ui::ToolPage ui;
    ui.setupUi(widget);
    connect(ui.incView, &QToolButton::clicked, view, [=](){
        view->scale(1.1, 1.1);
    });
    connect(ui.decView, &QToolButton::clicked, view, [=](){
        view->scale(0.9, 0.9);
    });
    connect(ui.fitView, &QToolButton::clicked, view, [=](){
        view->fitInView(view->scene()->sceneRect(), Qt::KeepAspectRatio);
    });
    connect(ui.clearView, &QToolButton::clicked, view, [=](){
        view->clearView();
    });
    connect(ui.refreshConnBtn, &QToolButton::clicked, view, [=](){
        view->getJsonClient()->connectClient();
    });
    return widget;
}

void MainWindow::initMainWindow(QList<GraphView*> &graphViews) {

    connect(ui->toolBox, &QToolBox::currentChanged, [&]() {
        for (GraphView* view: graphViews) {view->hide();}
        currentView = graphViews.at(ui->toolBox->currentIndex());
        currentView->show();
    });

    connect(ui->executeButton, &QToolButton::clicked, [&]() {
        currentView->evaluateJs(ui->scriptLineEdit->text());
    });

    for (GraphView* view: graphViews) {
        view->setParent(centralWidget());
        ui->verticalLayout->addWidget(view);
        view->hide();
        ushort itemId = ui->toolBox->addItem(
            createToolPage(ui->toolBox, view),
            view->objectName()
        );
        JSONClient* client = view->getJsonClient();
        updateViewTabIcon(itemId, client->state());

        connect(client, &JSONClient::requestStarted, this, &MainWindow::disableExecButton);
        connect(client, &JSONClient::requestSuccess, this, &MainWindow::enableExecButton);
        connect(client, &JSONClient::requestCompleted, this, [=](const QString& data){
            if (!data.isEmpty()) {
                QMessageBox msgBox;
                msgBox.setText(data);
                msgBox.exec();
            }
        });
        connect(client, &JSONClient::requestFailed, this, &MainWindow::enableExecButton);
        connect(client, &JSONClient::stateChanged, this, [=](QAbstractSocket::SocketState state){
            updateViewTabIcon(itemId, state);
        });
        connect(client, &JSONClient::stateChanged, ui->executeButton, [=](){
            enableExecButton();
        });
    }
}

void MainWindow::disableExecButton() {
    ui->executeButton->setDisabled(true);
}

void MainWindow::enableExecButton() {
    ui->executeButton->setDisabled(false);
}

GraphView* MainWindow::getCurrentView() const {
    return currentView;
}

MainWindow::~MainWindow()
{
    delete ui;
}
