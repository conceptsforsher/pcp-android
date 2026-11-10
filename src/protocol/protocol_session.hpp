#pragma once

#include "protocol_type.hpp"
#include "protocol_room.hpp"
#include "protocol_message.hpp"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

namespace pcp_chat {

class ProtocolSession : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolSession(QObject *parent = nullptr);
    ~ProtocolSession() override;

    ProtocolType protocolType() const { return m_protocolType; }
    QString sessionId() const { return m_sessionId; }
    QString userId() const { return m_userId; }
    QString displayName() const { return m_displayName; }
    bool isConnected() const { return m_connected; }

    void setSessionId(const QString &id) { m_sessionId = id; }
    void setUserId(const QString &id) { m_userId = id; }
    void setDisplayName(const QString &name) { m_displayName = name; }

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void reconnect() = 0;

    virtual void sendMessage(const QString &roomId, const QString &body,
                             const QString &replyTo = "") = 0;
    virtual void sendTyping(const QString &roomId, bool typing) = 0;
    virtual void markRead(const QString &roomId) = 0;
    virtual void joinRoom(const QString &roomId) = 0;
    virtual void leaveRoom(const QString &roomId) = 0;

    QList<ProtocolRoom> rooms() const { return m_rooms.values(); }
    ProtocolRoom room(const QString &id) const { return m_rooms.value(id); }
    bool hasRoom(const QString &id) const { return m_rooms.contains(id); }

signals:
    void connected();
    void disconnected(const QString &reason);
    void connectionError(const QString &error);
    void roomAdded(const ProtocolRoom &room);
    void roomUpdated(const ProtocolRoom &room);
    void roomRemoved(const QString &roomId);
    void messageReceived(const ProtocolMessage &message);
    void typingChanged(const QString &roomId, const QString &userId, bool typing);
    void sessionRestored(bool success);

protected:
    void addRoom(const ProtocolRoom &room);
    void updateRoom(const ProtocolRoom &room);
    void removeRoom(const QString &roomId);
    void setConnected(bool connected);
    void emitMessage(const ProtocolMessage &message);

    ProtocolType m_protocolType = ProtocolType::Unknown;
    QString m_sessionId;
    QString m_userId;
    QString m_displayName;
    bool m_connected = false;
    QHash<QString, ProtocolRoom> m_rooms;
};

} // namespace pcp_chat
