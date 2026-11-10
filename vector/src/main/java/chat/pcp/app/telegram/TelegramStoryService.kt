package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramStoryService(private val client: TdLibClient) {

    fun getActiveStories(limit: Int = 100) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getActiveStories")
            put("limit", limit)
        })
    }

    fun getChatActiveStories(chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatActiveStories")
            put("chat_id", chatId)
        })
    }

    fun getStory(chatId: Long, storyId: Long, onlyLocal: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStory")
            put("chat_id", chatId)
            put("story_id", storyId)
            put("only_local", onlyLocal)
        })
    }

    fun getStories(chatId: Long, storyIds: LongArray, onlyLocal: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStories")
            put("chat_id", chatId)
            put("story_ids", JSONArray(storyIds.toList()))
            put("only_local", onlyLocal)
        })
    }

    @Suppress("CyclomaticComplexMethod")
    fun sendStory(
        content: TelegramStoryContent,
        caption: String? = null,
        privacySettings: StoryPrivacySettings? = null,
        activePeriod: Int = 86400,
        isPinned: Boolean = false,
        protectContent: Boolean = false,
        fromStoryFullId: Long? = null
    ) {
        val inputContent = when (content) {
            is TelegramStoryContent.Photo -> {
                JSONObject().apply {
                    put("@type", "inputStoryContentPhoto")
                    put("photo", JSONObject().apply {
                        put("@type", "inputFileId")
                        put("id", content.photo.sizes.firstOrNull()?.fileId ?: 0)
                    })
                    put("added_sticker_file_ids", JSONArray())
                }
            }
            is TelegramStoryContent.Video -> {
                JSONObject().apply {
                    put("@type", "inputStoryContentVideo")
                    put("video", JSONObject().apply {
                        put("@type", "inputFileId")
                        put("id", content.video.fileId)
                    })
                    put("duration", content.video.duration)
                    put("cover_frame_timestamp", 0.0)
                    put("is_animation", content.isAnimation)
                    put("added_sticker_file_ids", JSONArray())
                }
            }
            else -> return
        }

        client.sendRequest(JSONObject().apply {
            put("@type", "sendStory")
            put("content", inputContent)
            if (caption != null) {
                put("caption", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", caption)
                })
            }
            put("privacy_settings", buildPrivacySettings(privacySettings))
            put("active_period", activePeriod)
            put("is_pinned", isPinned)
            put("protect_content", protectContent)
            if (fromStoryFullId != null) {
                put("from_story_full_id", JSONObject().apply {
                    put("@type", "storyFullId")
                    put("sender_chat_id", fromStoryFullId)
                    put("story_id", 0)
                })
            }
        })
    }

    fun editStory(
        chatId: Long,
        storyId: Long,
        content: TelegramStoryContent? = null,
        caption: String? = null,
        privacySettings: StoryPrivacySettings? = null
    ) {
        val inputContent = content?.let {
            when (it) {
                is TelegramStoryContent.Photo -> JSONObject().apply {
                    put("@type", "inputStoryContentPhoto")
                    put("photo", JSONObject().apply {
                        put("@type", "inputFileId")
                        put("id", it.photo.sizes.firstOrNull()?.fileId ?: 0)
                    })
                    put("added_sticker_file_ids", JSONArray())
                }
                is TelegramStoryContent.Video -> JSONObject().apply {
                    put("@type", "inputStoryContentVideo")
                    put("video", JSONObject().apply {
                        put("@type", "inputFileId")
                        put("id", it.video.fileId)
                    })
                    put("duration", it.video.duration)
                    put("cover_frame_timestamp", 0.0)
                    put("is_animation", it.isAnimation)
                    put("added_sticker_file_ids", JSONArray())
                }
                else -> null
            }
        }

        client.sendRequest(JSONObject().apply {
            put("@type", "editStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            if (inputContent != null) put("content", inputContent)
            if (caption != null) {
                put("caption", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", caption)
                })
            }
            if (privacySettings != null) {
                put("privacy_settings", buildPrivacySettings(privacySettings))
            }
        })
    }

    fun deleteStory(chatId: Long, storyId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
        })
    }

    fun setStoryReaction(
        chatId: Long,
        storyId: Long,
        reactionType: String?,
        updateRecentReactions: Boolean = true
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setStoryReaction")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("reaction_type", if (reactionType != null) {
                if (reactionType.startsWith("custom:")) {
                    JSONObject().apply {
                        put("@type", "reactionTypeCustomEmoji")
                        put("custom_emoji_id", reactionType.removePrefix("custom:").toLong())
                    }
                } else {
                    JSONObject().apply {
                        put("@type", "reactionTypeEmoji")
                        put("emoji", reactionType)
                    }
                }
            } else {
                JSONObject.NULL
            })
            put("update_recent_reactions", updateRecentReactions)
        })
    }

    fun openStory(chatId: Long, storyId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "openStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
        })
    }

    fun closeStory(chatId: Long, storyId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "closeStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
        })
    }

    fun getStoryViewers(
        chatId: Long,
        storyId: Long,
        offset: String = "",
        limit: Int = 50,
        onlyContacts: Boolean = false,
        preferWithReaction: Boolean = false
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryViewers")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("offset", offset)
            put("limit", limit)
            put("only_contacts", onlyContacts)
            put("prefer_with_reaction", preferWithReaction)
        })
    }

    fun getStoryInteractions(chatId: Long, storyId: Long, offset: String = "", limit: Int = 50) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryInteractions")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun getStoryStatistics(chatId: Long, storyId: Long, isDark: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryStatistics")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("is_dark", isDark)
        })
    }

    fun canSendStory(chatId: Long? = null): CanSendStoryResult {
        val result = client.execute(JSONObject().apply {
            put("@type", "canSendStory")
        })
        return try {
            val json = JSONObject(result)
            CanSendStoryResult(
                canSend = json.optBoolean("can_send", false),
                activeStoryLimitReached = json.optBoolean("active_story_limit_reached", false),
                weeklyLimitReached = json.optBoolean("weekly_limit_reached", false),
                monthlyLimitReached = json.optBoolean("monthly_limit_reached", false),
                needPremium = json.optBoolean("need_premium", false)
            )
        } catch (e: Exception) {
            CanSendStoryResult(false, false, false, false, true)
        }
    }

    fun toggleStoryIsPinned(chatId: Long, storyId: Long, isPinned: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "toggleStoryIsPinned")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("is_pinned", isPinned)
        })
    }

    fun loadActiveStories(limit: Int = 50) {
        client.sendRequest(JSONObject().apply {
            put("@type", "loadActiveStories")
            put("limit", limit)
        })
    }

    fun getStoryNotificationSettings() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryNotificationSettings")
        })
    }

    fun setStoryNotificationSettings(
        enableNotifications: Boolean = true,
        mutePeriod: Int = 0,
        soundId: Long = 0,
        showPreview: Boolean = true
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setStoryNotificationSettings")
            put("story_notification_settings", JSONObject().apply {
                put("@type", "storyNotificationSettings")
                put("enable_notifications", enableNotifications)
                put("mute_period", mutePeriod)
                put("sound_id", soundId)
                put("show_preview", showPreview)
            })
        })
    }

    fun getArchivedStories(fromStoryId: Long = 0, limit: Int = 100) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getArchivedStories")
            put("from_story_id", fromStoryId)
            put("limit", limit)
        })
    }

    fun reportStory(chatId: Long, storyId: Long, reason: String = "chatReportReasonSpam", text: String = "") {
        client.sendRequest(JSONObject().apply {
            put("@type", "reportStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("reason", JSONObject().apply { put("@type", reason) })
            if (text.isNotEmpty()) put("text", text)
        })
    }

    fun getStoryAvailableReactions(rowSize: Int = 8) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryAvailableReactions")
            put("row_size", rowSize)
        })
    }

    fun getChatsToSendStories() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatsToSendStories")
        })
    }

    fun getStoryPrivacy() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryPrivacy")
        })
    }

    fun setStoryPrivacy(privacySettings: StoryPrivacySettings, isActive: Boolean = true) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setStoryPrivacy")
            put("privacy_settings", buildPrivacySettings(privacySettings))
            put("is_active", isActive)
        })
    }

    fun activateStoryStealthMode() {
        client.sendRequest(JSONObject().apply {
            put("@type", "activateStoryStealthMode")
        })
    }

    fun canSendStoryToChat(chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "canSendStoryToChat")
            put("chat_id", chatId)
        })
    }

    fun forwardStory(
        toChatId: Long,
        fromChatId: Long,
        storyId: Long,
        caption: String? = null
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", toChatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageStory")
                put("story_sender_chat_id", fromChatId)
                put("story_id", storyId)
            })
            if (caption != null) {
                put("caption", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", caption)
                })
            }
        })
    }

    fun getStoryNotificationExceptions() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStoryNotificationExceptions")
        })
    }

    fun setStoryNotificationExceptions(userIds: List<Long>, enabled: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setStoryNotificationExceptions")
            put("user_ids", JSONArray(userIds))
            put("are_notifications_enabled", enabled)
        })
    }

    private fun buildPrivacySettings(settings: StoryPrivacySettings?): JSONObject {
        val visibility = settings?.visibility ?: StoryVisibility.EVERYONE
        val exceptionIds = settings?.exceptionUserIds ?: emptyList()
        val forwardingAllowed = settings?.isForwardingAllowed ?: true

        return JSONObject().apply {
            put("@type", "storyPrivacySettings")
            put("visibility", visibility.toJson())
            put("is_forwarding_allowed", forwardingAllowed)
            put("exception_user_ids", JSONArray(exceptionIds))
        }
    }

    private fun StoryVisibility.toJson(): JSONObject {
        return JSONObject().apply {
            put("@type", when (this@toJson) {
                StoryVisibility.EVERYONE -> "storyPrivacySettingsEveryone"
                StoryVisibility.CONTACTS -> "storyPrivacySettingsContacts"
                StoryVisibility.CLOSE_FRIENDS -> "storyPrivacySettingsCloseFriends"
                StoryVisibility.SELECTED_USERS -> "storyPrivacySettingsSelectedUsers"
            })
        }
    }

    fun parseStory(json: JSONObject): TelegramStory {
        val content = parseStoryContent(json.optJSONObject("content"))
        val interaction = json.optJSONObject("interaction_info")?.let { parseInteractionInfo(it) }
        val privacy = json.optJSONObject("privacy_settings")?.let { parsePrivacySettings(it) }
        val chosenReaction = json.optJSONObject("chosen_reaction_type")?.let { reaction ->
            when (reaction.optString("@type", "")) {
                "reactionTypeEmoji" -> reaction.optString("emoji", "")
                "reactionTypeCustomEmoji" -> "custom:${reaction.optLong("custom_emoji_id", 0)}"
                else -> null
            }
        }

        return TelegramStory(
            id = json.optLong("id", 0),
            senderChatId = json.optLong("sender_chat_id", 0),
            date = json.optLong("date", 0),
            isBeingViewed = json.optBoolean("is_being_viewed", false),
            isEdited = json.optBoolean("is_edited", false),
            isPinned = json.optBoolean("is_pinned", false),
            isVisibleOnlyForSelf = json.optBoolean("is_visible_only_for_self", false),
            canBeForwarded = json.optBoolean("can_be_forwarded", false),
            canBeReplied = json.optBoolean("can_be_replied", false),
            canGetStatistics = json.optBoolean("can_get_statistics", false),
            canToggleIsPinned = json.optBoolean("can_toggle_is_pinned", false),
            content = content,
            interactionInfo = interaction,
            chosenReactionType = chosenReaction,
            privacySettings = privacy
        )
    }

    private fun parseStoryContent(json: JSONObject?): TelegramStoryContent {
        if (json == null) return TelegramStoryContent.Unsupported("null")
        return when (json.optString("@type", "")) {
            "storyContentPhoto" -> {
                val photo = json.optJSONObject("photo")?.let { TelegramContentParser.parseMessage(
                    JSONObject().apply {
                        put("@type", "message")
                        put("content", JSONObject().apply {
                            put("@type", "messagePhoto")
                            put("photo", it)
                        })
                    }
                ).photo } ?: return TelegramStoryContent.Unsupported("no_photo")
                TelegramStoryContent.Photo(photo)
            }
            "storyContentVideo" -> {
                val video = json.optJSONObject("video")?.let { videoJson ->
                    TelegramVideo(
                        duration = videoJson.optInt("duration", 0),
                        width = videoJson.optInt("width", 0),
                        height = videoJson.optInt("height", 0),
                        mimeType = videoJson.optString("mime_type", "video/mp4"),
                        thumbnail = null,
                        videoId = videoJson.optInt("id", 0),
                        fileId = videoJson.optInt("id", 0),
                        fileUniqueId = videoJson.optInt("remote", 0)?.let { it xor 0x3001 } ?: 0,
                        fileSize = videoJson.optLong("size", 0L),
                        supportsStreaming = videoJson.optBoolean("supports_streaming", false),
                        isSpoiler = false
                    )
                } ?: return TelegramStoryContent.Unsupported("no_video")
                TelegramStoryContent.Video(
                    video = video,
                    isAnimation = json.optBoolean("is_animation", false),
                    altDocument = json.optJSONObject("alternative_document")?.let { doc ->
                        TelegramDocument(
                            fileName = doc.optString("file_name", ""),
                            mimeType = doc.optString("mime_type", ""),
                            minithumbnail = null,
                            thumbnail = null,
                            documentId = doc.optInt("id", 0),
                            fileId = doc.optInt("id", 0),
                            fileUniqueId = doc.optInt("remote", 0)?.let { it xor 0x3101 } ?: 0,
                            fileSize = doc.optLong("size", 0L)
                        )
                    }
                )
            }
            "storyContentUnsupported" -> TelegramStoryContent.Unsupported(json.optString("type", "unsupported"))
            else -> TelegramStoryContent.Unsupported(json.optString("@type", "unknown"))
        }
    }

    private fun parseInteractionInfo(json: JSONObject): StoryInteractionInfo {
        return StoryInteractionInfo(
            viewCount = json.optInt("view_count", 0),
            forwardCount = json.optInt("forward_count", 0),
            reactionCount = json.optInt("reaction_count", 0),
            recentViewerIds = json.optJSONArray("recent_viewer_user_ids")?.let { arr ->
                (0 until arr.length()).map { arr.getLong(it) }
            } ?: emptyList()
        )
    }

    private fun parsePrivacySettings(json: JSONObject): StoryPrivacySettings {
        val visibility = when (json.optJSONObject("visibility")?.optString("@type", "")) {
            "storyPrivacySettingsEveryone" -> StoryVisibility.EVERYONE
            "storyPrivacySettingsContacts" -> StoryVisibility.CONTACTS
            "storyPrivacySettingsCloseFriends" -> StoryVisibility.CLOSE_FRIENDS
            "storyPrivacySettingsSelectedUsers" -> StoryVisibility.SELECTED_USERS
            else -> StoryVisibility.CONTACTS
        }
        return StoryPrivacySettings(
            visibility = visibility,
            isForwardingAllowed = json.optBoolean("is_forwarding_allowed", true),
            exceptionUserIds = json.optJSONArray("exception_user_ids")?.let { arr ->
                (0 until arr.length()).map { arr.getLong(it) }
            } ?: emptyList()
        )
    }
}
