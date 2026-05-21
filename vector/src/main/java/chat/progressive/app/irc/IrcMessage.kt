package chat.progressive.app.irc

import chat.progressive.app.protocol.ProtocolContentType
import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.ProtocolType

data class IrcMessage(
    override val id: String,
    override val chatId: String,
    override val senderId: String,
    override val senderName: String,
    override val text: String,
    override val timestamp: Long = System.currentTimeMillis(),
    override val isOutgoing: Boolean = false,
    override val contentType: ProtocolContentType = ProtocolContentType.TEXT
) : IProtocolMessage {
    override val protocolType: ProtocolType = ProtocolType.IRC
}

data class IrcServerInfo(
    val host: String,
    val port: Int = 6667,
    val tls: Boolean = false,
    val nick: String,
    val user: String = "",
    val realName: String = "",
    val password: String = ""
)
