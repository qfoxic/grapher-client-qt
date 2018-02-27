#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QNetworkSession>
#include <QScriptValue>


class JSONClient : public QTcpSocket {
    Q_OBJECT

public:
    explicit JSONClient(const QString&host, const int port, QObject* parent = nullptr);
    inline virtual ~JSONClient() { delete networkSession; }

public slots:
    JSONClient *command(const QString &cmd);
    void connectClient();

private:
    bool isErrorReply(const QJsonObject& object) const;
    bool isDone(const QJsonObject& object) const;

signals:
    void requestStarted() const;
    void requestSuccess() const;
    void requestFailed() const;
    void requestCompleted(const QString&) const;
    void jsonReady(QJsonObject&) const;

private:
    Q_DISABLE_COPY(JSONClient)
    JSONClient& operator=(JSONClient&&) = delete;
    JSONClient(JSONClient&&) = delete;

private:
    QNetworkSession* networkSession;
    QString host;
    int port;
};

Q_DECLARE_METATYPE(JSONClient*)

QScriptValue jsonClientToScript(QScriptEngine *engine, JSONClient* const &in);
void jsonClientFromScript(const QScriptValue &object, JSONClient *&out);

#endif
