package chat.progressive.app.xmpp

import chat.progressive.app.protocol.ConnectionState
import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.IProtocolRoom
import chat.progressive.app.protocol.IProtocolSession
import chat.progressive.app.protocol.ProtocolContentType
import chat.progressive.app.protocol.ProtocolError
import chat.progressive.app.protocol.ProtocolSessionListener
import chat.progressive.app.protocol.ProtocolType
import kotlinx.coroutines.CompletableDeferred
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONArray
import org.json.JSONObject
import timber.log.Timber

class XmppSession(
    private val nativeHandle: Long,
    private val ownJid: String
) : IProtocolSession, XmppNative.XmppCallback {

    override val protocolType: ProtocolType = ProtocolType.XMPP
    override val userId: String = ownJid

    private val _isLoggedIn = MutableStateFlow(false)
    override val isLoggedIn: StateFlow<Boolean> = _isLoggedIn

    private val _connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)
    override val connectionState: StateFlow<ConnectionState> = _connectionState

    private val listeners = mutableListOf<ProtocolSessionListener>()

    private val mamPending = mutableMapOf<String, CompletableDeferred<List<IProtocolMessage>>>()

    override fun open() {
        XmppNative.setCallback(this)
        _isLoggedIn.value = true
        _connectionState.value = ConnectionState.CONNECTING
    }

    override fun close() {
        XmppNative.setCallback(null)
        XmppNative.nativeXmppDisconnect(nativeHandle)
        _isLoggedIn.value = false
        _connectionState.value = ConnectionState.DISCONNECTED
    }

    override suspend fun sendMessage(chatId: String, text: String) {
        XmppNative.nativeXmppSend(nativeHandle, chatId, text)
    }

    fun requestHistory(chatId: String, beforeId: String = "", limit: Int = 50): CompletableDeferred<List<IProtocolMessage>> {
        val deferred = CompletableDeferred<List<IProtocolMessage>>()
        val key = "mam_$chatId"
        mamPending[key] = deferred
        XmppNative.nativeXmppRequestMam(nativeHandle, chatId, beforeId, limit)
        return deferred
    }

    fun requestHistoryPage(chatId: String, afterId: String, limit: Int = 50) {
        XmppNative.nativeXmppRequestMamPage(nativeHandle, chatId, afterId, limit)
    }

    override fun getChats(): List<IProtocolRoom> {
        val json = XmppNative.nativeXmppGetChats(nativeHandle)
        val chats = parseChatsJson(json)
        return chats.map { chat ->
            createRoom(chat)
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

    // --- XmppNative.XmppCallback ---

    override fun onConnectionChanged(json: String) {
        try {
            val obj = JSONObject(json)
            val state = obj.optString("state", "")
            if (state == "CONNECTED") {
                updateConnectionState(ConnectionState.CONNECTED)
            } else if (state == "DISCONNECTED") {
                updateConnectionState(ConnectionState.DISCONNECTED)
            }
        } catch (e: Exception) {
            Timber.e(e, "onConnectionChanged parse error")
        }
    }

    override fun onMessage(json: String) {
        val msg = parseMessageJson(json) ?: return
        listeners.forEach { it.onNewMessage(msg.chatId, msg) }
    }

    override fun onMamMessage(json: String) {
        val msg = parseMessageJson(json) ?: return
        val key = "mam_${msg.chatId}"
        mamPending[key]?.let { pending ->
            pending.complete(listOf(msg))
            mamPending.remove(key)
        }
    }

    override fun onMamComplete(json: String) {
        Timber.d("onMamComplete: %s", json)
    }

    override fun onPresence(json: String) {
        Timber.d("onPresence: %s", json)
    }

    override fun onRoster(json: String) {
        Timber.d("onRoster: %s", json)
    }

    override fun onError(json: String) {
        try {
            val obj = JSONObject(json)
            val msg = obj.optString("message", json)
            listeners.forEach { it.onError(ProtocolError(0, msg)) }
        } catch (e: Exception) {
            listeners.forEach { it.onError(ProtocolError(0, json)) }
        }
    }

    // --- Helpers ---

    private fun parseMessageJson(json: String): IProtocolMessage? {
        return try {
            val msg = JSONObject(json)
            object : IProtocolMessage {
                override val id: String = msg.optString("id", "")
                override val chatId: String = msg.getString("chatJid")
                override val senderId: String = msg.optString("senderJid", "")
                override val senderName: String = msg.optString("senderName", "")
                override val text: String = msg.optString("body", "")
                override val timestamp: Long = msg.optLong("timestamp", 0L)
                override val isOutgoing: Boolean = msg.optBoolean("isOutgoing", false)
                override val contentType: ProtocolContentType = ProtocolContentType.TEXT
                override val protocolType: ProtocolType = ProtocolType.XMPP
            }
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse XMPP message JSON")
            null
        }
    }

    private fun createRoom(chat: XmppChatData): IProtocolRoom {
        return object : IProtocolRoom {
            override val id: String = chat.jid
            override val title: String = chat.name
            override val lastMessage: String? = chat.lastMessage
            override val lastMessageTime: Long? = chat.lastMessageTime
            override val unreadCount: Int = chat.unreadCount
            override val isGroup: Boolean = chat.isGroupChat
            override val avatarUrl: String? = null
            override val protocolType: ProtocolType = ProtocolType.XMPP

            override suspend fun getMessages(limit: Int, fromMessageId: String): List<IProtocolMessage> {
                val deferred = requestHistory(id, fromMessageId, limit)
                return deferred.await()
            }

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
