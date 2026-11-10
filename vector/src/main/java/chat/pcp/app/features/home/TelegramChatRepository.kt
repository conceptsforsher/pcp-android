package chat.pcp.app.features.home

import android.os.Handler
import android.os.Looper
import chat.pcp.app.native.PcpNative
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import org.json.JSONObject
import timber.log.Timber

object TelegramChatRepository {

    private val _chats = MutableStateFlow<List<TelegramChatSummary>>(emptyList())
    val chats: StateFlow<List<TelegramChatSummary>> = _chats.asStateFlow()

    private val _isConnected = MutableStateFlow(false)
    val isConnected: StateFlow<Boolean> = _isConnected.asStateFlow()

    private val _isLoggedIn = MutableStateFlow(false)
    val isLoggedIn: StateFlow<Boolean> = _isLoggedIn.asStateFlow()

    private var nativeHandle: Long = 0
    private val mainHandler = Handler(Looper.getMainLooper())
    private val chatMap = linkedMapOf<Long, TelegramChatSummary>()

    fun attach(handle: Long) {
        nativeHandle = handle
        PcpNative.tgAuthListener = authListener
        Timber.d("TelegramChatRepository attached to handle $handle")
    }

    fun detach() {
        PcpNative.tgAuthListener = null
        nativeHandle = 0
    }

    private val authListener = object : chat.pcp.app.native.TgAuthListener {
        override fun onAuthStateChanged(type: String, stateJson: String) {
            if (type == "authorizationStateReady") {
                mainHandler.post {
                    _isLoggedIn.value = true
                    loadChats()
                }
            }
        }

        override fun onConnectionStateChanged(state: String) {
            mainHandler.post {
                _isConnected.value = state == "connectionStateReady"
            }
        }
    }

    fun loadChats() {
        val h = NativeSessionManager.getHandle()
        if (h == 0L) return
        Timber.d("Loading Telegram chats...")
        PcpNative.tgLoadChats(h, 100)
    }

    fun openChat(chatId: Long) {
        val h = NativeSessionManager.getHandle()
        if (h == 0L) return
        PcpNative.tgOpenChat(h, chatId)
    }

    // Called from native via update callback bridge
    fun handleUpdate(type: String, json: String) {
        try {
            val obj = JSONObject(json)
            when (type) {
                "updateNewChat" -> {
                    val chat = obj.optJSONObject("chat") ?: return
                    val id = chat.optLong("id", 0)
                    val title = chat.optString("title", "")
                    val typeObj = chat.optJSONObject("type")
                    val isGroup = typeObj?.optString("@type", "")?.contains("Group") == true
                    val isChannel = typeObj?.optString("@type", "")?.contains("Channel") == true
                    if (title.isNotEmpty()) {
                        chatMap[id] = TelegramChatSummary(
                            id = id, title = title, lastMessage = "", lastMessageTime = 0,
                            unreadCount = 0, isGroup = isGroup, isChannel = isChannel, isPinned = false
                        )
                        emitChats()
                    }
                }
                "updateChatLastMessage" -> {
                    val chatId = obj.optLong("chat_id", 0)
                    val lastMsg = obj.optJSONObject("last_message")
                    val text = extractMessageText(lastMsg)
                    val time = lastMsg?.optLong("date", 0) ?: 0
                    chatMap[chatId]?.let {
                        chatMap[chatId] = it.copy(lastMessage = text, lastMessageTime = time)
                        emitChats()
                    }
                }
                "updateChatReadInbox" -> {
                    val chatId = obj.optLong("chat_id", 0)
                    val unread = obj.optInt("unread_count", 0)
                    chatMap[chatId]?.let {
                        chatMap[chatId] = it.copy(unreadCount = unread)
                        emitChats()
                    }
                }
                "updateChatTitle" -> {
                    val chatId = obj.optLong("chat_id", 0)
                    val title = obj.optString("title", "")
                    chatMap[chatId]?.let {
                        chatMap[chatId] = it.copy(title = title)
                        emitChats()
                    }
                }
                "updateChatIsPinned" -> {
                    val chatId = obj.optLong("chat_id", 0)
                    val pinned = obj.optBoolean("is_pinned", false)
                    chatMap[chatId]?.let {
                        chatMap[chatId] = it.copy(isPinned = pinned)
                        emitChats()
                    }
                }
            }
        } catch (e: Exception) {
            Timber.e(e, "Failed to handle Telegram update")
        }
    }

    private fun extractMessageText(msg: JSONObject?): String {
        if (msg == null) return ""
        val content = msg.optJSONObject("content") ?: return ""
        val type = content.optString("@type", "")
        return when {
            type.contains("Text") -> content.optJSONObject("text")?.optString("text", "") ?: ""
            type.contains("Photo") -> "[Photo]"
            type.contains("Video") -> "[Video]"
            type.contains("Voice") -> "[Voice]"
            type.contains("Sticker") -> "[Sticker]"
            type.contains("Document") -> "[Document]"
            type.contains("Animation") -> "[GIF]"
            type.contains("Poll") -> "[Poll]"
            type.contains("Dice") -> "[Dice]"
            type.contains("Call") -> "[Call]"
            type.contains("Location") -> "[Location]"
            else -> "[$type]"
        }
    }

    private fun emitChats() {
        val sorted = chatMap.values
            .sortedWith(compareByDescending<TelegramChatSummary> { it.isPinned }
                .thenByDescending { it.lastMessageTime })
        mainHandler.post { _chats.value = sorted }
    }
}
