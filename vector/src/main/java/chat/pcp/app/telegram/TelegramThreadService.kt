package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramThreadService(private val client: TdLibClient) {

    fun getMessageThread(chatId: Long, messageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageThread")
            put("chat_id", chatId)
            put("message_id", messageId)
        })
    }

    fun getMessageThreadHistory(
        chatId: Long,
        messageId: Long,
        fromMessageId: Long = 0,
        offset: Int = 0,
        limit: Int = 50
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageThreadHistory")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("from_message_id", fromMessageId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun sendThreadReply(
        chatId: Long,
        threadRootMessageId: Long,
        text: String,
        replyToMessageId: Long = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
            if (replyToMessageId > 0) {
                put("reply_to", JSONObject().apply {
                    put("@type", "inputMessageReplyTo")
                    put("message_id", replyToMessageId)
                })
            }
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageText")
                put("text", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", text)
                })
            })
        })
    }

    fun sendThreadPhoto(chatId: Long, threadRootMessageId: Long, filePath: String, caption: String? = null) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessagePhoto")
                put("photo", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                if (caption != null) {
                    put("caption", JSONObject().apply {
                        put("@type", "formattedText")
                        put("text", caption)
                    })
                }
            })
        })
    }

    fun reactToThreadMessage(
        chatId: Long,
        threadRootMessageId: Long,
        messageId: Long,
        reaction: String,
        isBig: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "addMessageReaction")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("reaction_type", if (reaction.startsWith("custom:")) {
                JSONObject().apply {
                    put("@type", "reactionTypeCustomEmoji")
                    put("custom_emoji_id", reaction.removePrefix("custom:").toLong())
                }
            } else {
                JSONObject().apply {
                    put("@type", "reactionTypeEmoji")
                    put("emoji", reaction)
                }
            })
        })
    }

    fun forwardToThread(
        chatId: Long,
        threadRootMessageId: Long,
        fromChatId: Long,
        messageIds: LongArray
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "forwardMessages")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
            put("from_chat_id", fromChatId)
            put("message_ids", JSONArray(messageIds.toList()))
        })
    }

    fun searchInThread(
        chatId: Long,
        threadRootMessageId: Long,
        query: String,
        limit: Int = 50,
        fromMessageId: Long = 0,
        offset: Int = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchChatMessages")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
            put("query", query)
            put("limit", limit)
            put("from_message_id", fromMessageId)
            put("offset", offset)
            put("filter", JSONObject().apply { put("@type", "searchMessagesFilterEmpty") })
        })
    }

    fun getForumTopics(
        chatId: Long,
        query: String = "",
        offset: Long = 0,
        limit: Int = 100
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getForumTopics")
            put("chat_id", chatId)
            put("query", query)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun getForumTopic(chatId: Long, topicId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getForumTopic")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
        })
    }

    fun createForumTopic(
        chatId: Long,
        name: String,
        iconColor: Int = 0x6FB9F0,
        iconCustomEmojiId: Long = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "createForumTopic")
            put("chat_id", chatId)
            put("name", name)
            val icon = JSONObject().apply {
                put("@type", "forumTopicIcon")
                put("color", iconColor)
                if (iconCustomEmojiId > 0) {
                    put("custom_emoji_id", iconCustomEmojiId)
                }
            }
            put("icon", icon)
        })
    }

    fun editForumTopic(
        chatId: Long,
        topicId: Long,
        name: String? = null,
        editIcon: Boolean = false,
        iconCustomEmojiId: Long = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "editForumTopic")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
            if (name != null) put("name", name)
            if (editIcon) {
                put("edit_icon_custom_emoji", true)
                put("icon_custom_emoji_id", iconCustomEmojiId)
            }
        })
    }

    fun deleteForumTopic(chatId: Long, topicId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteForumTopic")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
        })
    }

    fun toggleForumTopicIsClosed(chatId: Long, topicId: Long, isClosed: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "toggleForumTopicIsClosed")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
            put("is_closed", isClosed)
        })
    }

    fun toggleForumTopicIsPinned(chatId: Long, topicId: Long, isPinned: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "toggleForumTopicIsPinned")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
            put("is_pinned", isPinned)
        })
    }

    fun toggleForumTopicIsHidden(chatId: Long, topicId: Long, isHidden: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "toggleForumTopicIsHidden")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
            put("is_hidden", isHidden)
        })
    }

    fun setForumTopicNotificationSettings(
        chatId: Long,
        topicId: Long,
        settings: TelegramNotificationSettings
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setForumTopicNotificationSettings")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
            put("notification_settings", settingsToJson(settings))
        })
    }

    fun getForumTopicDefaultIcons() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getForumTopicDefaultIcons")
        })
    }

    fun getForumTopicLink(chatId: Long, topicId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getForumTopicLink")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
        })
    }

    fun pinThreadMessage(
        chatId: Long,
        threadRootMessageId: Long,
        messageId: Long,
        disableNotification: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "pinChatMessage")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("message_thread_id", threadRootMessageId)
            put("disable_notification", disableNotification)
            put("only_for_self", false)
        })
    }

    fun unpinThreadMessage(chatId: Long, threadRootMessageId: Long, messageId: Long = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "unpinChatMessage")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun readAllThreadMessages(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "readAllMessageThreadMessages")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun readAllThreadMentions(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "readAllMessageThreadMentions")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun readAllThreadReactions(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "readAllMessageThreadReactions")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun getCurrentThreadDraft(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMessageThreadDraftMessage")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun saveThreadDraft(chatId: Long, threadRootMessageId: Long, text: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setMessageThreadDraftMessage")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
            put("draft_message", JSONObject().apply {
                put("@type", "draftMessage")
                put("input_message_text", JSONObject().apply {
                    put("@type", "inputMessageText")
                    put("text", JSONObject().apply {
                        put("@type", "formattedText")
                        put("text", text)
                    })
                })
            })
        })
    }

    fun clearThreadDraft(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setMessageThreadDraftMessage")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
            put("draft_message", JSONObject().apply {
                put("@type", "draftMessage")
            })
        })
    }

    fun openMessageThread(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "openMessageThread")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun closeMessageThread(chatId: Long, threadRootMessageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "closeMessageThread")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    private fun settingsToJson(settings: TelegramNotificationSettings): JSONObject {
        return JSONObject().apply {
            put("@type", "chatNotificationSettings")
            put("use_default_mute_for", settings.useDefaultMuteFor)
            put("mute_for", settings.muteFor)
            put("use_default_sound", settings.useDefaultSound)
            put("sound_id", settings.soundId)
            put("use_default_show_preview", settings.useDefaultShowPreview)
            put("show_preview", settings.showPreview)
            put("use_default_disable_pinned_message_notifications", settings.useDefaultDisablePinnedMessageNotifications)
            put("disable_pinned_message_notifications", settings.disablePinnedMessageNotifications)
            put("use_default_disable_mention_notifications", settings.useDefaultDisableMentionNotifications)
            put("disable_mention_notifications", settings.disableMentionNotifications)
        }
    }

    fun parseThread(json: JSONObject): TelegramThread {
        return TelegramThread(
            chatId = json.optLong("chat_id", 0),
            messageThreadId = json.optLong("message_thread_id", 0),
            rootMessageId = json.optLong("root_message_id", 0),
            title = json.optString("thread_title", ""),
            replyCount = json.optInt("reply_count", 0),
            lastMessageId = json.optLong("last_message_id", 0),
            lastReadInboxMessageId = json.optLong("last_read_inbox_message_id", 0),
            lastReadOutboxMessageId = json.optLong("last_read_outbox_message_id", 0),
            isOutgoing = json.optBoolean("is_outgoing", false),
            unreadCount = json.optInt("unread_count", 0),
            unreadMentionCount = json.optInt("unread_mention_count", 0),
            unreadReactionCount = json.optInt("unread_reaction_count", 0)
        )
    }

    fun parseForumTopic(json: JSONObject): TelegramForumTopic {
        val info = json.optJSONObject("info") ?: json
        val icon = info.optJSONObject("icon")?.let { iconJson ->
            TelegramForumTopicIcon(
                customEmojiId = iconJson.optLong("custom_emoji_id", 0),
                color = iconJson.optInt("color", 0)
            )
        }
        return TelegramForumTopic(
            id = json.optLong("id", 0),
            chatId = json.optLong("chat_id", 0),
            name = info.optString("name", ""),
            creationDate = info.optLong("creation_date", 0),
            creatorUserId = info.optLong("creator_user_id", 0),
            icon = icon,
            isClosed = info.optBoolean("is_closed", false),
            isPinned = info.optBoolean("is_pinned", false),
            isGeneral = info.optBoolean("is_general", false),
            isHidden = info.optBoolean("is_hidden", false),
            unreadCount = json.optInt("unread_count", 0),
            unreadMentionCount = json.optInt("unread_mention_count", 0),
            unreadReactionCount = json.optInt("unread_reaction_count", 0),
            lastMessageId = json.optLong("last_message_id", 0),
            lastReadInboxMessageId = json.optLong("last_read_inbox_message_id", 0),
            lastReadOutboxMessageId = json.optLong("last_read_outbox_message_id", 0),
            notificationSettings = json.optJSONObject("notification_settings")?.let { parseNotificationSettings(it) }
        )
    }

    fun parseForumTopics(json: JSONObject): TelegramForumTopics {
        val topics = json.optJSONArray("topics")?.let { arr ->
            (0 until arr.length()).map { parseForumTopic(arr.getJSONObject(it)) }
        } ?: emptyList()
        return TelegramForumTopics(
            totalCount = json.optInt("total_count", topics.size),
            topics = topics,
            nextOffset = json.optLong("next_offset", 0)
        )
    }

    private fun parseNotificationSettings(json: JSONObject): TelegramNotificationSettings {
        return TelegramNotificationSettings(
            useDefaultMuteFor = json.optBoolean("use_default_mute_for", true),
            muteFor = json.optInt("mute_for", 0),
            useDefaultSound = json.optBoolean("use_default_sound", true),
            soundId = json.optLong("sound_id", 0),
            useDefaultShowPreview = json.optBoolean("use_default_show_preview", true),
            showPreview = json.optBoolean("show_preview", true),
            useDefaultDisablePinnedMessageNotifications = json.optBoolean("use_default_disable_pinned_message_notifications", true),
            disablePinnedMessageNotifications = json.optBoolean("disable_pinned_message_notifications", false),
            useDefaultDisableMentionNotifications = json.optBoolean("use_default_disable_mention_notifications", true),
            disableMentionNotifications = json.optBoolean("disable_mention_notifications", false)
        )
    }
}
