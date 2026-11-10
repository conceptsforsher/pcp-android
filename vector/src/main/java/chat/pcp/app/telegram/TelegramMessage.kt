package chat.pcp.app.telegram

import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.ProtocolContentType
import chat.pcp.app.protocol.ProtocolType

data class TelegramMessage(
    override val id: Long,
    override val chatId: Long,
    override val senderId: String,
    override val senderName: String,
    override val text: String,
    override val timestamp: Long,
    override val isOutgoing: Boolean,
    override val contentType: ProtocolContentType = ProtocolContentType.TEXT
) : IProtocolMessage {
    override val protocolType: ProtocolType = ProtocolType.TELEGRAM
}

data class TelegramChatInfo(
    val id: Long,
    val title: String,
    val lastMessage: String?,
    val lastMessageTime: Long?,
    val unreadCount: Int,
    val isGroup: Boolean,
    val avatarPath: String?
)

data class TelegramUser(
    val id: Long,
    val firstName: String,
    val lastName: String,
    val phoneNumber: String?,
    val username: String?
)

data class TelegramAuthState(
    val state: AuthStep,
    val codeInfo: String? = null,
    val hint: String? = null
)

enum class AuthStep {
    WAITING_PHONE_NUMBER,
    WAITING_CODE,
    WAITING_PASSWORD,
    READY,
    CLOSED,
    ERROR
}
