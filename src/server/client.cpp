#include <QNetworkConfigurationManager>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QDebug>
#include "server/client.h"
#include "config/config.h"


JSONClient::JSONClient(const QString& hostName, const int portNum, QObject *parent) : QTcpSocket{parent}, networkSession{nullptr}, host{hostName}, port{portNum}
{
    networkSession = new QNetworkSession(QNetworkConfigurationManager().defaultConfiguration(), this);
    networkSession->open();
    connectClient();

    connect(this, &JSONClient::readyRead, this, [&]() {
        forever {
            QJsonObject jsonNode{QJsonDocument::fromJson(readLine()).object()};
            if (jsonNode.empty()) {
                break;
            }
            if (isDone(jsonNode)) {
                emit requestSuccess();
                emit requestCompleted(QJsonDocument{jsonNode}.toJson(QJsonDocument::Indented));
                break;
            } else if (isErrorReply(jsonNode)) {
                emit requestFailed();
                emit requestCompleted(QJsonDocument{jsonNode}.toJson(QJsonDocument::Indented));
                break;
            } else {
                emit jsonReady(jsonNode);
            }
            qApp->processEvents();
        }
    }, Qt::DirectConnection);
}

void JSONClient::connectClient() {
    if (state() == QTcpSocket::UnconnectedState) {
        connectToHost(host, port);
        waitForConnected();
    }
}

JSONClient* JSONClient::command(const QString &cmd) {
    write(cmd.toUtf8() + "\n");
    emit requestStarted();
    return this;
}

bool JSONClient::isErrorReply(const QJsonObject& object) const {
    return !object.toVariantMap()["error"].toString().isEmpty() ?
            !object.toVariantMap()["error"].toString().startsWith("200") : false;
}

bool JSONClient::isDone(const QJsonObject& object) const {
    return !object.toVariantMap()["info"].toString().isEmpty() ?
            object.toVariantMap()["info"].toString().startsWith("200") : false;
}

QScriptValue jsonClientToScript(QScriptEngine *engine, JSONClient* const &in) {
    return engine->newQObject(in);
}

void jsonClientFromScript(const QScriptValue &object, JSONClient* &out) {
    out = qobject_cast<JSONClient*>(object.toQObject());
}
