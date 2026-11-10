package chat.pcp.app.telegram

import chat.pcp.app.protocol.ConnectionState
import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.IProtocolRoom
import chat.pcp.app.protocol.IProtocolSession
import chat.pcp.app.protocol.ProtocolError
import chat.pcp.app.protocol.ProtocolSessionListener
import chat.pcp.app.protocol.ProtocolType
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.launch
import org.json.JSONArray
import org.json.JSONObject
import timber.log.Timber

class TelegramSession(
    private val tdClient: TdLibClient
) : IProtocolSession, TelegramUpdateListener {

    override val protocolType: ProtocolType = ProtocolType.TELEGRAM

    private val scope = CoroutineScope(SupervisorJob() + Dispatchers.IO)

    private val _isLoggedIn = MutableStateFlow(false)
    override val isLoggedIn: StateFlow<Boolean> = _isLoggedIn

    private val _connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)
    override val connectionState: StateFlow<ConnectionState> = _connectionState

    override val userId: String get() = tdClient.userId

    private val listeners = mutableListOf<ProtocolSessionListener>()
    private val chats = mutableMapOf<Long, IProtocolRoom>()

    val stickerService = TelegramStickerService(tdClient)
    val pollService = TelegramPollService(tdClient)
    val reactionService = TelegramReactionService(tdClient)
    val secretChatService = TelegramSecretChatService(tdClient)
    val forwardService = TelegramForwardService(tdClient)
    val botService = TelegramBotService(tdClient)
    val folderService = TelegramFolderService(tdClient)
    val mediaService = TelegramMediaService(tdClient)
    val fileService = TelegramFileService(tdClient)
    val scheduledService = TelegramScheduledService(tdClient)
    val locationService = TelegramLocationService(tdClient)
    val proxyService = TelegramProxyService(tdClient)
    val storyService = TelegramStoryService(tdClient)
    val profileService = TelegramProfileService(tdClient)
    val threadService = TelegramThreadService(tdClient)

    init {
        tdClient.addListener(this)
    }

    override fun open() {
        tdClient.initialize()
    }

    override fun close() {
        tdClient.removeListener(this)
        tdClient.close()
    }

    override suspend fun sendMessage(chatId: Long, text: String) {
        tdClient.sendChatMessage(chatId, text)
    }

    override fun getChats(): List<IProtocolRoom> {
        return chats.values.toList()
    }

    override fun getChat(chatId: Long): IProtocolRoom? {
        return chats[chatId]
    }

    override fun addListener(listener: ProtocolSessionListener) {
        listeners.add(listener)
    }

    override fun removeListener(listener: ProtocolSessionListener) {
        listeners.remove(listener)
    }

    override fun onConnectionStateChanged(state: chat.pcp.app.telegram.ConnectionState) {
        _connectionState.value = when (state) {
            chat.pcp.app.telegram.ConnectionState.CONNECTED -> ConnectionState.CONNECTED
            chat.pcp.app.telegram.ConnectionState.CONNECTING -> ConnectionState.CONNECTING
            chat.pcp.app.telegram.ConnectionState.UPDATING -> ConnectionState.UPDATING
            chat.pcp.app.telegram.ConnectionState.DISCONNECTED -> ConnectionState.DISCONNECTED
        }
        listeners.forEach { it.onConnectionStateChanged(connectionState.value) }
    }

    override fun onNewMessage(chatId: Long, messageId: Long, rawJson: String) {
        try {
            val msg = parseMessageJson(JSONObject(rawJson))
            if (msg != null) {
                listeners.forEach { it.onNewMessage(chatId, msg) }
            }
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse message")
        }
    }

    override fun onChatTitleChanged(chatId: Long, title: String) {
        listeners.forEach { it.onChatTitleChanged(chatId, title) }
    }

    override fun onChatListChanged() {
        listeners.forEach { it.onChatListChanged() }
    }

    fun handleTdLibResponse(type: String, json: JSONObject) {
        when (type) {
            "chats" -> parseChatList(json)
            "chat" -> parseSingleChat(json)
            "messages" -> parseMessages(json)
            "user" -> parseUser(json)
            "authorizationStateReady" -> {
                _isLoggedIn.value = true
                tdClient.loadChats()
            }
        }
    }

    private fun parseChatList(json: JSONObject) {
        val chatIds = json.optJSONArray("chat_ids") ?: return
        // Request chat info for each chat
    }

    private fun parseSingleChat(json: JSONObject) {
        val chatId = json.optLong("id", 0)
        val title = json.optString("title", "")
        val lastMsg = json.optJSONObject("last_message")
        val unreadCount = json.optInt("unread_count", 0)
        val isGroup = json.optJSONObject("type")?.optString("@type")?.contains("Group") == true

        val lastMessageText = lastMsg?.optJSONObject("content")?.let { extractText(it) }
        val lastMessageTime = lastMsg?.optLong("date", 0)?.takeIf { it > 0 }

        val room = TelegramRoom(
            id = chatId,
            title = title,
            lastText = lastMessageText,
            lastTime = lastMessageTime,
            unread = unreadCount,
            isGroup = isGroup,
            tdClient = tdClient
        )
        chats[chatId] = room
    }

    private fun parseMessages(json: JSONObject) {
        val messages = json.optJSONArray("messages") ?: return
        for (i in 0 until messages.length()) {
            val msgJson = messages.getJSONObject(i)
            val msg = parseMessageJson(msgJson)
            if (msg != null) {
                val chatId = msg.chatId
                listeners.forEach { it.onNewMessage(chatId, msg) }
            }
        }
    }

    private fun parseMessageJson(msgJson: JSONObject): TelegramMessage? {
        val id = msgJson.optLong("id", 0)
        val chatId = msgJson.optLong("chat_id", 0)
        val date = msgJson.optLong("date", 0)
        val senderObj = msgJson.optJSONObject("sender_id")
        val senderId = senderObj?.optString("user_id", "0") ?: "0"
        val isOutgoing = msgJson.optBoolean("is_outgoing", false)
        val content = msgJson.optJSONObject("content") ?: return null

        val (contentType, text) = parseContent(content)

        return TelegramMessage(
            id = id,
            chatId = chatId,
            senderId = senderId,
            senderName = senderId,
            text = text,
            timestamp = date,
            isOutgoing = isOutgoing,
            contentType = contentType
        )
    }

    private fun parseContent(content: JSONObject): Pair<chat.pcp.app.protocol.ProtocolContentType, String> {
        val contentType = content.optString("@type", "")
        return when {
            contentType.contains("Text") || contentType.contains("text") -> {
                val textObj = content.optJSONObject("text")
                val text = textObj?.optString("text", "") ?: content.optString("text", "")
                Pair(chat.pcp.app.protocol.ProtocolContentType.TEXT, text)
            }
            contentType.contains("Photo") -> Pair(chat.pcp.app.protocol.ProtocolContentType.PHOTO, "[Photo]")
            contentType.contains("Video") -> Pair(chat.pcp.app.protocol.ProtocolContentType.VIDEO, "[Video]")
            contentType.contains("Voice") -> Pair(chat.pcp.app.protocol.ProtocolContentType.VOICE, "[Voice]")
            contentType.contains("Document") -> Pair(chat.pcp.app.protocol.ProtocolContentType.DOCUMENT, "[Document]")
            contentType.contains("Sticker") -> Pair(chat.pcp.app.protocol.ProtocolContentType.STICKER, "[Sticker]")
            contentType.contains("Location") -> Pair(chat.pcp.app.protocol.ProtocolContentType.LOCATION, "[Location]")
            contentType.contains("Contact") -> Pair(chat.pcp.app.protocol.ProtocolContentType.CONTACT, "[Contact]")
            contentType.contains("Poll") -> Pair(chat.pcp.app.protocol.ProtocolContentType.POLL, "[Poll]")
            else -> Pair(chat.pcp.app.protocol.ProtocolContentType.UNKNOWN, "[Unknown]")
        }
    }

    private fun extractText(content: JSONObject): String? {
        return when {
            content.optString("@type", "").contains("Text") -> {
                content.optJSONObject("text")?.optString("text")
            }
            else -> "[Media]"
        }
    }

    private fun parseUser(json: JSONObject) {
        val userId = json.optLong("id", 0)
        val firstName = json.optString("first_name", "")
        val lastName = json.optString("last_name", "")
        val username = json.optString("username", "")
        val phone = json.optString("phone_number", "")

        if (json.optString("@type", "") == "user" && username.isNotEmpty()) {
            tdClient._userId = username
        }
    }

    private fun JSONArray.toList(): List<Any> {
        val result = mutableListOf<Any>()
        for (i in 0 until length()) {
            result.add(get(i))
        }
        return result
    }
}
