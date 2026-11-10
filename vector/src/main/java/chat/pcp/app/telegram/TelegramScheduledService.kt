package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramScheduledService(private val client: TdLibClient) {

    fun sendMessageScheduled(chatId: Long, text: String, scheduleDate: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageText")
                put("text", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", text)
                })
            })
            put("scheduling_state", JSONObject().apply {
                put("@type", "messageSchedulingStateSendAtDate")
                put("send_date", scheduleDate)
            })
        })
    }

    fun getScheduledMessages(chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getScheduledMessages")
            put("chat_id", chatId)
        })
    }

    fun rescheduleMessage(chatId: Long, messageId: Long, newDate: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "editMessageSchedulingState")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("scheduling_state", JSONObject().apply {
                put("@type", "messageSchedulingStateSendAtDate")
                put("send_date", newDate)
            })
        })
    }

    fun deleteScheduledMessage(chatId: Long, messageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteMessages")
            put("chat_id", chatId)
            put("message_ids", org.json.JSONArray(listOf(messageId)))
            put("revoke", true)
        })
    }

    fun sendWhenOnline(chatId: Long, text: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageText")
                put("text", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", text)
                })
            })
            put("scheduling_state", JSONObject().apply {
                put("@type", "messageSchedulingStateSendWhenOnline")
            })
        })
    }
}
