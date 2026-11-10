#pragma once

#include "../protocol/protocol_session.hpp"
#include <QString>
#include <QTimer>

class QNetworkAccessManager;

namespace pcp_chat {

class MatrixSession : public ProtocolSession
{
    Q_OBJECT

public:
    explicit MatrixSession(QNetworkAccessManager *network, QObject *parent = nullptr);
    ~MatrixSession() override;

    void setHomeserver(const QString &url);
    void setAccessToken(const QString &token);
    void setDeviceId(const QString &id);

    QString homeserver() const { return m_homeserver; }
    QString accessToken() const { return m_accessToken; }

    // ProtocolSession interface
    void connect() override;
    void disconnect() override;
    void reconnect() override;

    void sendMessage(const QString &roomId, const QString &body,
                     const QString &replyTo = "") override;
    void sendTyping(const QString &roomId, bool typing) override;
    void markRead(const QString &roomId) override;
    void joinRoom(const QString &roomId) override;
    void leaveRoom(const QString &roomId) override;

    // Matrix-specific
    void sync();
    void sendReadReceipt(const QString &roomId, const QString &eventId);
    void sendReadMarkers(const QString &roomId, const QString &fullyRead, const QString &read);
    void sendReceipt(const QString &roomId, const QString &eventId,
                     const QString &receiptType = "m.read");

signals:
    void syncCompleted(const QString &nextBatch);
    void syncError(const QString &error);
    void homeserverResolved(const QString &url);

private:
    void performServerDiscovery();
    void performLogin();
    void startSyncLoop();
    void processSyncResponse(const QByteArray &data);
    void processRoomsData(const QJsonObject &rooms);

    QNetworkAccessManager *m_networkManager;
    QString m_homeserver;
    QString m_accessToken;
    QString m_deviceId;
    QString m_nextBatch;
    QString m_filterId;
    QTimer m_syncTimer;
    int m_syncTimeoutSecs = 30;
    bool m_initialSyncDone = false;
};

} // namespace pcp_chat
