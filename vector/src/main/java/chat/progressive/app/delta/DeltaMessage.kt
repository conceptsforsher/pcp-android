package chat.progressive.app.delta

import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.ProtocolContentType
import chat.progressive.app.protocol.ProtocolType

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
