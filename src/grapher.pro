QT += widgets network script


HEADERS += graph/node.h \
    graph/view.h \
    config/config.h \
    mainwindow/mainwindow.h \
    graph/graph.h \
    server/client.h \
    graph/scene.h \
    graph/tooltip.h

SOURCES += main.cpp \
    graph/node.cpp \
    graph/view.cpp \
    config/config.cpp \
    mainwindow/mainwindow.cpp \
    graph/graph.cpp \
    server/client.cpp \
    graph/scene.cpp \
    graph/tooltip.cpp

ICON = icons/grapher.png

STATECHARTS +=

FORMS += \
    mainwindow/toolpage.ui \
    mainwindow/mainwindow.ui

RESOURCES += icons.qrc

DISTFILES += \
    ../js/config.js \
    ../js/common.js
