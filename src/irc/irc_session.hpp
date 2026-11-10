#pragma once

#include "../protocol/protocol_session.hpp"
#include <QString>
#include <QTimer>

class QNetworkAccessManager;
class QTcpSocket;

namespace pcp_chat {

class IrcSession : public ProtocolSession
{
    Q_OBJECT

public:
    explicit IrcSession(QNetworkAccessManager *network, QObject *parent = nullptr);
    ~IrcSession() override;

    void setServer(const QString &server) { m_server = server; }
    void setPort(int port) { m_port = port; }
    void setNick(const QString &nick) { m_nickname = nick; }
    void setPassword(const QString &pass) { m_password = pass; }
    void setUseSsl(bool ssl) { m_useSsl = ssl; }

    QString server() const { return m_server; }

    void connect() override;
    void disconnect() override;
    void reconnect() override;

    void sendMessage(const QString &roomId, const QString &body,
                     const QString &replyTo = "") override;
    void sendTyping(const QString &roomId, bool typing) override;
    void markRead(const QString &roomId) override;
    void joinRoom(const QString &roomId) override;
    void leaveRoom(const QString &roomId) override;

    void sendRaw(const QString &line);
    void sendPrivmsg(const QString &target, const QString &message);
    void sendNotice(const QString &target, const QString &message);
    void sendJoin(const QString &channel);
    void sendPart(const QString &channel);
    void sendNick(const QString &newNick);
    void sendWhois(const QString &nick);

signals:
    void rawLineReceived(const QString &line);
    void nickChanged(const QString &oldNick, const QString &newNick);
    void motdReceived(const QString &motd);
    void channelListReceived(const QList<QPair<QString, int>> &channels);
    void usersListReceived(const QString &channel, const QStringList &users);

private:
    void processLine(const QString &line);
    void handlePing(const QString &server);
    void handlePrivmsg(const QString &prefix, const QString &target, const QString &text);
    void handleJoin(const QString &prefix, const QString &channel);
    void handlePart(const QString &prefix, const QString &channel, const QString &reason);
    void handleNick(const QString &prefix, const QString &newNick);
    void handleTopic(const QString &prefix, const QString &channel, const QString &topic);
    void handleQuit(const QString &prefix, const QString &reason);
    void handleMode(const QString &prefix, const QString &channel,
                    const QString &modes, const QString &target);
    void handleKick(const QString &prefix, const QString &channel,
                    const QString &target, const QString &reason);
    void handleNumeric(int code, const QStringList &params);
    void startPingTimer();
    QPair<QString, QString> parsePrefix(const QString &prefix);

    QNetworkAccessManager *m_networkManager;
    QTcpSocket *m_socket = nullptr;
    QString m_server;
    int m_port = 6667;
    QString m_nickname;
    QString m_password;
    QString m_realName = "PCP User";
    bool m_useSsl = false;
    QTimer m_pingTimer;
    QString m_readBuffer;
    bool m_registered = false;
};

} // namespace pcp_chat
