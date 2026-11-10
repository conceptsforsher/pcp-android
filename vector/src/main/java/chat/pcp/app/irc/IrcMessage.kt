package chat.pcp.app.irc

import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.ProtocolContentType
import chat.pcp.app.protocol.ProtocolType

data class IrcMessage(
    override val id: String,
    override val chatId: String,
    override val senderId: String,
    override val senderName: String,
    override val text: String,
    override val timestamp: Long = 0,
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
