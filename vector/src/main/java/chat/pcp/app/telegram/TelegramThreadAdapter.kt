package chat.pcp.app.telegram

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONObject
import timber.log.Timber

class TelegramThreadAdapter(
    private val threadService: TelegramThreadService
) {

    data class ThreadState(
        val currentThread: TelegramThread? = null,
        val threadMessages: List<TelegramRichMessage> = emptyList(),
        val threads: Map<Long, TelegramThread> = emptyMap(),
        val forumTopics: TelegramForumTopics? = null,
        val currentTopic: TelegramForumTopic? = null,
        val draftText: String = "",
        val isLoading: Boolean = false,
        val hasMoreMessages: Boolean = false,
        val totalCount: Int = 0,
        val nextFromMessageId: Long = 0
    ) {
        val unreadThreadCount: Int get() = threads.values.count { it.unreadCount > 0 }
    }

    private val _state = MutableStateFlow(ThreadState())
    val state: StateFlow<ThreadState> = _state

    fun openThread(chatId: Long, messageId: Long) {
        _state.value = _state.value.copy(isLoading = true)
        threadService.getMessageThread(chatId, messageId)
        threadService.getMessageThreadHistory(chatId, messageId)
        threadService.openMessageThread(chatId, messageId)
    }

    fun loadThreadHistory(
        chatId: Long,
        messageId: Long,
        fromMessageId: Long = 0,
        limit: Int = 50
    ) {
        _state.value = _state.value.copy(isLoading = true)
        threadService.getMessageThreadHistory(chatId, messageId, fromMessageId, limit = limit)
    }

    fun loadMoreThreadHistory() {
        val current = _state.value
        val thread = current.currentThread ?: return
        val nextId = current.nextFromMessageId.takeIf { it > 0 } ?: return
        threadService.getMessageThreadHistory(
            thread.chatId,
            thread.rootMessageId,
            fromMessageId = nextId
        )
    }

    fun replyToThread(chatId: Long, threadRootMessageId: Long, text: String, replyToMessageId: Long = 0) {
        threadService.sendThreadReply(chatId, threadRootMessageId, text, replyToMessageId)
    }

    fun sendThreadPhoto(chatId: Long, threadRootMessageId: Long, filePath: String, caption: String? = null) {
        threadService.sendThreadPhoto(chatId, threadRootMessageId, filePath, caption)
    }

    fun reactToThreadMessage(chatId: Long, threadRootMessageId: Long, messageId: Long, reaction: String) {
        threadService.reactToThreadMessage(chatId, threadRootMessageId, messageId, reaction)
    }

    fun forwardToThread(chatId: Long, threadRootMessageId: Long, fromChatId: Long, messageIds: LongArray) {
        threadService.forwardToThread(chatId, threadRootMessageId, fromChatId, messageIds)
    }

    fun searchInThread(chatId: Long, threadRootMessageId: Long, query: String) {
        threadService.searchInThread(chatId, threadRootMessageId, query)
    }

    fun closeThread(chatId: Long, threadRootMessageId: Long) {
        threadService.closeMessageThread(chatId, threadRootMessageId)
        _state.value = ThreadState()
    }

    fun saveDraft(chatId: Long, threadRootMessageId: Long, text: String) {
        threadService.saveThreadDraft(chatId, threadRootMessageId, text)
        _state.value = _state.value.copy(draftText = text)
    }

    fun loadDraft(chatId: Long, threadRootMessageId: Long) {
        threadService.getCurrentThreadDraft(chatId, threadRootMessageId)
    }

    fun clearDraft(chatId: Long, threadRootMessageId: Long) {
        threadService.clearThreadDraft(chatId, threadRootMessageId)
        _state.value = _state.value.copy(draftText = "")
    }

    fun markThreadRead(chatId: Long, threadRootMessageId: Long) {
        threadService.readAllThreadMessages(chatId, threadRootMessageId)
        threadService.readAllThreadMentions(chatId, threadRootMessageId)
        threadService.readAllThreadReactions(chatId, threadRootMessageId)
    }

    fun loadForumTopics(chatId: Long, query: String = "") {
        _state.value = _state.value.copy(isLoading = true)
        threadService.getForumTopics(chatId, query)
    }

    fun openForumTopic(chatId: Long, topicId: Long) {
        _state.value = _state.value.copy(isLoading = true)
        threadService.getForumTopic(chatId, topicId)
        threadService.getMessageThreadHistory(chatId, topicId)
    }

    fun createForumTopic(chatId: Long, name: String, iconCustomEmojiId: Long = 0) {
        threadService.createForumTopic(chatId, name, iconCustomEmojiId = iconCustomEmojiId)
    }

    fun editForumTopic(chatId: Long, topicId: Long, name: String) {
        threadService.editForumTopic(chatId, topicId, name = name)
    }

    fun deleteForumTopic(chatId: Long, topicId: Long) {
        threadService.deleteForumTopic(chatId, topicId)
    }

    fun toggleTopicClosed(chatId: Long, topicId: Long, isClosed: Boolean) {
        threadService.toggleForumTopicIsClosed(chatId, topicId, isClosed)
    }

    fun toggleTopicPinned(chatId: Long, topicId: Long, isPinned: Boolean) {
        threadService.toggleForumTopicIsPinned(chatId, topicId, isPinned)
    }

    fun toggleTopicHidden(chatId: Long, topicId: Long, isHidden: Boolean) {
        threadService.toggleForumTopicIsHidden(chatId, topicId, isHidden)
    }

    fun pinThreadMessage(chatId: Long, threadRootMessageId: Long, messageId: Long) {
        threadService.pinThreadMessage(chatId, threadRootMessageId, messageId)
    }

    fun unpinThreadMessage(chatId: Long, threadRootMessageId: Long, messageId: Long = 0) {
        threadService.unpinThreadMessage(chatId, threadRootMessageId, messageId)
    }

    fun handleTdThreadUpdate(type: String, json: JSONObject) {
        when (type) {
            "messageThreadInfo" -> handleThreadInfo(json)
            "messages" -> handleThreadMessages(json)
            "forumTopics" -> handleForumTopics(json)
            "forumTopicInfo" -> handleForumTopicInfo(json)
            "updateMessageThread" -> handleThreadUpdate(json)
            "updateForumTopic" -> handleForumTopicUpdate(json)
            "messageThreadDraftMessage" -> handleDraftLoaded(json)
        }
    }

    private fun handleThreadInfo(json: JSONObject) {
        val thread = threadService.parseThread(json)
        _state.value = _state.value.copy(
            currentThread = thread,
            isLoading = false
        )
    }

    private fun handleThreadMessages(json: JSONObject) {
        val messages = json.optJSONArray("messages")?.let { arr ->
            (0 until arr.length()).map { i ->
                TelegramContentParser.parseMessage(arr.getJSONObject(i))
            }
        } ?: emptyList()
        val totalCount = json.optInt("total_count", messages.size)
        val nextFromId = messages.lastOrNull()?.id ?: 0L

        _state.value = _state.value.copy(
            threadMessages = if (nextFromId == _state.value.nextFromMessageId) {
                _state.value.threadMessages + messages
            } else {
                messages
            },
            totalCount = totalCount,
            hasMoreMessages = messages.size >= 50,
            nextFromMessageId = nextFromId,
            isLoading = false
        )
    }

    private fun handleForumTopics(json: JSONObject) {
        val topics = threadService.parseForumTopics(json)
        _state.value = _state.value.copy(
            forumTopics = topics,
            isLoading = false
        )
    }

    private fun handleForumTopicInfo(json: JSONObject) {
        val topic = threadService.parseForumTopic(json)
        _state.value = _state.value.copy(
            currentTopic = topic,
            isLoading = false
        )
    }

    private fun handleThreadUpdate(json: JSONObject) {
        try {
            val thread = threadService.parseThread(json)
            _state.value = _state.value.copy(
                threads = _state.value.threads + (thread.messageThreadId to thread),
                currentThread = if (_state.value.currentThread?.messageThreadId == thread.messageThreadId) thread else _state.value.currentThread
            )
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse thread update")
        }
    }

    private fun handleForumTopicUpdate(json: JSONObject) {
        try {
            val topic = threadService.parseForumTopic(json)
            val existing = _state.value.forumTopics
            if (existing != null) {
                val updatedTopics = existing.topics.map { if (it.id == topic.id) topic else it }
                _state.value = _state.value.copy(
                    forumTopics = existing.copy(topics = updatedTopics)
                )
            }
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse forum topic update")
        }
    }

    private fun handleDraftLoaded(json: JSONObject) {
        val draft = json.optJSONObject("draft_message")
        val text = draft?.optJSONObject("input_message_text")
            ?.optJSONObject("text")
            ?.optString("text", "") ?: ""
        _state.value = _state.value.copy(draftText = text)
    }
}
