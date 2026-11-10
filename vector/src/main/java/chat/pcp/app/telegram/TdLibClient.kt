package chat.pcp.app.telegram

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.isActive
import kotlinx.coroutines.launch
import org.json.JSONObject
import timber.log.Timber

class TdLibClient(
    private val databaseDirectory: String,
    private val filesDirectory: String,
    private val apiId: Int,
    private val apiHash: String
) {
    private val scope = CoroutineScope(Dispatchers.IO + Job())
    private var clientPtr: Long = 0
    private var receiveJob: Job? = null

    private val _authState = MutableStateFlow(TelegramAuthState(AuthStep.WAITING_PHONE_NUMBER))
    val authState: StateFlow<TelegramAuthState> = _authState

    private val _connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)
    val connectionState: StateFlow<ConnectionState> = _connectionState

    val userId: String get() = _userId
    internal var _userId = ""

    private val listeners = mutableListOf<TelegramUpdateListener>()

    fun initialize() {
        if (!TdLibNative.ensureLoaded()) {
            Timber.e("TDLib native library not available")
            _authState.value = TelegramAuthState(AuthStep.ERROR, hint = "TDLib unavailable")
            return
        }

        clientPtr = TdLibNative.nativeTdCreateClient()
        if (clientPtr == 0L) {
            Timber.e("Failed to create TDLib client")
            _authState.value = TelegramAuthState(AuthStep.ERROR, hint = "Client creation failed")
            return
        }

        Timber.i("TDLib client created: $clientPtr")

        sendRequest(buildJson {
            put("@type", "setTdlibParameters")
            put("use_test_dc", false)
            put("database_directory", databaseDirectory)
            put("files_directory", filesDirectory)
            put("use_file_database", true)
            put("use_chat_info_database", true)
            put("use_message_database", true)
            put("use_secret_chats", false)
            put("api_id", apiId)
            put("api_hash", apiHash)
            put("system_language_code", "en")
            put("device_model", "Android")
            put("application_version", "1.0")
            put("enable_storage_optimizer", true)
            put("ignore_file_names", false)
        })

        startReceiveLoop()
    }

    fun sendPhoneNumber(phone: String) {
        sendRequest(buildJson {
            put("@type", "setAuthenticationPhoneNumber")
            put("phone_number", phone)
        })
    }

    fun sendCode(code: String) {
        sendRequest(buildJson {
            put("@type", "checkAuthenticationCode")
            put("code", code)
        })
    }

    fun sendPassword(password: String) {
        sendRequest(buildJson {
            put("@type", "checkAuthenticationPassword")
            put("password", password)
        })
    }

    fun loadChats(limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "loadChats")
            put("limit", limit)
        })
    }

    fun getChatHistory(chatId: Long, limit: Int = 50, fromMessageId: Long = 0) {
        sendRequest(buildJson {
            put("@type", "getChatHistory")
            put("chat_id", chatId)
            put("limit", limit)
            put("from_message_id", fromMessageId)
        })
    }

    fun sendChatMessage(chatId: Long, text: String) {
        sendRequest(buildJson {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageText")
                put("text", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", text)
                })
            })
        })
    }

    fun viewMessages(chatId: Long, messageIds: LongArray) {
        sendRequest(buildJson {
            put("@type", "viewMessages")
            put("chat_id", chatId)
            put("message_ids", org.json.JSONArray(messageIds.toList()))
        })
    }

    fun logout() {
        sendRequest(buildJson { put("@type", "logOut") })
    }

    fun execute(json: JSONObject): String {
        val request = json.toString()
        val result = TdLibNative.nativeTdExecute(clientPtr, request)
        return result ?: ""
    }

    fun sendMessageAlbum(chatId: Long, albumItems: org.json.JSONArray) {
        sendRequest(buildJson {
            put("@type", "sendMessageAlbum")
            put("chat_id", chatId)
            put("input_message_contents", albumItems)
        })
    }

    fun downloadFile(fileId: Int, priority: Int = 1, offset: Long = 0, limit: Long = 0, synchronous: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "downloadFile")
            put("file_id", fileId)
            put("priority", priority)
            put("offset", offset)
            put("limit", limit)
            put("synchronous", synchronous)
        })
    }

    fun cancelDownloadFile(fileId: Int, onlyIfPending: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "cancelDownloadFile")
            put("file_id", fileId)
            put("only_if_pending", onlyIfPending)
        })
    }

    fun getMessageLink(chatId: Long, messageId: Long, mediaTimestamp: Int = 0, forAlbum: Boolean = false, forComment: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "getMessageLink")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("media_timestamp", mediaTimestamp)
            put("for_album", forAlbum)
            put("for_comment", forComment)
        })
    }

    fun getScheduledMessages(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "getScheduledMessages")
            put("chat_id", chatId)
        })
    }

    fun getChatPinnedMessage(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "getChatPinnedMessage")
            put("chat_id", chatId)
        })
    }

    fun pinChatMessage(chatId: Long, messageId: Long, disableNotification: Boolean = false, onlyForSelf: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "pinChatMessage")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("disable_notification", disableNotification)
            put("only_for_self", onlyForSelf)
        })
    }

    fun unpinChatMessage(chatId: Long, messageId: Long = 0) {
        sendRequest(buildJson {
            put("@type", "unpinChatMessage")
            put("chat_id", chatId)
            put("message_id", messageId)
        })
    }

    fun unpinAllChatMessages(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "unpinAllChatMessages")
            put("chat_id", chatId)
        })
    }

    fun deleteMessages(chatId: Long, messageIds: LongArray, revoke: Boolean = true) {
        sendRequest(buildJson {
            put("@type", "deleteMessages")
            put("chat_id", chatId)
            put("message_ids", org.json.JSONArray(messageIds.toList()))
            put("revoke", revoke)
        })
    }

    fun editMessageText(chatId: Long, messageId: Long, newText: String) {
        sendRequest(buildJson {
            put("@type", "editMessageText")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageText")
                put("text", JSONObject().apply {
                    put("@type", "formattedText")
                    put("text", newText)
                })
            })
        })
    }

    fun editMessageCaption(chatId: Long, messageId: Long, newCaption: String) {
        sendRequest(buildJson {
            put("@type", "editMessageCaption")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("caption", JSONObject().apply {
                put("@type", "formattedText")
                put("text", newCaption)
            })
        })
    }

    fun searchMessages(query: String, limit: Int = 50, offset: String = "", filter: String = "searchMessagesFilterEmpty") {
        sendRequest(buildJson {
            put("@type", "searchMessages")
            put("query", query)
            put("limit", limit)
            put("offset", offset)
            put("filter", JSONObject().apply {
                put("@type", filter)
            })
        })
    }

    fun searchChatMessages(chatId: Long, query: String, limit: Int = 50, fromMessageId: Long = 0, offset: Int = 0, filter: String = "searchMessagesFilterEmpty") {
        sendRequest(buildJson {
            put("@type", "searchChatMessages")
            put("chat_id", chatId)
            put("query", query)
            put("limit", limit)
            put("from_message_id", fromMessageId)
            put("offset", offset)
            put("filter", JSONObject().apply {
                put("@type", filter)
            })
        })
    }

    fun openChat(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "openChat")
            put("chat_id", chatId)
        })
    }

    fun closeChat(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "closeChat")
            put("chat_id", chatId)
        })
    }

    fun setChatTitle(chatId: Long, title: String) {
        sendRequest(buildJson {
            put("@type", "setChatTitle")
            put("chat_id", chatId)
            put("title", title)
        })
    }

    fun toggleChatIsPinned(chatId: Long, isPinned: Boolean) {
        sendRequest(buildJson {
            put("@type", "toggleChatIsPinned")
            put("chat_id", chatId)
            put("is_pinned", isPinned)
        })
    }

    fun toggleChatIsMarkedAsUnread(chatId: Long, isMarked: Boolean) {
        sendRequest(buildJson {
            put("@type", "toggleChatIsMarkedAsUnread")
            put("chat_id", chatId)
            put("is_marked_as_unread", isMarked)
        })
    }

    fun setChatDraftMessage(chatId: Long, text: String) {
        sendRequest(buildJson {
            put("@type", "setChatDraftMessage")
            put("chat_id", chatId)
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

    fun setChatClientData(chatId: Long, clientData: String) {
        sendRequest(buildJson {
            put("@type", "setChatClientData")
            put("chat_id", chatId)
            put("client_data", clientData)
        })
    }

    fun getChatMember(chatId: Long, userId: Long) {
        sendRequest(buildJson {
            put("@type", "getChatMember")
            put("chat_id", chatId)
            put("user_id", userId)
        })
    }

    fun searchChatMembers(chatId: Long, query: String, limit: Int = 50) {
        sendRequest(buildJson {
            put("@type", "searchChatMembers")
            put("chat_id", chatId)
            put("query", query)
            put("limit", limit)
        })
    }

    fun getUserProfilePhotos(userId: Long, offset: Int = 0, limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "getUserProfilePhotos")
            put("user_id", userId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun searchPublicChat(username: String) {
        sendRequest(buildJson {
            put("@type", "searchPublicChat")
            put("username", username)
        })
    }

    fun searchPublicChats(query: String) {
        sendRequest(buildJson {
            put("@type", "searchPublicChats")
            put("query", query)
        })
    }

    fun createNewSupergroupChat(title: String, isChannel: Boolean = false, description: String = "", forImport: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "createNewSupergroupChat")
            put("title", title)
            put("is_channel", isChannel)
            put("description", description)
            put("for_import", forImport)
        })
    }

    fun createNewBasicGroupChat(userIds: LongArray, title: String = "") {
        sendRequest(buildJson {
            put("@type", "createNewBasicGroupChat")
            put("user_ids", org.json.JSONArray(userIds.toList()))
            put("title", title)
        })
    }

    fun createNewSecretChat(userId: Long) {
        sendRequest(buildJson {
            put("@type", "createNewSecretChat")
            put("user_id", userId)
        })
    }

    fun searchContacts(query: String = "", limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "searchContacts")
            put("query", query)
            put("limit", limit)
        })
    }

    fun importContacts(contacts: List<JSONObject>) {
        sendRequest(buildJson {
            put("@type", "importContacts")
            put("contacts", org.json.JSONArray(contacts))
        })
    }

    fun getGroupsInCommon(userId: Long, offset: Long = 0, limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "getGroupsInCommon")
            put("user_id", userId)
            put("offset_chat_id", offset)
            put("limit", limit)
        })
    }

    fun searchHashtags(prefix: String, limit: Int = 20) {
        sendRequest(buildJson {
            put("@type", "searchHashtags")
            put("prefix", prefix)
            put("limit", limit)
        })
    }

    fun getStorageStatistics(chatLimit: Int = 10) {
        sendRequest(buildJson {
            put("@type", "getStorageStatistics")
            put("chat_limit", chatLimit)
        })
    }

    fun getStorageStatisticsFast() {
        sendRequest(buildJson {
            put("@type", "getStorageStatisticsFast")
        })
    }

    fun getNetworkStatistics(onlyCurrent: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "getNetworkStatistics")
            put("only_current", onlyCurrent)
        })
    }

    fun getApplicationConfig() {
        sendRequest(buildJson {
            put("@type", "getApplicationConfig")
        })
    }

    fun getCountryCode() {
        sendRequest(buildJson {
            put("@type", "getCountryCode")
        })
    }

    fun getApplicationDownloadLink() {
        sendRequest(buildJson {
            put("@type", "getApplicationDownloadLink")
        })
    }

    fun getCurrentState() {
        sendRequest(buildJson {
            put("@type", "getCurrentState")
        })
    }

    fun resetAllNotificationSettings() {
        sendRequest(buildJson {
            put("@type", "resetAllNotificationSettings")
        })
    }

    fun setAlarm(seconds: Double) {
        sendRequest(buildJson {
            put("@type", "setAlarm")
            put("seconds", seconds)
        })
    }

    fun getSupportUser() {
        sendRequest(buildJson {
            put("@type", "getSupportUser")
        })
    }

    fun checkChatInviteLink(inviteLink: String) {
        sendRequest(buildJson {
            put("@type", "checkChatInviteLink")
            put("invite_link", inviteLink)
        })
    }

    fun joinChatByInviteLink(inviteLink: String) {
        sendRequest(buildJson {
            put("@type", "joinChatByInviteLink")
            put("invite_link", inviteLink)
        })
    }

    fun getChatInviteLink(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "getChatInviteLink")
            put("chat_id", chatId)
        })
    }

    fun getStory(chatId: Long, storyId: Long) {
        sendRequest(buildJson {
            put("@type", "getStory")
            put("chat_id", chatId)
            put("story_id", storyId)
        })
    }

    fun getChatActiveStories(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "getChatActiveStories")
            put("chat_id", chatId)
        })
    }

    fun readAllChatMentions(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "readAllChatMentions")
            put("chat_id", chatId)
        })
    }

    fun readAllChatReactions(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "readAllChatReactions")
            put("chat_id", chatId)
        })
    }

    fun getMessageThread(chatId: Long, messageId: Long) {
        sendRequest(buildJson {
            put("@type", "getMessageThread")
            put("chat_id", chatId)
            put("message_id", messageId)
        })
    }

    fun getMessageThreadHistory(chatId: Long, messageId: Long, fromMessageId: Long = 0, limit: Int = 50) {
        sendRequest(buildJson {
            put("@type", "getMessageThreadHistory")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("from_message_id", fromMessageId)
            put("limit", limit)
        })
    }

    fun getForumTopicDefaultIcons() {
        sendRequest(buildJson {
            put("@type", "getForumTopicDefaultIcons")
        })
    }

    fun createForumTopic(chatId: Long, name: String, iconId: Int = 0) {
        sendRequest(buildJson {
            put("@type", "createForumTopic")
            put("chat_id", chatId)
            put("name", name)
            put("icon", JSONObject().apply {
                put("@type", "forumTopicIcon")
                put("custom_emoji_id", iconId)
            })
        })
    }

    fun getForumTopics(chatId: Long, offset: Long = 0, limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "getForumTopics")
            put("chat_id", chatId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun reportChat(chatId: Long, messageIds: LongArray? = null, reason: String = "", text: String = "") {
        sendRequest(buildJson {
            put("@type", "reportChat")
            put("chat_id", chatId)
            if (messageIds != null) put("message_ids", org.json.JSONArray(messageIds.toList()))
            put("reason", JSONObject().apply {
                put("@type", if (reason.isNotEmpty()) reason else "chatReportReasonSpam")
            })
            if (text.isNotEmpty()) put("text", text)
        })
    }

    fun blockUser(userId: Long) {
        sendRequest(buildJson {
            put("@type", "blockMessageSenderFromReplies")
            put("message_id", 0)
            put("delete_message", true)
            put("delete_all_messages", true)
            put("report_spam", true)
        })
    }

    fun getActiveStories(limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "getActiveStories")
            put("limit", limit)
        })
    }

    fun getStory(chatId: Long, storyId: Long, onlyLocal: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "getStory")
            put("chat_id", chatId)
            put("story_id", storyId)
            put("only_local", onlyLocal)
        })
    }

    fun openStory(chatId: Long, storyId: Long) {
        sendRequest(buildJson {
            put("@type", "openStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
        })
    }

    fun closeStory(chatId: Long, storyId: Long) {
        sendRequest(buildJson {
            put("@type", "closeStory")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
        })
    }

    fun getStoryInteractions(chatId: Long, storyId: Long, offset: String = "", limit: Int = 50) {
        sendRequest(buildJson {
            put("@type", "getStoryInteractions")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun getStoryStatistics(chatId: Long, storyId: Long, isDark: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "getStoryStatistics")
            put("story_sender_chat_id", chatId)
            put("story_id", storyId)
            put("is_dark", isDark)
        })
    }

    fun getStoryNotificationSettings() {
        sendRequest(buildJson {
            put("@type", "getStoryNotificationSettings")
        })
    }

    fun getArchivedStories(fromStoryId: Long = 0, limit: Int = 100) {
        sendRequest(buildJson {
            put("@type", "getArchivedStories")
            put("from_story_id", fromStoryId)
            put("limit", limit)
        })
    }

    fun getStoryPrivacy() {
        sendRequest(buildJson {
            put("@type", "getStoryPrivacy")
        })
    }

    fun getUser(userId: Long) {
        sendRequest(buildJson {
            put("@type", "getUser")
            put("user_id", userId)
        })
    }

    fun getUserFullInfo(userId: Long) {
        sendRequest(buildJson {
            put("@type", "getUserFullInfo")
            put("user_id", userId)
        })
    }

    fun getMe() {
        sendRequest(buildJson {
            put("@type", "getMe")
        })
    }

    fun setBio(bio: String) {
        sendRequest(buildJson {
            put("@type", "setBio")
            put("bio", bio)
        })
    }

    fun setName(firstName: String, lastName: String = "") {
        sendRequest(buildJson {
            put("@type", "setName")
            put("first_name", firstName)
            put("last_name", lastName)
        })
    }

    fun setUsername(username: String) {
        sendRequest(buildJson {
            put("@type", "setUsername")
            put("username", username)
        })
    }

    fun setProfilePhoto(photoPath: String) {
        sendRequest(buildJson {
            put("@type", "setProfilePhoto")
            put("photo", JSONObject().apply {
                put("@type", "inputChatPhotoStatic")
                put("photo", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", photoPath)
                })
            })
        })
    }

    fun deleteProfilePhoto(profilePhotoId: Long) {
        sendRequest(buildJson {
            put("@type", "deleteProfilePhoto")
            put("profile_photo_id", profilePhotoId)
        })
    }

    fun setEmojiStatus(customEmojiId: Long, duration: Int = 0) {
        sendRequest(buildJson {
            put("@type", "setEmojiStatus")
            put("emoji_status", JSONObject().apply {
                put("@type", "emojiStatus")
                put("custom_emoji_id", customEmojiId)
            })
        })
    }

    fun getActiveSessions() {
        sendRequest(buildJson {
            put("@type", "getActiveSessions")
        })
    }

    fun terminateSession(sessionId: Long) {
        sendRequest(buildJson {
            put("@type", "terminateSession")
            put("session_id", sessionId)
        })
    }

    fun terminateAllOtherSessions() {
        sendRequest(buildJson {
            put("@type", "terminateAllOtherSessions")
        })
    }

    fun getConnectedWebsites() {
        sendRequest(buildJson {
            put("@type", "getConnectedWebsites")
        })
    }

    fun disconnectWebsite(websiteId: Long) {
        sendRequest(buildJson {
            put("@type", "disconnectWebsite")
            put("website_id", websiteId)
        })
    }

    fun getPremiumState() {
        sendRequest(buildJson {
            put("@type", "getPremiumState")
        })
    }

    fun getBackgrounds(forDarkTheme: Boolean = false) {
        sendRequest(buildJson {
            put("@type", "getBackgrounds")
            put("for_dark_theme", forDarkTheme)
        })
    }

    fun getChatBackground(chatId: Long) {
        sendRequest(buildJson {
            put("@type", "getChatBackground")
            put("chat_id", chatId)
        })
    }

    fun setChatBackground(chatId: Long, backgroundId: Long) {
        sendRequest(buildJson {
            put("@type", "setChatBackground")
            put("chat_id", chatId)
            put("background", JSONObject().apply {
                put("@type", "inputBackgroundPrevious")
                put("background_id", backgroundId)
            })
        })
    }

    fun deleteChatBackground(chatId: Long, restorePrevious: Boolean = true) {
        sendRequest(buildJson {
            put("@type", "deleteChatBackground")
            put("chat_id", chatId)
            put("restore_previous", restorePrevious)
        })
    }

    fun getMessageThreadHistory(chatId: Long, messageId: Long, fromMessageId: Long = 0, offset: Int = 0, limit: Int = 50) {
        sendRequest(buildJson {
            put("@type", "getMessageThreadHistory")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("from_message_id", fromMessageId)
            put("offset", offset)
            put("limit", limit)
        })
    }

    fun createForumTopic(chatId: Long, name: String, iconColor: Int = 0x6FB9F0, iconCustomEmojiId: Long = 0) {
        sendRequest(buildJson {
            put("@type", "createForumTopic")
            put("chat_id", chatId)
            put("name", name)
            put("icon", JSONObject().apply {
                put("@type", "forumTopicIcon")
                put("color", iconColor)
                if (iconCustomEmojiId > 0) put("custom_emoji_id", iconCustomEmojiId)
            })
        })
    }

    fun editForumTopic(chatId: Long, topicId: Long, name: String? = null, editIcon: Boolean = false, iconCustomEmojiId: Long = 0) {
        sendRequest(buildJson {
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
        sendRequest(buildJson {
            put("@type", "deleteForumTopic")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
        })
    }

    fun getForumTopic(chatId: Long, topicId: Long) {
        sendRequest(buildJson {
            put("@type", "getForumTopic")
            put("chat_id", chatId)
            put("message_thread_id", topicId)
        })
    }

    fun openMessageThread(chatId: Long, threadRootMessageId: Long) {
        sendRequest(buildJson {
            put("@type", "openMessageThread")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun closeMessageThread(chatId: Long, threadRootMessageId: Long) {
        sendRequest(buildJson {
            put("@type", "closeMessageThread")
            put("chat_id", chatId)
            put("message_thread_id", threadRootMessageId)
        })
    }

    fun sendRequest(json: JSONObject) {
        val request = json.toString()
        TdLibNative.nativeTdSend(clientPtr, request)
        Timber.d("TDLib send: %s", request.take(200))
    }

    private fun startReceiveLoop() {
        receiveJob = scope.launch {
            _connectionState.value = ConnectionState.CONNECTING
            while (isActive && clientPtr != 0L) {
                val response = TdLibNative.nativeTdReceive(clientPtr, 1.0)
                if (response != null) {
                    try {
                        handleResponse(JSONObject(response))
                    } catch (e: Exception) {
                        Timber.e(e, "Failed to parse TDLib response")
                    }
                }
            }
            Timber.i("TDLib receive loop stopped")
        }
    }

    @Suppress("CyclomaticComplexMethod")
    private fun handleResponse(json: JSONObject) {
        val type = json.optString("@type", "")
        Timber.d("TDLib receive: %s", type)

        when (type) {
            "updateAuthorizationState" -> handleAuthUpdate(json)
            "updateConnectionState" -> handleConnectionUpdate(json)
            "updateNewMessage" -> handleNewMessage(json)
            "updateChatTitle" -> handleChatTitle(json)
            "updateNewChat" -> handleNewChat(json)
            "updateUserStatus" -> {}
            "updateUser" -> handleUserUpdate(json)
            "updateChatLastMessage" -> handleChatLastMessage(json)
            "updateChatReadInbox" -> {}
            "updateFile" -> {}
            "updateActiveStories" -> handleStoryListUpdate(json)
            "updateStory" -> handleStoryUpdate(json)
            "updateStoryDeleted" -> handleStoryUpdate(json)
            "updateStoryInteraction" -> handleStoryUpdate(json)
            "updateMessageThread" -> handleStoryUpdate(json)
            "updateForumTopic" -> handleStoryUpdate(json)
            "ok" -> {}
            else -> {
                if (type.startsWith("chat") || type.startsWith("chats") || type.startsWith("message")) {
                    Timber.d("TDLib response: %s", json.toString().take(300))
                }
            }
        }
    }

    private fun handleAuthUpdate(json: JSONObject) {
        val authStateObj = json.getJSONObject("authorization_state")
        val authType = authStateObj.getString("@type")

        when (authType) {
            "authorizationStateWaitPhoneNumber" -> {
                _authState.value = TelegramAuthState(AuthStep.WAITING_PHONE_NUMBER)
            }
            "authorizationStateWaitCode" -> {
                val codeInfo = authStateObj.optJSONObject("code_info")
                _authState.value = TelegramAuthState(
                    AuthStep.WAITING_CODE,
                    codeInfo = codeInfo?.toString(),
                    hint = "Code sent"
                )
            }
            "authorizationStateWaitPassword" -> {
                _authState.value = TelegramAuthState(AuthStep.WAITING_PASSWORD)
            }
            "authorizationStateReady" -> {
                _authState.value = TelegramAuthState(AuthStep.READY)
                loadChats()
            }
            "authorizationStateClosed" -> {
                _authState.value = TelegramAuthState(AuthStep.CLOSED)
                _connectionState.value = ConnectionState.DISCONNECTED
            }
            "authorizationStateLoggingOut" -> {
                Timber.i("Logging out...")
            }
        }
    }

    private fun handleConnectionUpdate(json: JSONObject) {
        val state = json.getJSONObject("state").getString("@type")
        _connectionState.value = when (state) {
            "connectionStateReady" -> ConnectionState.CONNECTED
            "connectionStateConnecting" -> ConnectionState.CONNECTING
            "connectionStateUpdating" -> ConnectionState.UPDATING
            "connectionStateConnectingToProxy" -> ConnectionState.CONNECTING
            else -> ConnectionState.DISCONNECTED
        }
        listeners.forEach { it.onConnectionStateChanged(connectionState.value) }
    }

    private fun handleNewMessage(json: JSONObject) {
        val msg = json.optJSONObject("message") ?: return
        val chatId = msg.optLong("chat_id", 0)
        val msgId = msg.optLong("id", 0)

        listeners.forEach { it.onNewMessage(chatId, msgId, msg.toString()) }
    }

    private fun handleChatTitle(json: JSONObject) {
        val chatId = json.optLong("chat_id", 0)
        val title = json.optString("title", "")
        listeners.forEach { it.onChatTitleChanged(chatId, title) }
    }

    private fun handleNewChat(json: JSONObject) {
        listeners.forEach { it.onChatListChanged() }
    }

    private fun handleChatLastMessage(json: JSONObject) {
        listeners.forEach { it.onChatListChanged() }
    }

    private fun handleStoryUpdate(json: JSONObject) {
        val type = json.optString("@type", "")
        listeners.forEach { it.onStoryUpdate(type, json.toString()) }
    }

    private fun handleStoryListUpdate(json: JSONObject) {
        listeners.forEach { it.onStoryListUpdate(json.toString()) }
    }

    private fun handleUserUpdate(json: JSONObject) {
        // Profile updates are broadcast to listeners
    }

    fun addListener(listener: TelegramUpdateListener) {
        listeners.add(listener)
    }

    fun removeListener(listener: TelegramUpdateListener) {
        listeners.remove(listener)
    }

    fun close() {
        receiveJob?.cancel()
        receiveJob = null
        if (clientPtr != 0L) {
            TdLibNative.nativeTdDestroyClient(clientPtr)
            clientPtr = 0
        }
    }

    private fun buildJson(builder: JSONObject.() -> Unit): JSONObject {
        return JSONObject().apply(builder)
    }
}

interface TelegramUpdateListener {
    fun onConnectionStateChanged(state: ConnectionState) {}
    fun onNewMessage(chatId: Long, messageId: Long, rawJson: String) {}
    fun onChatTitleChanged(chatId: Long, title: String) {}
    fun onChatListChanged() {}
    fun onStoryUpdate(type: String, rawJson: String) {}
    fun onStoryListUpdate(activeStoriesJson: String) {}
}

enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    UPDATING
}
