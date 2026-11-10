package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramBotService(private val client: TdLibClient) {

    fun getBotInfo(botUserId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getUser")
            put("user_id", botUserId)
        })
    }

    fun getBotCommands(botUserId: Long, chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getCommands")
            put("scope", JSONObject().apply {
                put("@type", "botCommandScopePeer")
                put("peer", JSONObject().apply {
                    put("@type", "inputPeerUser")
                    put("user_id", botUserId)
                })
            })
            put("language_code", "")
        })
    }

    fun setBotCommands(botUserId: Long, commands: List<TelegramBotCommand>, scope: String = "default") {
        client.sendRequest(JSONObject().apply {
            put("@type", "setCommands")
            put("scope", when (scope) {
                "all_group" -> JSONObject().apply { put("@type", "botCommandScopeAllGroupChats") }
                "all_private" -> JSONObject().apply { put("@type", "botCommandScopeAllPrivateChats") }
                else -> JSONObject().apply { put("@type", "botCommandScopeDefault") }
            })
            put("language_code", "")
            put("commands", org.json.JSONArray(commands.map { cmd ->
                JSONObject().apply {
                    put("command", cmd.command)
                    put("description", cmd.description)
                }
            }))
        })
    }

    fun getInlineQueryResults(
        botUserId: Long,
        chatId: Long,
        query: String,
        offset: String = ""
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getInlineQueryResults")
            put("bot_user_id", botUserId)
            put("chat_id", chatId)
            put("user_location", JSONObject().apply { put("@type", "location") })
            put("query", query)
            put("offset", offset)
        })
    }

    fun sendInlineQueryResult(
        chatId: Long,
        messageThreadId: Long = 0,
        queryId: Long,
        resultId: String,
        hideViaBot: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendInlineQueryResultMessage")
            put("chat_id", chatId)
            put("message_thread_id", messageThreadId)
            put("query_id", queryId)
            put("result_id", resultId)
            put("hide_via_bot", hideViaBot)
        })
    }

    fun answerCallbackQuery(
        callbackQueryId: Long,
        text: String? = null,
        showAlert: Boolean = false,
        url: String? = null,
        cacheTime: Int = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "answerCallbackQuery")
            put("callback_query_id", callbackQueryId)
            if (text != null) put("text", text)
            put("show_alert", showAlert)
            if (url != null) put("url", url)
            put("cache_time", cacheTime)
        })
    }

    fun answerShippingQuery(
        shippingQueryId: Long,
        shippingOptions: List<ShippingOption>? = null,
        errorMessage: String? = null
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "answerShippingQuery")
            put("shipping_query_id", shippingQueryId)
            if (shippingOptions != null) {
                put("shipping_options", org.json.JSONArray(shippingOptions.map { opt ->
                    JSONObject().apply {
                        put("id", opt.id)
                        put("title", opt.title)
                        put("prices", org.json.JSONArray(opt.prices.map { price ->
                            JSONObject().apply {
                                put("label", price.label)
                                put("amount", price.amount)
                            }
                        }))
                    }
                }))
            }
            if (errorMessage != null) put("error_message", errorMessage)
        })
    }

    data class ShippingOption(
        val id: String,
        val title: String,
        val prices: List<ShippingPrice>
    )
    data class ShippingPrice(val label: String, val amount: Long)

    fun getCallbackQueryAnswer(callbackQueryId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getCallbackQueryAnswer")
            put("callback_query_id", callbackQueryId)
        })
    }

    fun toggleBotIsPaused(botUserId: Long, isPaused: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "toggleBotIsPaused")
            put("bot_user_id", botUserId)
            put("is_paused", isPaused)
        })
    }

    fun setBotUpdatesStatus(pendingUpdateCount: Int, errorMessage: String = "") {
        client.sendRequest(JSONObject().apply {
            put("@type", "setBotUpdatesStatus")
            put("pending_update_count", pendingUpdateCount)
            put("error_message", errorMessage)
        })
    }

    fun canBotSendMessages(botUserId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "canBotSendMessages")
            put("bot_user_id", botUserId)
        })
    }
}
