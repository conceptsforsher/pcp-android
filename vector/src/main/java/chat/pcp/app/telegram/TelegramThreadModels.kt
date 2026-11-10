package chat.pcp.app.telegram

data class TelegramThread(
    val chatId: Long,
    val messageThreadId: Long,
    val rootMessageId: Long,
    val title: String,
    val replyCount: Int,
    val lastMessageId: Long,
    val lastReadInboxMessageId: Long,
    val lastReadOutboxMessageId: Long,
    val isOutgoing: Boolean,
    val unreadCount: Int,
    val unreadMentionCount: Int,
    val unreadReactionCount: Int
)

data class TelegramForumTopic(
    val id: Long,
    val chatId: Long,
    val name: String,
    val creationDate: Long,
    val creatorUserId: Long,
    val icon: TelegramForumTopicIcon?,
    val isClosed: Boolean,
    val isPinned: Boolean,
    val isGeneral: Boolean,
    val isHidden: Boolean,
    val unreadCount: Int,
    val unreadMentionCount: Int,
    val unreadReactionCount: Int,
    val lastMessageId: Long,
    val lastReadInboxMessageId: Long,
    val lastReadOutboxMessageId: Long,
    val notificationSettings: TelegramNotificationSettings?
)

data class TelegramForumTopicIcon(
    val customEmojiId: Long,
    val color: Int
)

data class TelegramForumTopics(
    val totalCount: Int,
    val topics: List<TelegramForumTopic>,
    val nextOffset: Long
)

data class TelegramThreadMessages(
    val totalCount: Int,
    val messages: List<TelegramRichMessage>,
    val nextFromMessageId: Long
)

data class TelegramForumTopicDraft(
    val topicId: Long,
    val text: String,
    val date: Long
)
