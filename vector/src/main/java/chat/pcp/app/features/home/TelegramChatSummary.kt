package chat.pcp.app.features.home

data class TelegramChatSummary(
    val id: Long,
    val title: String,
    val lastMessage: String,
    val lastMessageTime: Long,
    val unreadCount: Int,
    val isGroup: Boolean,
    val isChannel: Boolean,
    val isPinned: Boolean
)
