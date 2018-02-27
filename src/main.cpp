#include <QDebug>
#include <QApplication>
#include <QScriptValueIterator>
#include <QIcon>
#include "graph/view.h"
#include "mainwindow/mainwindow.h"
#include "config/config.h"


// TODO. Add title (id/name) of a node to a pop-up.
// BUG! Add close button to popup.

// TODO. Replace modal pop-ups with something more nice.
// !TODO. Add hooks on different actions. node came in. node moved, node updated. Hooks could be available from JS.
// !TODO. Add ability to save common filters.
// TODO! Item Context Menu:
/* - adjust function: is to adjust all children around parent.
*/


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/grapher.png"));
    app.setApplicationName("Grapher");
    MainWindow mainWindow;
    QScriptEngine engine;
    QScriptValue config = Config::readConfig(engine);
    QScriptValueIterator it(config);
    QList<GraphView*> graphViews;

    while (it.hasNext()) {
        it.next();
        GraphView* v = new GraphView(it.value(), it.name());
        graphViews << v;
    }

    mainWindow.initMainWindow(graphViews);
    mainWindow.show();
    return app.exec();
}
