#pragma once

#include "protocol_type.hpp"
#include "protocol_session.hpp"
#include <QObject>
#include <QHash>
#include <QString>
#include <memory>

class QNetworkAccessManager;

namespace pcp_chat {

class ProtocolManager : public QObject
{
    Q_OBJECT

public:
    explicit ProtocolManager(QNetworkAccessManager *network, QObject *parent = nullptr);
    ~ProtocolManager() override;

    void initialize();

    // Session management
    ProtocolSession *createSession(ProtocolType type, const QString &userId,
                                    const QString &server, const QString &token);
    void restoreSession(const QString &protocol, const QString &userId,
                        const QString &server, const QString &token);
    void removeSession(const QString &sessionId);
    ProtocolSession *session(const QString &sessionId) const;
    QList<ProtocolSession *> allSessions() const;
    QList<ProtocolSession *> sessionsByType(ProtocolType type) const;
    int sessionCount() const;

    // Unified access
    QList<ProtocolRoom> allRooms() const;
    ProtocolRoom room(const QString &roomId) const;
    void sendMessage(const QString &roomId, const QString &body);

    QNetworkAccessManager *networkManager() const { return m_networkManager; }

signals:
    void sessionCreated(ProtocolSession *session);
    void sessionRemoved(const QString &sessionId);
    void activeSessionChanged(ProtocolSession *session);
    void globalUnreadCountChanged(int count);

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QHash<QString, ProtocolSession *> m_sessions;
    ProtocolSession *m_activeSession = nullptr;
};

} // namespace pcp_chat
