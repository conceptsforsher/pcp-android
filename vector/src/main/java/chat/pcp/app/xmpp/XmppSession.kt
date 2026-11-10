package chat.pcp.app.xmpp

import chat.pcp.app.protocol.ConnectionState
import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.IProtocolRoom
import chat.pcp.app.protocol.IProtocolSession
import chat.pcp.app.protocol.ProtocolContentType
import chat.pcp.app.protocol.ProtocolError
import chat.pcp.app.protocol.ProtocolSessionListener
import chat.pcp.app.protocol.ProtocolType
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONArray
import org.json.JSONObject
import timber.log.Timber

class XmppSession(
    private val nativeHandle: Long,
    private val ownJid: String
) : IProtocolSession {

    override val protocolType: ProtocolType = ProtocolType.XMPP
    override val userId: String = ownJid

    private val _isLoggedIn = MutableStateFlow(false)
    override val isLoggedIn: StateFlow<Boolean> = _isLoggedIn

    private val _connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)
    override val connectionState: StateFlow<ConnectionState> = _connectionState

    private val listeners = mutableListOf<ProtocolSessionListener>()

    override fun open() {
        _isLoggedIn.value = true
        _connectionState.value = ConnectionState.CONNECTING
    }

    override fun close() {
        XmppNative.nativeXmppDisconnect(nativeHandle)
        _isLoggedIn.value = false
        _connectionState.value = ConnectionState.DISCONNECTED
    }

    override suspend fun sendMessage(chatId: String, text: String) {
        XmppNative.nativeXmppSend(nativeHandle, chatId, text)
    }

    override fun getChats(): List<IProtocolRoom> {
        val json = XmppNative.nativeXmppGetChats(nativeHandle)
        val chats = parseChatsJson(json)
        return chats.map { chat ->
            object : IProtocolRoom {
                override val id: String = chat.jid
                override val title: String = chat.name
                override val lastMessage: String? = chat.lastMessage
                override val lastMessageTime: Long? = chat.lastMessageTime
                override val unreadCount: Int = chat.unreadCount
                override val isGroup: Boolean = chat.isGroupChat
                override val avatarUrl: String? = null
                override val protocolType: ProtocolType = ProtocolType.XMPP

                override suspend fun getMessages(limit: Int, fromMessageId: String): List<IProtocolMessage> = emptyList()
                override suspend fun sendTextMessage(text: String) {
                    XmppNative.nativeXmppSend(nativeHandle, id, text)
                }
                override suspend fun markAsRead() {}
                override suspend fun leave() {
                    if (isGroup) {
                        XmppNative.nativeXmppLeaveRoom(nativeHandle, id)
                    }
                }
            }
        }
    }

    override fun getChat(chatId: String): IProtocolRoom? {
        return getChats().find { it.id == chatId }
    }

    override fun addListener(listener: ProtocolSessionListener) {
        listeners.add(listener)
    }

    override fun removeListener(listener: ProtocolSessionListener) {
        listeners.remove(listener)
    }

    fun updateConnectionState(state: ConnectionState) {
        _connectionState.value = state
        listeners.forEach { it.onConnectionStateChanged(state) }
    }

    fun onMessageReceived(json: String) {
        try {
            val msg = JSONObject(json)
            val chatId = msg.getString("chatJid")
            val body = msg.getString("body")
            val message = object : IProtocolMessage {
                override val id: String = msg.optString("id", "")
                override val chatId: String = chatId
                override val senderId: String = msg.optString("senderJid", "")
                override val senderName: String = msg.optString("senderName", "")
                override val text: String = body
                override val timestamp: Long = msg.optLong("timestamp", 0L)
                override val isOutgoing: Boolean = msg.optBoolean("isOutgoing", false)
                override val contentType: ProtocolContentType = ProtocolContentType.TEXT
                override val protocolType: ProtocolType = ProtocolType.XMPP
            }
            listeners.forEach { it.onNewMessage(chatId, message) }
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse XMPP message")
        }
    }

    fun onError(msg: String) {
        listeners.forEach { it.onError(ProtocolError(0, msg)) }
    }

    private fun parseChatsJson(json: String): List<XmppChatData> {
        val arr = JSONArray(json)
        val chats = mutableListOf<XmppChatData>()
        for (i in 0 until arr.length()) {
            val obj = arr.getJSONObject(i)
            chats.add(
                XmppChatData(
                    jid = obj.getString("jid"),
                    name = obj.optString("name", obj.getString("jid")),
                    lastMessage = obj.optString("lastMessage", null),
                    lastMessageTime = obj.optLong("lastMessageTime", 0),
                    unreadCount = obj.optInt("unreadCount", 0),
                    isGroupChat = obj.optBoolean("isGroupChat", false)
                )
            )
        }
        return chats
    }

    private data class XmppChatData(
        val jid: String,
        val name: String,
        val lastMessage: String?,
        val lastMessageTime: Long,
        val unreadCount: Int,
        val isGroupChat: Boolean
    )
}
