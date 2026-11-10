package chat.pcp.app.telegram

import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.IProtocolRoom
import chat.pcp.app.protocol.ProtocolType

class TelegramRoom(
    override val id: Long,
    override val title: String,
    private val lastText: String?,
    private val lastTime: Long?,
    private val unread: Int,
    override val isGroup: Boolean,
    val tdClient: TdLibClient
) : IProtocolRoom {

    override val lastMessage: String? get() = lastText
    override val lastMessageTime: Long? get() = lastTime
    override val unreadCount: Int get() = unread
    override val avatarUrl: String? = null
    override val protocolType: ProtocolType = ProtocolType.TELEGRAM

    val pollService = TelegramPollService(tdClient)
    val reactionService = TelegramReactionService(tdClient)
    val forwardService = TelegramForwardService(tdClient)
    val stickerService = TelegramStickerService(tdClient)
    val fileService = TelegramFileService(tdClient)
    val scheduledService = TelegramScheduledService(tdClient)
    val locationService = TelegramLocationService(tdClient)
    val mediaService = TelegramMediaService(tdClient)
    val threadService = TelegramThreadService(tdClient)

    override suspend fun getMessages(limit: Int, fromMessageId: Long): List<IProtocolMessage> {
        tdClient.getChatHistory(id, limit, fromMessageId)
        return emptyList()
    }

    override suspend fun sendTextMessage(text: String) {
        tdClient.sendChatMessage(id, text)
    }

    override suspend fun markAsRead() {
        tdClient.viewMessages(id, longArrayOf())
    }

    override suspend fun leave() {
        tdClient.sendRequest(
            org.json.JSONObject().apply {
                put("@type", "leaveChat")
                put("chat_id", id)
            }
        )
    }

    fun deleteMessage(messageId: Long, revoke: Boolean = true) {
        tdClient.deleteMessages(id, longArrayOf(messageId), revoke)
    }

    fun editMessage(messageId: Long, newText: String) {
        tdClient.editMessageText(id, messageId, newText)
    }

    fun forwardMessages(toChatId: Long, messageIds: LongArray, sendCopy: Boolean = false) {
        forwardService.forwardMessages(toChatId, id, messageIds, sendCopy)
    }

    fun pinMessage(messageId: Long, disableNotification: Boolean = false) {
        tdClient.pinChatMessage(id, messageId, disableNotification)
    }

    fun unpinMessage(messageId: Long = 0) {
        tdClient.unpinChatMessage(id, messageId)
    }

    fun searchMessages(query: String, limit: Int = 50, fromMessageId: Long = 0) {
        tdClient.searchChatMessages(id, query, limit, fromMessageId)
    }

    fun getMember(userId: Long) {
        tdClient.getChatMember(id, userId)
    }

    fun searchMembers(query: String, limit: Int = 50) {
        tdClient.searchChatMembers(id, query, limit)
    }

    fun getScheduledMessages() {
        tdClient.getScheduledMessages(id)
    }

    fun getPinnedMessage() {
        tdClient.getChatPinnedMessage(id)
    }

    fun setChatTitle(title: String) {
        tdClient.setChatTitle(id, title)
    }

    fun getMessageThread(messageId: Long) {
        tdClient.getMessageThread(id, messageId)
    }

    fun getThreadHistory(messageId: Long, fromMessageId: Long = 0) {
        tdClient.getMessageThreadHistory(id, messageId, fromMessageId, 0, 50)
    }

    fun replyInThread(threadRootMessageId: Long, text: String, replyToMessageId: Long = 0) {
        threadService.sendThreadReply(id, threadRootMessageId, text, replyToMessageId)
    }

    fun getForumTopics(query: String = "") {
        threadService.getForumTopics(id, query)
    }

    fun createForumTopic(name: String) {
        tdClient.createForumTopic(id, name)
    }

    fun deleteForumTopic(topicId: Long) {
        tdClient.deleteForumTopic(id, topicId)
    }

    fun toggleForumTopicClosed(topicId: Long, isClosed: Boolean) {
        threadService.toggleForumTopicIsClosed(id, topicId, isClosed)
    }

    fun toggleForumTopicPinned(topicId: Long, isPinned: Boolean) {
        threadService.toggleForumTopicIsPinned(id, topicId, isPinned)
    }
}
