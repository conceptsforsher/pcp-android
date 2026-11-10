#include "protocol_manager.hpp"
#include "../matrix/matrix_session.hpp"
#include "../irc/irc_session.hpp"
#include "../lemmy/lemmy_session.hpp"

#include <QNetworkAccessManager>
#include <QDebug>

namespace pcp_chat {

ProtocolManager::ProtocolManager(QNetworkAccessManager *network, QObject *parent)
    : QObject(parent)
    , m_networkManager(network)
{
}

ProtocolManager::~ProtocolManager()
{
    qDeleteAll(m_sessions);
}

void ProtocolManager::initialize()
{
    qDebug() << "ProtocolManager initialized with all protocol backends";
}

ProtocolSession *ProtocolManager::createSession(ProtocolType type,
                                                  const QString &userId,
                                                  const QString &server,
                                                  const QString &token)
{
    ProtocolSession *session = nullptr;

    switch (type) {
        case ProtocolType::Matrix: {
            auto *matrixSession = new MatrixSession(m_networkManager, this);
            matrixSession->setSessionId(QString("matrix_%1_%2").arg(userId, server));
            matrixSession->setUserId(userId);
            matrixSession->setHomeserver(server);
            matrixSession->setAccessToken(token);
            session = matrixSession;
            break;
        }
        case ProtocolType::IRC: {
            auto *ircSession = new IrcSession(m_networkManager, this);
            ircSession->setSessionId(QString("irc_%1_%2").arg(userId, server));
            ircSession->setUserId(userId);
            ircSession->setServer(server);
            session = ircSession;
            break;
        }
        case ProtocolType::Lemmy: {
            auto *lemmySession = new LemmySession(m_networkManager, this);
            lemmySession->setSessionId(QString("lemmy_%1_%2").arg(userId, server));
            lemmySession->setUserId(userId);
            lemmySession->setInstance(server);
            lemmySession->setAccessToken(token);
            session = lemmySession;
            break;
        }
        default:
            return nullptr;
    }

    m_sessions[session->sessionId()] = session;

    connect(session, &ProtocolSession::connected, this, [this, session]() {
        if (!m_activeSession) {
            m_activeSession = session;
            emit activeSessionChanged(session);
        }
    });

    connect(session, &ProtocolSession::messageReceived, this, [this](const ProtocolMessage &msg) {
        // Forward to notification manager etc.
    });

    emit sessionCreated(session);
    return session;
}

void ProtocolManager::restoreSession(const QString &protocol, const QString &userId,
                                       const QString &server, const QString &token)
{
    ProtocolType type = protocolTypeFromString(protocol);
    if (type != ProtocolType::Unknown) {
        ProtocolSession *session = createSession(type, userId, server, token);
        if (session) {
            session->connect();
        }
    }
}

void ProtocolManager::removeSession(const QString &sessionId)
{
    if (auto *session = m_sessions.take(sessionId)) {
        session->disconnect();
        delete session;
        emit sessionRemoved(sessionId);
    }
}

ProtocolSession *ProtocolManager::session(const QString &sessionId) const
{
    return m_sessions.value(sessionId);
}

QList<ProtocolSession *> ProtocolManager::allSessions() const
{
    return m_sessions.values();
}

QList<ProtocolSession *> ProtocolManager::sessionsByType(ProtocolType type) const
{
    QList<ProtocolSession *> result;
    for (auto *session : m_sessions) {
        if (session->protocolType() == type)
            result.append(session);
    }
    return result;
}

int ProtocolManager::sessionCount() const { return m_sessions.size(); }

QList<ProtocolRoom> ProtocolManager::allRooms() const
{
    QList<ProtocolRoom> rooms;
    for (auto *session : m_sessions)
        rooms.append(session->rooms());
    return rooms;
}

ProtocolRoom ProtocolManager::room(const QString &roomId) const
{
    for (auto *session : m_sessions) {
        if (session->hasRoom(roomId))
            return session->room(roomId);
    }
    return ProtocolRoom();
}

void ProtocolManager::sendMessage(const QString &roomId, const QString &body)
{
    for (auto *session : m_sessions) {
        if (session->hasRoom(roomId)) {
            session->sendMessage(roomId, body);
            return;
        }
    }
}

} // namespace pcp_chat
