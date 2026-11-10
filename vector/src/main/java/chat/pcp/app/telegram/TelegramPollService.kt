package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramPollService(private val client: TdLibClient) {

    fun createPoll(
        chatId: Long,
        question: String,
        options: List<String>,
        isAnonymous: Boolean = true,
        isQuiz: Boolean = false,
        correctOptionId: Int = -1,
        allowMultipleAnswers: Boolean = false,
        closeDate: Long = 0,
        isClosed: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessagePoll")
                put("question", question)
                put("options", JSONArray(options))
                put("is_anonymous", isAnonymous)
                put("type", if (isQuiz)
                    JSONObject().apply {
                        put("@type", "pollTypeQuiz")
                        put("correct_option_id", correctOptionId)
                    }
                else
                    JSONObject().apply {
                        put("@type", "pollTypeRegular")
                        put("allow_multiple_answers", allowMultipleAnswers)
                    }
                )
                if (closeDate > 0) put("close_date", closeDate)
                put("is_closed", isClosed)
            })
        })
    }

    fun setPollVote(chatId: Long, messageId: Long, optionIds: IntArray) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setPollAnswer")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("option_ids", JSONArray(optionIds.toList()))
        })
    }

    fun getPollResults(chatId: Long, messageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getPollVoters")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("option_id", 0)
            put("offset", 0)
            put("limit", 50)
        })
    }

    fun stopPoll(chatId: Long, messageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "stopPoll")
            put("chat_id", chatId)
            put("message_id", messageId)
        })
    }
}
