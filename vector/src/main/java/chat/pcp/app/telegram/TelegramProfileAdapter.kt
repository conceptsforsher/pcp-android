package chat.pcp.app.telegram

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONObject

class TelegramProfileAdapter(
    private val profileService: TelegramProfileService
) {

    data class ProfileState(
        val myUser: TelegramFullUser? = null,
        val viewedUser: TelegramFullUser? = null,
        val userFullInfo: TelegramFullUser? = null,
        val profilePhotos: TelegramProfilePhotos? = null,
        val groupsInCommon: List<Long> = emptyList(),
        val activeSessions: List<TelegramSessionInfo> = emptyList(),
        val connectedWebsites: List<TelegramConnectedWebsite> = emptyList(),
        val blockedUsers: List<Long> = emptyList(),
        val closeFriends: List<Long> = emptyList(),
        val isPremium: Boolean = false,
        val isLoading: Boolean = false,
        val error: String? = null
    )

    private val _state = MutableStateFlow(ProfileState())
    val state: StateFlow<ProfileState> = _state

    fun loadMyProfile() {
        _state.value = _state.value.copy(isLoading = true)
        profileService.getMe()
    }

    fun loadUserProfile(userId: Long) {
        _state.value = _state.value.copy(isLoading = true)
        profileService.getUser(userId)
        profileService.getUserFullInfo(userId)
    }

    fun loadUserPhotos(userId: Long, offset: Int = 0, limit: Int = 100) {
        profileService.getUserProfilePhotos(userId, offset, limit)
    }

    fun updateBio(bio: String) {
        profileService.setBio(bio)
    }

    fun updateName(firstName: String, lastName: String = "") {
        profileService.setName(firstName, lastName)
    }

    fun updateUsername(username: String) {
        profileService.setUsername(username)
    }

    fun checkUsernameAvailable(username: String) {
        profileService.checkUsername(username)
    }

    fun setEmojiStatus(customEmojiId: Long, duration: Int = 0) {
        profileService.setEmojiStatus(customEmojiId, duration)
    }

    fun clearEmojiStatus() {
        profileService.clearEmojiStatus()
    }

    fun loadActiveSessions() {
        profileService.getActiveSessions()
    }

    fun terminateSession(sessionId: Long) {
        profileService.terminateSession(sessionId)
    }

    fun terminateAllOtherSessions() {
        profileService.terminateAllOtherSessions()
    }

    fun loadConnectedWebsites() {
        profileService.getConnectedWebsites()
    }

    fun disconnectWebsite(websiteId: Long) {
        profileService.disconnectWebsite(websiteId)
    }

    fun disconnectAllWebsites() {
        profileService.disconnectAllWebsites()
    }

    fun loadGroupsInCommon(userId: Long) {
        profileService.getGroupsInCommon(userId)
    }

    fun loadCloseFriends() {
        profileService.getCloseFriends()
    }

    fun setCloseFriends(userIds: List<Long>) {
        profileService.setCloseFriends(userIds)
    }

    fun loadBlockedUsers() {
        profileService.getBlockedUsers()
    }

    fun blockUser(userId: Long) {
        profileService.blockUser(userId)
        _state.value = _state.value.copy(
            blockedUsers = _state.value.blockedUsers + userId
        )
    }

    fun unblockUser(userId: Long) {
        profileService.unblockUser(userId)
        _state.value = _state.value.copy(
            blockedUsers = _state.value.blockedUsers - userId
        )
    }

    fun loadPremiumState() {
        profileService.getPremiumState()
    }

    fun loadSharedPhotos(chatId: Long, userId: Long = 0, limit: Int = 50) {
        profileService.getSharedPhotos(chatId, userId, limit)
    }

    fun loadSharedVideos(chatId: Long, userId: Long = 0, limit: Int = 50) {
        profileService.getSharedVideos(chatId, userId, limit)
    }

    fun loadSharedDocuments(chatId: Long, userId: Long = 0, limit: Int = 50) {
        profileService.getSharedDocuments(chatId, userId, limit)
    }

    fun loadSharedLinks(chatId: Long, userId: Long = 0, limit: Int = 50) {
        profileService.getSharedLinks(chatId, userId, limit)
    }

    fun loadSharedVoiceNotes(chatId: Long, userId: Long = 0, limit: Int = 50) {
        profileService.getSharedVoiceNotes(chatId, userId, limit)
    }

    fun loadChatBackground(chatId: Long) {
        profileService.getChatBackground(chatId)
    }

    fun setChatBackground(chatId: Long, wallpaperId: Long) {
        profileService.setChatBackground(chatId, wallpaperId)
    }

    fun deleteChatBackground(chatId: Long) {
        profileService.deleteChatBackground(chatId)
    }

    fun handleTdProfileResponse(type: String, json: JSONObject) {
        when (type) {
            "user" -> handleUser(json)
            "userFullInfo" -> handleUserFullInfo(json)
            "userProfilePhotos" -> handleProfilePhotos(json)
            "sessions" -> handleSessions(json)
            "connectedWebsites" -> handleWebsites(json)
            "chats" -> handleGroupsInCommon(json)
            "ok" -> {}
        }
    }

    private fun handleUser(json: JSONObject) {
        val user = profileService.parseUser(json)
        _state.value = _state.value.copy(
            viewedUser = user,
            isLoading = false
        )
    }

    private fun handleUserFullInfo(json: JSONObject) {
        val current = _state.value.viewedUser
        if (current != null) {
            val updated = profileService.parseUserFullInfo(current, json)
            _state.value = _state.value.copy(
                viewedUser = updated,
                userFullInfo = updated,
                isLoading = false
            )
        }
    }

    private fun handleProfilePhotos(json: JSONObject) {
        val totalCount = json.optInt("total_count", 0)
        val photos = json.optJSONArray("photos")?.let { arr ->
            (0 until arr.length()).map { profileService.parseProfilePhoto(arr.getJSONObject(it)) }
        } ?: emptyList()
        _state.value = _state.value.copy(
            profilePhotos = TelegramProfilePhotos(totalCount, photos)
        )
    }

    private fun handleSessions(json: JSONObject) {
        val sessions = json.optJSONArray("sessions")?.let { arr ->
            (0 until arr.length()).map { i ->
                val s = arr.getJSONObject(i)
                TelegramSessionInfo(
                    id = s.optLong("id", 0),
                    isCurrent = s.optBoolean("is_current", false),
                    isPasswordPending = s.optBoolean("is_password_pending", false),
                    apiId = s.optInt("api_id", 0),
                    applicationName = s.optString("application_name", ""),
                    applicationVersion = s.optString("application_version", ""),
                    isOfficialApplication = s.optBoolean("is_official_application", false),
                    deviceModel = s.optString("device_model", ""),
                    platform = s.optString("platform", ""),
                    systemVersion = s.optString("system_version", ""),
                    logInDate = s.optLong("log_in_date", 0),
                    lastActiveDate = s.optLong("last_active_date", 0),
                    ip = s.optString("ip", ""),
                    country = s.optString("country", ""),
                    region = s.optString("region", "")
                )
            }
        } ?: emptyList()
        _state.value = _state.value.copy(activeSessions = sessions)
    }

    private fun handleWebsites(json: JSONObject) {
        val websites = json.optJSONArray("websites")?.let { arr ->
            (0 until arr.length()).map { i ->
                val w = arr.getJSONObject(i)
                TelegramConnectedWebsite(
                    id = w.optLong("id", 0),
                    domainName = w.optString("domain_name", ""),
                    botUserId = w.optLong("bot_user_id", 0),
                    browser = w.optString("browser", ""),
                    platform = w.optString("platform", ""),
                    logInDate = w.optLong("log_in_date", 0),
                    lastActiveDate = w.optLong("last_active_date", 0),
                    ip = w.optString("ip", ""),
                    location = w.optString("location", "")
                )
            }
        } ?: emptyList()
        _state.value = _state.value.copy(connectedWebsites = websites)
    }

    private fun handleGroupsInCommon(json: JSONObject) {
        val chatIds = json.optJSONArray("chat_ids")?.let { arr ->
            (0 until arr.length()).map { arr.getLong(it) }
        } ?: emptyList()
        _state.value = _state.value.copy(groupsInCommon = chatIds)
    }
}
