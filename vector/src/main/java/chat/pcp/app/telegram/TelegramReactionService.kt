package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramReactionService(private val client: TdLibClient) {

    fun setMessageReaction(
        chatId: Long,
        messageId: Long,
        reactionType: String?,
        isBig: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setMessageReaction")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("reaction_types", if (reactionType != null) {
                JSONArray().apply {
                    put(JSONObject().apply {
                        if (reactionType.startsWith("custom:")) {
                            put("@type", "reactionTypeCustomEmoji")
                            put("custom_emoji_id", reactionType.removePrefix("custom:").toLong())
                        } else {
                            put("@type", "reactionTypeEmoji")
                            put("emoji", reactionType)
                        }
                    })
                }
            } else {
                JSONArray()
            })
            put("is_big", isBig)
        })
    }

    fun addMessageReaction(
        chatId: Long,
        messageId: Long,
        reactionType: String,
        isBig: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "addMessageReaction")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("reaction_type", if (reactionType.startsWith("custom:")) {
                JSONObject().apply {
                    put("@type", "reactionTypeCustomEmoji")
                    put("custom_emoji_id", reactionType.removePrefix("custom:").toLong())
                }
            } else {
                JSONObject().apply {
                    put("@type", "reactionTypeEmoji")
                    put("emoji", reactionType)
                }
            })
            put("is_big", isBig)
        })
    }

    fun removeMessageReaction(
        chatId: Long,
        messageId: Long,
        reactionType: String
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "removeMessageReaction")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("reaction_type", if (reactionType.startsWith("custom:")) {
                JSONObject().apply {
                    put("@type", "reactionTypeCustomEmoji")
                    put("custom_emoji_id", reactionType.removePrefix("custom:").toLong())
                }
            } else {
                JSONObject().apply {
                    put("@type", "reactionTypeEmoji")
                    put("emoji", reactionType)
                }
            })
        })
    }

    fun getMessageAvailableReactions(chatId: Long, messageId: Long, rowSize: Int = 8) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageAvailableReactions")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("row_size", rowSize)
        })
    }

    fun getMessageAddedReactions(chatId: Long, messageId: Long, reactionType: String? = null, offset: String = "", limit: Int = 20) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageAddedReactions")
            put("chat_id", chatId)
            put("message_id", messageId)
            if (reactionType != null) {
                put("reaction_type", if (reactionType.startsWith("custom:")) {
                    JSONObject().apply {
                        put("@type", "reactionTypeCustomEmoji")
                        put("custom_emoji_id", reactionType.removePrefix("custom:").toLong())
                    }
                } else {
                    JSONObject().apply {
                        put("@type", "reactionTypeEmoji")
                        put("emoji", reactionType)
                    }
                })
            }
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun getDefaultReactionType() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getDefaultReactionType")
        })
    }

    fun getDefaultEmojiStatuses() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getDefaultEmojiStatuses")
        })
    }

    fun clearRecentReactions() {
        client.sendRequest(JSONObject().apply {
            put("@type", "clearRecentReactions")
        })
    }

    fun getRecentReactions() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getRecentReactions")
        })
    }

    fun setDefaultReactionType(reactionType: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setDefaultReactionType")
            put("reaction_type", if (reactionType.startsWith("custom:")) {
                JSONObject().apply {
                    put("@type", "reactionTypeCustomEmoji")
                    put("custom_emoji_id", reactionType.removePrefix("custom:").toLong())
                }
            } else {
                JSONObject().apply {
                    put("@type", "reactionTypeEmoji")
                    put("emoji", reactionType)
                }
            })
        })
    }

    fun setMessageReactions(chatId: Long, messageId: Long, available: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setMessageReactions")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("reactions_are_available", available)
        })
    }
}
