#include <QDebug>
#include <QFile>
#include "config/config.h"


QScriptValue Config::include_js(QScriptEngine& scriptEngine, const QString& name) {
    QFile scriptFile(name);
    QScriptValue result;
    if (scriptFile.open(QIODevice::ReadOnly)) {
        result = scriptEngine.evaluate(scriptFile.readAll()).toString();
        if (scriptEngine.hasUncaughtException()) {
              qDebug() << "uncaught exception at line" << scriptEngine.uncaughtExceptionLineNumber()
                       << ":" << result.toString();
        }
    }
    scriptFile.close();
    return result;
}

QScriptValue Config::readConfig(QScriptEngine& engine) {
    engine.globalObject().setProperty("config", QScriptValue());
    Config::include_js(engine, Constants::CONFIG_JS_FILE);
    return engine.globalObject().property("config");
}


const QString Constants::COMMON_JS_FILE{"./js/common.js"};
const QString Constants::CONFIG_JS_FILE{"./js/config.js"};
const QString Constants::VIEW_JS_FILE{"./js/%1.js"};

const QString Constants::NODE_ID_FIELD{"id"};
const QString Constants::NODE_TYPE_FIELD{"type"};
const QString Constants::NODE_PID_FIELD{"pid"};

const QString Constants::LINK_NODE{"link"};
