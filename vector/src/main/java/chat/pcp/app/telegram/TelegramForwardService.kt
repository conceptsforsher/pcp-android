package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramForwardService(private val client: TdLibClient) {

    fun forwardMessages(
        chatId: Long,
        fromChatId: Long,
        messageIds: LongArray,
        sendCopy: Boolean = false,
        removeCaption: Boolean = false,
        onlyPreview: Boolean = false
    ) {
        val inputContent = if (onlyPreview) {
            JSONObject().apply { put("@type", "inputMessageForwarded") }
        } else if (sendCopy) {
            null
        } else {
            JSONObject().apply { put("@type", "inputMessageForwarded") }
        }

        client.sendRequest(JSONObject().apply {
            put("@type", "forwardMessages")
            put("chat_id", chatId)
            put("from_chat_id", fromChatId)
            put("message_ids", org.json.JSONArray(messageIds.toList()))
            put("send_copy", sendCopy)
            put("remove_caption", removeCaption)
            if (!onlyPreview) {
                put("options", JSONObject().apply {
                    put("send_copy", sendCopy)
                    put("remove_caption", removeCaption)
                })
            }
        })
    }

    fun getMessageLink(chatId: Long, messageId: Long, mediaTimestamp: Int = 0, forAlbum: Boolean = false, forComment: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageLink")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("media_timestamp", mediaTimestamp)
            put("for_album", forAlbum)
            put("for_comment", forComment)
        })
    }

    fun getMessageLinkInfo(url: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageLinkInfo")
            put("url", url)
        })
    }

    fun getMessagePublicForwards(
        chatId: Long,
        messageId: Long,
        offset: String = "",
        limit: Int = 50
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessagePublicForwards")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun getMessageStatistics(chatId: Long, messageId: Long, isDark: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageStatistics")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("is_dark", isDark)
        })
    }
}
