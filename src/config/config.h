#ifndef CONFIG_H
#define CONFIG_H

#include <QScriptValue>
#include <QScriptEngine>
#include "server/client.h"


namespace Config {
    QScriptValue readConfig(QScriptEngine& engine);
    QScriptValue include_js(QScriptEngine& scriptEngine, const QString& fileName);
}


namespace Constants {
    extern const QString COMMON_JS_FILE;
    extern const QString CONFIG_JS_FILE;
    extern const QString VIEW_JS_FILE;

    extern const QString NODE_ID_FIELD;
    extern const QString NODE_TYPE_FIELD;
    extern const QString NODE_PID_FIELD;

    extern const QString LINK_NODE;
}

#endif // CONFIG_H
