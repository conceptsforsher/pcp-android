package chat.pcp.app.features.home

import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

data class TelegramMessageItem(
    val id: Long,
    val chatId: Long,
    val text: String,
    val date: Long,
    val isOutgoing: Boolean,
    val senderId: String = ""
)

object TelegramMessageRepository {
    private val messageCache = linkedMapOf<Long, MutableList<TelegramMessageItem>>()

    fun parseMessages(chatId: Long, rawJson: String): List<TelegramMessageItem> {
        val msgs = mutableListOf<TelegramMessageItem>()
        try {
            val json = org.json.JSONObject(rawJson)
            val arr = json.optJSONArray("messages") ?: return msgs
            for (i in 0 until arr.length()) {
                val m = arr.getJSONObject(i)
                val msgId = m.optLong("id", 0)
                if (msgId <= 0) continue
                val content = m.optJSONObject("content") ?: continue
                val text = extractText(content)
                val senderObj = m.optJSONObject("sender_id")
                val senderId = senderObj?.optString("user_id", "") ?: senderObj?.optString("chat_id", "") ?: ""
                val msg = TelegramMessageItem(
                    id = msgId,
                    chatId = chatId,
                    text = text,
                    date = m.optLong("date", 0),
                    isOutgoing = m.optBoolean("is_outgoing", false),
                    senderId = senderId
                )
                if (messageCache.getOrPut(chatId) { mutableListOf() }.none { it.id == msgId }) {
                    msgs.add(msg)
                }
            }
        } catch (_: Exception) { }
        return msgs
    }

    fun parseSingleMessage(chatId: Long, rawJson: String): TelegramMessageItem? {
        try {
            val json = org.json.JSONObject(rawJson)
            val msg = json.optJSONObject("message") ?: return null
            val msgId = msg.optLong("id", 0)
            if (msgId <= 0) return null
            val content = msg.optJSONObject("content") ?: return null
            val senderObj = msg.optJSONObject("sender_id")
            val senderId = senderObj?.optString("user_id", "") ?: senderObj?.optString("chat_id", "") ?: ""
            return TelegramMessageItem(
                id = msgId,
                chatId = chatId,
                text = extractText(content),
                date = msg.optLong("date", 0),
                isOutgoing = msg.optBoolean("is_outgoing", false),
                senderId = senderId
            )
        } catch (_: Exception) { return null }
    }

    fun addMessages(chatId: Long, msgs: List<TelegramMessageItem>) {
        val list = messageCache.getOrPut(chatId) { mutableListOf() }
        for (msg in msgs) {
            if (list.none { it.id == msg.id }) {
                list.add(msg)
            }
        }
        list.sortBy { it.id }
    }

    fun addMessage(chatId: Long, msg: TelegramMessageItem) {
        addMessages(chatId, listOf(msg))
    }

    fun getMessages(chatId: Long): List<TelegramMessageItem> {
        return messageCache[chatId]?.sortedBy { it.id } ?: emptyList()
    }

    private fun extractText(content: org.json.JSONObject): String {
        val type = content.optString("@type", "")
        return when {
            type.contains("Text") -> content.optJSONObject("text")?.optString("text", "")
                ?: content.optString("text", "")
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
            else -> ""
        }
    }
}
