package chat.pcp.app.telegram

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONObject
import timber.log.Timber

class TelegramStoryAdapter(
    private val storyService: TelegramStoryService,
    private val session: TelegramSession
) {

    data class StoryFeedState(
        val activeStories: Map<Long, List<TelegramStory>> = emptyMap(),
        val isLoading: Boolean = false,
        val viewedStories: Set<StoryPosition> = emptySet(),
        val currentStoryIndex: Int = 0,
        val currentChatId: Long = 0,
        val storyInteractions: Map<StoryPosition, StoryInteractionInfo> = emptyMap(),
        val storyViewers: Map<StoryPosition, List<StoryViewer>> = emptyMap(),
        val canSendStory: CanSendStoryResult = CanSendStoryResult(false, false, false, false, true),
        val privacySettings: StoryPrivacy? = null,
        val hasPendingStory: Boolean = false
    ) {
        data class StoryPosition(val chatId: Long, val storyId: Long)
        data class StoryViewer(
            val userId: Long,
            val firstName: String,
            val lastName: String,
            val reaction: String?
        )
    }

    private val _feedState = MutableStateFlow(StoryFeedState())
    val feedState: StateFlow<StoryFeedState> = _feedState

    fun loadActiveStories() {
        _feedState.value = _feedState.value.copy(isLoading = true)
        storyService.getActiveStories(100)
    }

    fun openStory(chatId: Long, storyId: Long) {
        storyService.openStory(chatId, storyId)
        _feedState.value = _feedState.value.copy(
            currentChatId = chatId,
            currentStoryIndex = 0,
            viewedStories = _feedState.value.viewedStories +
                StoryFeedState.StoryPosition(chatId, storyId)
        )
    }

    fun closeStory(chatId: Long, storyId: Long) {
        storyService.closeStory(chatId, storyId)
        _feedState.value = _feedState.value.copy(
            currentChatId = 0,
            currentStoryIndex = 0
        )
    }

    fun nextStory() {
        val state = _feedState.value
        val stories = state.activeStories[state.currentChatId] ?: return
        val nextIdx = state.currentStoryIndex + 1
        if (nextIdx < stories.size) {
            val story = stories[nextIdx]
            storyService.openStory(state.currentChatId, story.id)
            _feedState.value = state.copy(
                currentStoryIndex = nextIdx,
                viewedStories = state.viewedStories +
                    StoryFeedState.StoryPosition(state.currentChatId, story.id)
            )
        }
    }

    fun previousStory() {
        val state = _feedState.value
        val prevIdx = state.currentStoryIndex - 1
        if (prevIdx >= 0) {
            val stories = state.activeStories[state.currentChatId] ?: return
            val story = stories[prevIdx]
            _feedState.value = state.copy(currentStoryIndex = prevIdx)
        }
    }

    fun reactToStory(chatId: Long, storyId: Long, reaction: String?) {
        storyService.setStoryReaction(chatId, storyId, reaction)
    }

    fun loadStoryViewers(chatId: Long, storyId: Long, onlyContacts: Boolean = false) {
        storyService.getStoryViewers(chatId, storyId, onlyContacts = onlyContacts)
    }

    fun loadStoryStatistics(chatId: Long, storyId: Long) {
        storyService.getStoryStatistics(chatId, storyId)
    }

    fun deleteStory(chatId: Long, storyId: Long) {
        storyService.deleteStory(chatId, storyId)
        val state = _feedState.value
        val updated = state.activeStories[chatId]?.filter { it.id != storyId } ?: emptyList()
        _feedState.value = state.copy(
            activeStories = if (updated.isEmpty())
                state.activeStories - chatId
            else
                state.activeStories + (chatId to updated)
        )
    }

    fun togglePinStory(chatId: Long, storyId: Long, isPinned: Boolean) {
        storyService.toggleStoryIsPinned(chatId, storyId, isPinned)
    }

    fun reportStory(chatId: Long, storyId: Long, reason: String = "chatReportReasonSpam") {
        storyService.reportStory(chatId, storyId, reason)
    }

    fun forwardStoryToChat(toChatId: Long, fromChatId: Long, storyId: Long, caption: String? = null) {
        storyService.forwardStory(toChatId, fromChatId, storyId, caption)
    }

    fun checkCanSendStory() {
        val result = storyService.canSendStory()
        _feedState.value = _feedState.value.copy(canSendStory = result)
    }

    fun loadStoryPrivacy() {
        storyService.getStoryPrivacy()
    }

    fun setStoryPrivacy(privacySettings: StoryPrivacySettings, isActive: Boolean = true) {
        storyService.setStoryPrivacy(privacySettings, isActive)
    }

    fun activateStealthMode() {
        storyService.activateStoryStealthMode()
    }

    fun loadArchivedStories(fromStoryId: Long = 0) {
        storyService.getArchivedStories(fromStoryId)
    }

    fun handleTdStoryUpdate(type: String, json: JSONObject) {
        when (type) {
            "updateActiveStories" -> handleActiveStoriesUpdate(json)
            "updateStory" -> handleStoryUpdate(json)
            "updateStoryDeleted" -> handleStoryDeleted(json)
            "updateStoryInteraction" -> handleStoryInteraction(json)
            "updateStoryPrivacy" -> handleStoryPrivacy(json)
        }
    }

    private fun handleActiveStoriesUpdate(json: JSONObject) {
        val activeStories = json.optJSONArray("active_stories") ?: return
        val stories = mutableMapOf<Long, List<TelegramStory>>()

        for (i in 0 until activeStories.length()) {
            val active = activeStories.getJSONObject(i)
            val chatId = active.optLong("chat_id", 0)
            val list = active.optJSONObject("list")
            val storyItems = list?.optJSONArray("stories")?.let { arr ->
                (0 until arr.length()).map { j ->
                    storyService.parseStory(arr.getJSONObject(j))
                }
            } ?: emptyList()
            if (storyItems.isNotEmpty()) {
                stories[chatId] = storyItems
            }
        }

        _feedState.value = _feedState.value.copy(
            activeStories = stories,
            isLoading = false
        )
    }

    private fun handleStoryUpdate(json: JSONObject) {
        val story = json.optJSONObject("story") ?: return
        val parsed = storyService.parseStory(story)
        val chatId = parsed.senderChatId
        val existing = _feedState.value.activeStories[chatId]?.toMutableList() ?: mutableListOf()
        val idx = existing.indexOfFirst { it.id == parsed.id }
        if (idx >= 0) {
            existing[idx] = parsed
        } else {
            existing.add(0, parsed)
        }
        _feedState.value = _feedState.value.copy(
            activeStories = _feedState.value.activeStories + (chatId to existing)
        )
    }

    private fun handleStoryDeleted(json: JSONObject) {
        val chatId = json.optLong("story_sender_chat_id", 0)
        val storyId = json.optLong("story_id", 0)
        val existing = _feedState.value.activeStories[chatId]?.filter { it.id != storyId } ?: emptyList()
        _feedState.value = _feedState.value.copy(
            activeStories = if (existing.isEmpty())
                _feedState.value.activeStories - chatId
            else
                _feedState.value.activeStories + (chatId to existing)
        )
    }

    private fun handleStoryInteraction(json: JSONObject) {
        val chatId = json.optLong("story_sender_chat_id", 0)
        val storyId = json.optLong("story_id", 0)
        val interaction = json.optJSONObject("interaction_info")?.let {
            StoryInteractionInfo(
                viewCount = it.optInt("view_count", 0),
                forwardCount = it.optInt("forward_count", 0),
                reactionCount = it.optInt("reaction_count", 0),
                recentViewerIds = emptyList()
            )
        }
        if (interaction != null) {
            _feedState.value = _feedState.value.copy(
                storyInteractions = _feedState.value.storyInteractions +
                    (StoryFeedState.StoryPosition(chatId, storyId) to interaction)
            )
        }
    }

    private fun handleStoryPrivacy(json: JSONObject) {
        val settings = json.optJSONObject("privacy_settings")?.let {
            StoryPrivacySettings(
                visibility = when (it.optJSONObject("visibility")?.optString("@type", "")) {
                    "storyPrivacySettingsEveryone" -> StoryVisibility.EVERYONE
                    "storyPrivacySettingsContacts" -> StoryVisibility.CONTACTS
                    "storyPrivacySettingsCloseFriends" -> StoryVisibility.CLOSE_FRIENDS
                    "storyPrivacySettingsSelectedUsers" -> StoryVisibility.SELECTED_USERS
                    else -> StoryVisibility.CONTACTS
                },
                isForwardingAllowed = it.optBoolean("is_forwarding_allowed", true),
                exceptionUserIds = emptyList()
            )
        }
        _feedState.value = _feedState.value.copy(
            privacySettings = StoryPrivacy(
                settings = settings ?: StoryPrivacySettings(StoryVisibility.CONTACTS, true, emptyList()),
                isActive = json.optBoolean("is_active", true)
            )
        )
    }
}
