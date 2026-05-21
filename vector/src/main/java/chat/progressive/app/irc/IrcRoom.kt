package chat.progressive.app.irc

import chat.progressive.app.protocol.ProtocolContentType
import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.IProtocolRoom
import chat.progressive.app.protocol.ProtocolType

class IrcRoom(
    override val id: String,
    override val title: String,
    override val topic: String? = null,
    override val isGroup: Boolean = true,
    private val ircClient: IrcClient
) : IProtocolRoom {

    override val lastMessage: String?
        get() = ircClient.getMessagesForChannel(id).lastOrNull()?.text

    override val lastMessageTime: Long?
        get() = ircClient.getMessagesForChannel(id).lastOrNull()?.timestamp

    override val unreadCount: Int = 0
    override val protocolType: ProtocolType = ProtocolType.IRC

    override suspend fun getMessages(limit: Int, fromMessageId: String): List<IProtocolMessage> {
        return ircClient.getMessagesForChannel(id).takeLast(limit)
    }

    override suspend fun sendTextMessage(text: String) {
        ircClient.sendMessage(id, text)
    }

    override suspend fun markAsRead() {

    }

    override suspend fun leave() {
        ircClient.part(id)
    }
}
