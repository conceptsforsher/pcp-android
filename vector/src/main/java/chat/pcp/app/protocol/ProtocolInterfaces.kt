package chat.pcp.app.protocol

enum class ProtocolType(val id: String, val displayName: String) {
    MATRIX("matrix", "Matrix"),
    IRC("irc", "IRC"),
    REDDIT("reddit", "Reddit");

    companion object {
        fun fromId(id: String): ProtocolType =
            entries.find { it.id == id } ?: MATRIX
    }
}

enum class ConnectionState { DISCONNECTED, CONNECTING, CONNECTED, ERROR }

enum class ProtocolContentType { TEXT, SYSTEM, UNKNOWN }

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

interface IProtocolRoom {
    val id: String
    val title: String
    val topic: String
    val lastMessage: String?
    val lastMessageTime: Long
    val unreadCount: Int
    val isGroup: Boolean
    val protocolType: ProtocolType
    fun getMessages(limit: Int = 50, fromMessageId: String? = null): List<IProtocolMessage>
    fun sendTextMessage(text: String)
    fun markAsRead()
    fun leave()
}

interface IProtocolSession {
    val protocolType: ProtocolType
    val userId: String
    val isLoggedIn: kotlinx.coroutines.flow.StateFlow<Boolean>
    val connectionState: kotlinx.coroutines.flow.StateFlow<ConnectionState>
    fun open()
    fun close()
    fun sendMessage(chatId: String, text: String)
    fun getChats(): List<IProtocolRoom>
    fun getChat(chatId: String): IProtocolRoom?
    fun addListener(listener: ProtocolSessionListener)
    fun removeListener(listener: ProtocolSessionListener)
}

interface ProtocolSessionListener {
    fun onConnectionStateChanged(state: ConnectionState)
    fun onNewMessage(chatId: String, message: IProtocolMessage)
    fun onChatTitleChanged(chatId: String, title: String)
    fun onChatListChanged()
    fun onError(error: ProtocolError)
}

data class ProtocolError(val code: Int, val message: String)
