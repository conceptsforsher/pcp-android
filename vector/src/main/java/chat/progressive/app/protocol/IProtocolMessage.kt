package chat.progressive.app.protocol

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
    ACTION,
    NOTICE,
    PHOTO,
    VIDEO,
    VOICE,
    DOCUMENT,
    STICKER,
    LOCATION,
    CONTACT,
    POLL,
    SYSTEM,
    PRESENCE,
    JOIN,
    PART,
    KICK,
    NICK,
    TOPIC,
    INVITE,
    UNKNOWN
}
