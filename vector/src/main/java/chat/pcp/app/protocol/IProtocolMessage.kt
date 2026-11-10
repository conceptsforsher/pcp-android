package chat.pcp.app.protocol

interface IProtocolMessage {
    val id: String
    val chatId: String
    val senderId: String
    val senderName: String
    val text: String
    val timestamp: Long
    val isOutgoing: Boolean
    val contentType: ProtocolContentType
    val protocolType: ProtocolType
}

enum class ProtocolContentType {
    TEXT,
    PHOTO,
    VIDEO,
    VOICE,
    DOCUMENT,
    STICKER,
    LOCATION,
    CONTACT,
    POLL,
    SYSTEM,
    UNKNOWN
}
