package chat.progressive.app.features.lemmy.home

import com.airbnb.mvrx.MavericksState

data class LemmyInboxState(
    val loading: Boolean = false,
    val replies: List<LemmyInboxItem> = emptyList(),
    val mentions: List<LemmyInboxItem> = emptyList(),
    val messages: List<LemmyInboxItem> = emptyList(),
    val unreadCount: Int = 0,
    val error: String? = null
) : MavericksState

data class LemmyInboxItem(
    val id: Int,
    val type: InboxType,
    val content: String,
    val senderName: String,
    val communityName: String,
    val postName: String,
    val published: String,
    val isRead: Boolean
)

enum class InboxType { REPLY, MENTION, MESSAGE }
