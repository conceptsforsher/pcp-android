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
    JOIN,
    PART,
    QUIT,
    KICK,
    MODE,
    TOPIC,
    NICK,
    CTCP,
    SYSTEM,
    UNKNOWN
}
