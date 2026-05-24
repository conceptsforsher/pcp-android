package chat.progressive.app.protocol

import kotlinx.coroutines.flow.StateFlow

interface IProtocolSession {

    val protocolType: ProtocolType
    val userId: String
    val isLoggedIn: StateFlow<Boolean>
    val connectionState: StateFlow<ConnectionState>

    fun open()
    fun close()

    suspend fun sendMessage(chatId: Long, text: String)

    fun getChats(): List<IProtocolRoom>
    fun getChat(chatId: Long): IProtocolRoom?

    fun addListener(listener: ProtocolSessionListener)
    fun removeListener(listener: ProtocolSessionListener)
}

enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    UPDATING,
    SYNCHRONIZED
}

interface ProtocolSessionListener {
    fun onConnectionStateChanged(state: ConnectionState) {}
    fun onNewMessage(chatId: Long, message: IProtocolMessage) {}
    fun onChatTitleChanged(chatId: Long, title: String) {}
    fun onChatListChanged() {}
    fun onError(error: ProtocolError) {}
}

data class ProtocolError(
    val code: Int,
    val message: String
)
