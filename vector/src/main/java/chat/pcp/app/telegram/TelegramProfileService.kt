package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramProfileService(private val client: TdLibClient) {

    fun getUser(userId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getUser")
            put("user_id", userId)
        })
    }

    fun getUserFullInfo(userId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getUserFullInfo")
            put("user_id", userId)
        })
    }

    fun getMe() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMe")
        })
    }

    fun getUserProfilePhotos(
        userId: Long,
        offset: Int = 0,
        limit: Int = 100
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getUserProfilePhotos")
            put("user_id", userId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun setProfilePhoto(photoPath: String, isPersonal: Boolean = true) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setProfilePhoto")
            put("photo", JSONObject().apply {
                put("@type", "inputChatPhotoStatic")
                put("photo", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", photoPath)
                })
            })
            put("is_personal", isPersonal)
        })
    }

    fun deleteProfilePhoto(profilePhotoId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteProfilePhoto")
            put("profile_photo_id", profilePhotoId)
        })
    }

    fun setProfilePhotoByFileId(fileId: Int, isPersonal: Boolean = true) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setProfilePhoto")
            put("photo", JSONObject().apply {
                put("@type", "inputChatPhotoPrevious")
                put("chat_photo_id", fileId)
            })
            put("is_personal", isPersonal)
        })
    }

    fun setBio(bio: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setBio")
            put("bio", bio)
        })
    }

    fun setName(firstName: String, lastName: String = "") {
        client.sendRequest(JSONObject().apply {
            put("@type", "setName")
            put("first_name", firstName)
            put("last_name", lastName)
        })
    }

    fun setUsername(username: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setUsername")
            put("username", username)
        })
    }

    fun checkUsername(username: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "checkUsername")
            put("username", username)
        })
    }

    fun setEmojiStatus(customEmojiId: Long, duration: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setEmojiStatus")
            put("emoji_status", JSONObject().apply {
                put("@type", "emojiStatus")
                put("custom_emoji_id", customEmojiId)
                put("expiration_date", 0)
                if (duration > 0) put("duration", duration)
            })
        })
    }

    fun clearEmojiStatus() {
        client.sendRequest(JSONObject().apply {
            put("@type", "setEmojiStatus")
            put("emoji_status", JSONObject().apply {
                put("@type", "emojiStatusEmpty")
            })
        })
    }

    fun getDefaultEmojiStatuses() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getDefaultEmojiStatuses")
        })
    }

    fun getThemedEmojiStatuses() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getThemedEmojiStatuses")
        })
    }

    fun getRecentEmojiStatuses() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getRecentEmojiStatuses")
        })
    }

    fun setCloseFriends(userIds: List<Long>) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setCloseFriends")
            put("user_ids", JSONArray(userIds))
        })
    }

    fun getCloseFriends() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getCloseFriends")
        })
    }

    fun getGroupsInCommon(userId: Long, offsetChatId: Long = 0, limit: Int = 100) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getGroupsInCommon")
            put("user_id", userId)
            put("offset_chat_id", offsetChatId)
            put("limit", limit)
        })
    }

    fun getActiveSessions() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getActiveSessions")
        })
    }

    fun terminateSession(sessionId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "terminateSession")
            put("session_id", sessionId)
        })
    }

    fun terminateAllOtherSessions() {
        client.sendRequest(JSONObject().apply {
            put("@type", "terminateAllOtherSessions")
        })
    }

    fun getConnectedWebsites() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getConnectedWebsites")
        })
    }

    fun disconnectWebsite(websiteId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "disconnectWebsite")
            put("website_id", websiteId)
        })
    }

    fun disconnectAllWebsites() {
        client.sendRequest(JSONObject().apply {
            put("@type", "disconnectAllWebsites")
        })
    }

    fun setAccountTtl(ttlDays: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setAccountTtl")
            put("ttl", JSONObject().apply {
                put("@type", "accountTtl")
                put("days", ttlDays)
            })
        })
    }

    fun getAccountTtl() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getAccountTtl")
        })
    }

    fun deleteAccount(reason: String = "", password: String = "") {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteAccount")
            put("reason", reason)
            put("password", password)
        })
    }

    fun changePhoneNumber(phoneNumber: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "changePhoneNumber")
            put("phone_number", phoneNumber)
        })
    }

    fun checkChangePhoneNumberCode(code: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "checkChangePhoneNumberCode")
            put("code", code)
        })
    }

    fun getPhoneNumberInfo(phoneNumber: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getPhoneNumberInfo")
            put("phone_number", phoneNumber)
        })
    }

    fun getPhoneNumberInfoSync(phoneNumber: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getPhoneNumberInfoSync")
            put("phone_number_prefix", phoneNumber)
        })
    }

    fun getBlockedUsers(offset: Int = 0, limit: Int = 100) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getBlockedMessagesSenders")
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun blockUser(userId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "blockMessageSenderFromReplies")
            put("message_id", 0)
            put("delete_message", true)
            put("delete_all_messages", true)
            put("report_spam", true)
        })
    }

    fun unblockUser(userId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "unblockMessageSenderFromReplies")
            put("message_id", 0)
        })
    }

    fun getPremiumState() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getPremiumState")
        })
    }

    fun getPremiumFeatures(source: String = "premiumSourceFeatures") {
        client.sendRequest(JSONObject().apply {
            put("@type", "getPremiumFeatures")
            put("source", JSONObject().apply { put("@type", source) })
        })
    }

    fun viewPremiumFeature(feature: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "viewPremiumFeature")
            put("feature", JSONObject().apply { put("@type", feature) })
        })
    }

    fun clickPremiumSubscriptionButton() {
        client.sendRequest(JSONObject().apply {
            put("@type", "clickPremiumSubscriptionButton")
        })
    }

    fun getPremiumGiftCodePaymentOptions() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getPremiumGiftCodePaymentOptions")
        })
    }

    fun checkPremiumGiftCode(code: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "checkPremiumGiftCode")
            put("code", code)
        })
    }

    fun applyPremiumGiftCode(code: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "applyPremiumGiftCode")
            put("code", code)
        })
    }

    fun getChatMember(chatId: Long, userId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatMember")
            put("chat_id", chatId)
            put("user_id", userId)
        })
    }

    fun getChatAdministrators(chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatAdministrators")
            put("chat_id", chatId)
        })
    }

    fun getSharedMedia(
        chatId: Long,
        userId: Long = 0,
        filter: String = "searchMessagesFilterEmpty",
        fromMessageId: Long = 0,
        offset: Int = 0,
        limit: Int = 50
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchChatMessages")
            put("chat_id", chatId)
            put("query", "")
            put("sender_id", if (userId > 0) JSONObject().apply {
                put("@type", "messageSenderUser")
                put("user_id", userId)
            } else null)
            put("from_message_id", fromMessageId)
            put("offset", offset)
            put("limit", limit)
            put("filter", JSONObject().apply { put("@type", filter) })
        })
    }

    fun getSharedPhotos(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterPhoto", limit = limit)
    }

    fun getSharedVideos(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterVideo", limit = limit)
    }

    fun getSharedDocuments(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterDocument", limit = limit)
    }

    fun getSharedVoiceNotes(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterVoiceNote", limit = limit)
    }

    fun getSharedLinks(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterUrl", limit = limit)
    }

    fun getSharedAudio(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterAudio", limit = limit)
    }

    fun getSharedAnimation(chatId: Long, userId: Long = 0, limit: Int = 50) {
        getSharedMedia(chatId, userId, "searchMessagesFilterAnimation", limit = limit)
    }

    fun getChatNotificationSettingsException(chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatNotificationSettingsException")
            put("chat_id", chatId)
        })
    }

    fun setChatNotificationSettings(chatId: Long, settings: TelegramNotificationSettings) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setChatNotificationSettings")
            put("chat_id", chatId)
            put("notification_settings", JSONObject().apply {
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
            })
        })
    }

    fun getChatAvailableBackgrounds(forDarkTheme: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getBackgrounds")
            put("for_dark_theme", forDarkTheme)
        })
    }

    fun getChatBackground(chatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getChatBackground")
            put("chat_id", chatId)
        })
    }

    fun setChatBackground(chatId: Long, wallpaperId: Long = 0, type: String = "backgroundTypeWallpaper") {
        client.sendRequest(JSONObject().apply {
            put("@type", "setChatBackground")
            put("chat_id", chatId)
            put("background", JSONObject().apply {
                put("@type", "inputBackgroundPrevious")
                put("background_id", wallpaperId)
            })
            if (type == "backgroundTypeFill") {
                put("type", JSONObject().apply { put("@type", "backgroundTypeFill") })
            }
        })
    }

    fun deleteChatBackground(chatId: Long, restorePrevious: Boolean = true) {
        client.sendRequest(JSONObject().apply {
            put("@type", "deleteChatBackground")
            put("chat_id", chatId)
            put("restore_previous", restorePrevious)
        })
    }

    fun searchBackground(name: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchBackground")
            put("name", name)
        })
    }

    fun setDefaultBackground(background: JSONObject, forDarkTheme: Boolean = false, type: String = "backgroundTypeWallpaper") {
        client.sendRequest(JSONObject().apply {
            put("@type", "setDefaultBackground")
            put("background", background)
            put("for_dark_theme", forDarkTheme)
            put("type", JSONObject().apply { put("@type", type) })
        })
    }

    fun getDefaultBackground(forDarkTheme: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getDefaultBackground")
            put("for_dark_theme", forDarkTheme)
        })
    }

    fun getInstalledBackgrounds(forDarkTheme: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getInstalledBackgrounds")
            put("for_dark_theme", forDarkTheme)
        })
    }

    fun removeInstalledBackground(backgroundId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "removeInstalledBackground")
            put("background_id", backgroundId)
        })
    }

    fun resetInstalledBackgrounds() {
        client.sendRequest(JSONObject().apply {
            put("@type", "resetInstalledBackgrounds")
        })
    }

    fun getLocalizationTargetInfo(onlyLocal: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getLocalizationTargetInfo")
            put("only_local", onlyLocal)
        })
    }

    fun getLanguagePackStrings(languagePackId: String, keys: List<String>) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getLanguagePackStrings")
            put("language_pack_id", languagePackId)
            put("keys", JSONArray(keys))
        })
    }

    fun synchronizeLanguagePack(languagePackId: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "synchronizeLanguagePack")
            put("language_pack_id", languagePackId)
        })
    }

    fun setCustomLanguagePack(languagePackId: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setCustomLanguagePack")
            put("language_pack_id", languagePackId)
        })
    }

    fun parseProfilePhoto(json: JSONObject): TelegramProfilePhoto {
        val sizes = json.optJSONArray("sizes")?.let { arr ->
            (0 until arr.length()).map { i ->
                val s = arr.getJSONObject(i)
                TelegramPhoto.PhotoSize(
                    type = s.optString("type", ""),
                    photoId = s.optLong("id", 0L),
                    width = s.optInt("width", 0),
                    height = s.optInt("height", 0),
                    fileId = s.optJSONObject("photo")?.optInt("id", 0) ?: 0,
                    fileUniqueId = s.optJSONObject("photo")?.optInt("remote", 0)?.let { it xor 0x4001 } ?: 0,
                    fileSize = s.optJSONObject("photo")?.optLong("size", 0L) ?: 0L
                )
            }
        } ?: emptyList()
        return TelegramProfilePhoto(
            id = json.optLong("id", 0),
            isPersonal = json.optBoolean("is_personal", true),
            hasAnimation = json.optBoolean("has_animation", false),
            addedDate = json.optLong("added_date", 0),
            sizes = sizes,
            minithumbnail = null
        )
    }

    fun parseEmojiStatus(json: JSONObject?): TelegramEmojiStatus? {
        if (json == null) return null
        val type = json.optString("@type", "")
        if (type != "emojiStatus") return null
        return TelegramEmojiStatus(
            customEmojiId = json.optLong("custom_emoji_id", 0),
            expirationDate = json.optLong("expiration_date", 0)
        )
    }

    fun parseUser(json: JSONObject): TelegramFullUser {
        val type = json.optJSONObject("type")?.optString("@type", "") ?: ""
        return TelegramFullUser(
            id = json.optLong("id", 0),
            firstName = json.optString("first_name", ""),
            lastName = json.optString("last_name", ""),
            username = json.optString("username", ""),
            phoneNumber = json.optString("phone_number", ""),
            bio = null,
            profilePhoto = json.optJSONObject("profile_photo")?.let { parseProfilePhoto(it) },
            emojiStatus = json.optJSONObject("emoji_status")?.let { parseEmojiStatus(it) },
            premiumStatus = null,
            isVerified = json.optBoolean("is_verified", false),
            isSupport = json.optBoolean("is_support", false),
            isBot = type.contains("Bot"),
            isScam = json.optBoolean("is_scam", false),
            isFake = json.optBoolean("is_fake", false),
            isContact = json.optBoolean("is_contact", false),
            isMutualContact = json.optBoolean("is_mutual_contact", false),
            isCloseFriend = json.optBoolean("is_close_friend", false),
            restrictionReason = json.optStringOrNull("restriction_reason"),
            haveAccess = json.optBoolean("have_access", true),
            type = type,
            languageCode = json.optString("language_code", ""),
            addedToAttachmentMenu = json.optBoolean("added_to_attachment_menu", false)
        )
    }

    fun parseUserFullInfo(user: TelegramFullUser, fullInfo: JSONObject): TelegramFullUser {
        return user.copy(
            bio = fullInfo.optStringOrNull("bio") ?: user.bio,
            premiumStatus = fullInfo.optBoolean("is_premium", false).let { isPremium ->
                if (isPremium) TelegramPremiumStatus(
                    isPremium = true,
                    giftedById = fullInfo.optLong("gifted_by_id", 0),
                    expiresIn = fullInfo.optLong("premium_expiration_date", 0),
                    features = emptyList(),
                    subscriptionType = fullInfo.optString("premium_subscription_type", ""),
                    canUpgrade = fullInfo.optBoolean("can_upgrade", false)
                ) else null
            },
            isContact = fullInfo.optBoolean("is_contact", user.isContact),
            isMutualContact = fullInfo.optBoolean("is_mutual_contact", user.isMutualContact)
        )
    }
}
