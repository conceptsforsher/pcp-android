package chat.progressive.app.delta

import chat.progressive.app.protocol.IProtocolRoom
import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.ProtocolType

data class DeltaRoom(
    override val id: Long,
    override val title: String,
    override val lastMessage: String?,
    override val lastMessageTime: Long?,
    override val unreadCount: Int,
    override val isGroup: Boolean,
    override val avatarUrl: String?,
    override val protocolType: ProtocolType = ProtocolType.DELTACHAT,
    private val session: DeltaSession
) : IProtocolRoom {

    override suspend fun getMessages(limit: Int, fromMessageId: Long): List<IProtocolMessage> {
        return session.getMessages(id, limit)
    }

    override suspend fun sendTextMessage(text: String) {
        session.sendMessage(id, text)
    }

    override suspend fun markAsRead() {
        session.markAsRead(id)
    }

    override suspend fun leave() {
        session.leaveChat(id)
    }
}
