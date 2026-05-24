package chat.progressive.app.protocol

interface IProtocolRoom {
    val id: Long
    val title: String
    val lastMessage: String?
    val lastMessageTime: Long?
    val unreadCount: Int
    val isGroup: Boolean
    val avatarUrl: String?
    val protocolType: ProtocolType

    suspend fun getMessages(limit: Int, fromMessageId: Long): List<IProtocolMessage>
    suspend fun sendTextMessage(text: String)
    suspend fun markAsRead()
    suspend fun leave()
}
