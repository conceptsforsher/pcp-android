package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

data class TelegramStory(
    val id: Long,
    val senderChatId: Long,
    val date: Long,
    val isBeingViewed: Boolean,
    val isEdited: Boolean,
    val isPinned: Boolean,
    val isVisibleOnlyForSelf: Boolean,
    val canBeForwarded: Boolean,
    val canBeReplied: Boolean,
    val canGetStatistics: Boolean,
    val canToggleIsPinned: Boolean,
    val content: TelegramStoryContent,
    val interactionInfo: StoryInteractionInfo?,
    val chosenReactionType: String?,
    val privacySettings: StoryPrivacySettings?
)

sealed class TelegramStoryContent {
    data class Photo(val photo: TelegramPhoto) : TelegramStoryContent()
    data class Video(
        val video: TelegramVideo,
        val isAnimation: Boolean = false,
        val altDocument: TelegramDocument? = null
    ) : TelegramStoryContent()
    data class Unsupported(val type: String) : TelegramStoryContent()
}

data class StoryInteractionInfo(
    val viewCount: Int,
    val forwardCount: Int,
    val reactionCount: Int,
    val recentViewerIds: List<Long>
)

data class StoryPrivacySettings(
    val visibility: StoryVisibility,
    val isForwardingAllowed: Boolean,
    val exceptionUserIds: List<Long>
)

enum class StoryVisibility {
    EVERYONE,
    CONTACTS,
    CLOSE_FRIENDS,
    SELECTED_USERS
}

data class StoryList(
    val totalCount: Int,
    val stories: List<TelegramStory>,
    val nextOrder: String
)

data class ActiveStories(
    val chatId: Long,
    val list: StoryList,
    val maxReadStoryId: Long,
    val isPinned: Boolean
)

data class StoryInteraction(
    val storyId: Long,
    val storySenderChatId: Long,
    val type: String,
    val viewerChatId: Long
)

data class StoryStatistics(
    val storyInteractionInfo: StoryInteractionInfo,
    val heatmap: ByteArray?
)

data class CanSendStoryResult(
    val canSend: Boolean,
    val activeStoryLimitReached: Boolean,
    val weeklyLimitReached: Boolean,
    val monthlyLimitReached: Boolean,
    val needPremium: Boolean
)

data class StoryPrivacy(
    val settings: StoryPrivacySettings,
    val isActive: Boolean
)
