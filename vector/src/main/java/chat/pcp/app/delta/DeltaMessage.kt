package chat.pcp.app.delta

import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.ProtocolContentType
import chat.pcp.app.protocol.ProtocolType

data class DeltaMessage(
    override val id: Long,
    override val chatId: Long,
    override val senderId: String,
    override val senderName: String,
    override val text: String,
    override val timestamp: Long,
    override val isOutgoing: Boolean,
    override val contentType: ProtocolContentType,
    override val protocolType: ProtocolType = ProtocolType.DELTACHAT
) : IProtocolMessage
