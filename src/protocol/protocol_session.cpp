#include "protocol_session.hpp"

namespace pcp_chat {

ProtocolSession::ProtocolSession(QObject *parent)
    : QObject(parent)
{
}

ProtocolSession::~ProtocolSession() = default;

void ProtocolSession::addRoom(const ProtocolRoom &room)
{
    m_rooms[room.id] = room;
    emit roomAdded(room);
}

void ProtocolSession::updateRoom(const ProtocolRoom &room)
{
    m_rooms[room.id] = room;
    emit roomUpdated(room);
}

void ProtocolSession::removeRoom(const QString &roomId)
{
    m_rooms.remove(roomId);
    emit roomRemoved(roomId);
}

void ProtocolSession::setConnected(bool connected)
{
    if (m_connected != connected) {
        m_connected = connected;
        if (connected)
            emit connected();
        else
            emit disconnected("");
    }
}

void ProtocolSession::emitMessage(const ProtocolMessage &message)
{
    emit messageReceived(message);
}

} // namespace pcp_chat
