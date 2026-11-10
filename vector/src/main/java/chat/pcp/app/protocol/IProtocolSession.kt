package chat.pcp.app.protocol

import kotlinx.coroutines.flow.StateFlow

interface IProtocolSession {

    val protocolType: ProtocolType
    val userId: String
    val isLoggedIn: StateFlow<Boolean>
    val connectionState: StateFlow<ConnectionState>

    fun open()
    fun close()

    suspend fun sendMessage(chatId: String, text: String)

    fun getChats(): List<IProtocolRoom>
    fun getChat(chatId: String): IProtocolRoom?

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
    fun onNewMessage(chatId: String, message: IProtocolMessage) {}
    fun onChatTitleChanged(chatId: String, title: String) {}
    fun onChatListChanged() {}
    fun onError(error: ProtocolError) {}
}

data class ProtocolError(
    val code: Int,
    val message: String
)
