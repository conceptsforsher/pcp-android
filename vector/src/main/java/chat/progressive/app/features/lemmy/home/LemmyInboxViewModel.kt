package chat.progressive.app.features.lemmy.home

import androidx.lifecycle.viewModelScope
import chat.progressive.app.lemmy.LemmyClient
import chat.progressive.app.lemmy.LemmySessionHolder
import com.airbnb.mvrx.MavericksViewModelFactory
import dagger.assisted.Assisted
import dagger.assisted.AssistedFactory
import dagger.assisted.AssistedInject
import im.vector.app.core.di.MavericksAssistedViewModelFactory
import im.vector.app.core.di.hiltMavericksViewModelFactory
import im.vector.app.core.platform.VectorViewModel
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import org.json.JSONObject
import timber.log.Timber

class LemmyInboxViewModel @AssistedInject constructor(
    @Assisted val initialState: LemmyInboxState,
    private val sessionHolder: LemmySessionHolder
) : VectorViewModel<LemmyInboxState, LemmyInboxAction, LemmyHomeViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<LemmyInboxViewModel, LemmyInboxState> {
        override fun create(initialState: LemmyInboxState): LemmyInboxViewModel
    }

    companion object : MavericksViewModelFactory<LemmyInboxViewModel, LemmyInboxState> by hiltMavericksViewModelFactory()

    init {
        loadInbox()
    }

    override fun handle(action: LemmyInboxAction) {
        when (action) {
            is LemmyInboxAction.Refresh -> loadInbox()
            is LemmyInboxAction.MarkRead -> markRead(action.item)
            is LemmyInboxAction.MarkAllRead -> markAllRead()
        }
    }

    private fun loadInbox() {
        setState { copy(loading = true, error = null) }
        viewModelScope.launch {
            try {
                withContext(Dispatchers.IO) {
                    val sessions = sessionHolder.getAllSessions()
                    if (sessions.isEmpty()) {
                        setState { copy(loading = false, error = "No session") }
                        return@withContext
                    }
                    val session = sessions.first()
                    val client = LemmyClient()
                    client.init(session.instanceUrl)

                    val replies = mutableListOf<LemmyInboxItem>()
                    val mentions = mutableListOf<LemmyInboxItem>()
                    val messages = mutableListOf<LemmyInboxItem>()
                    var unread = 0

                    // Load replies
                    try {
                        val repliesResp = withContext(Dispatchers.IO) {
                            client.getReplies("", "New", 1, 30, false)
                        }
                        val arr = repliesResp.optJSONArray("replies")
                        if (arr != null) {
                            for (i in 0 until arr.length()) {
                                val r = arr.optJSONObject(i) ?: continue
                                val comment = r.optJSONObject("comment")
                                val creator = r.optJSONObject("creator")
                                val post = r.optJSONObject("post")
                                val community = r.optJSONObject("community")
                                val read = r.optBoolean("read", true)
                                if (!read) unread++
                                replies.add(LemmyInboxItem(
                                    id = r.optInt("id", 0),
                                    type = InboxType.REPLY,
                                    content = comment?.optString("content", "") ?: "",
                                    senderName = creator?.optString("name", "?") ?: "?",
                                    communityName = community?.optString("name", "") ?: "",
                                    postName = post?.optString("name", "") ?: "",
                                    published = comment?.optString("published", "") ?: "",
                                    isRead = read
                                ))
                            }
                        }
                    } catch (e: Exception) { Timber.e(e, "Replies load failed") }

                    // Load mentions
                    try {
                        val mentionsResp = withContext(Dispatchers.IO) {
                            client.getMentions("", "New", 1, 30, false)
                        }
                        val arr = mentionsResp.optJSONArray("mentions")
                        if (arr != null) {
                            for (i in 0 until arr.length()) {
                                val m = arr.optJSONObject(i) ?: continue
                                val comment = m.optJSONObject("comment")
                                val creator = m.optJSONObject("creator")
                                val post = m.optJSONObject("post")
                                val community = m.optJSONObject("community")
                                val read = m.optBoolean("read", true)
                                if (!read) unread++
                                mentions.add(LemmyInboxItem(
                                    id = m.optInt("id", 0),
                                    type = InboxType.MENTION,
                                    content = comment?.optString("content", "") ?: "",
                                    senderName = creator?.optString("name", "?") ?: "?",
                                    communityName = community?.optString("name", "") ?: "",
                                    postName = post?.optString("name", "") ?: "",
                                    published = comment?.optString("published", "") ?: "",
                                    isRead = read
                                ))
                            }
                        }
                    } catch (e: Exception) { Timber.e(e, "Mentions load failed") }

                    // Load private messages
                    try {
                        val msgsResp = withContext(Dispatchers.IO) {
                            client.listPrivateMessages("", 1, 30, false)
                        }
                        val arr = msgsResp.optJSONArray("private_messages")
                        if (arr != null) {
                            for (i in 0 until arr.length()) {
                                val pm = arr.optJSONObject(i) ?: continue
                                val msg = pm.optJSONObject("private_message")
                                val creator = pm.optJSONObject("creator")
                                val read = pm.optBoolean("read", true)
                                if (!read) unread++
                                messages.add(LemmyInboxItem(
                                    id = msg?.optInt("id", 0) ?: 0,
                                    type = InboxType.MESSAGE,
                                    content = msg?.optString("content", "") ?: "",
                                    senderName = creator?.optString("name", "?") ?: "?",
                                    communityName = "",
                                    postName = "",
                                    published = msg?.optString("published", "") ?: "",
                                    isRead = read
                                ))
                            }
                        }
                    } catch (e: Exception) { Timber.e(e, "Messages load failed") }

                    setState {
                        copy(
                            loading = false, replies = replies,
                            mentions = mentions, messages = messages,
                            unreadCount = unread, error = null
                        )
                    }
                }
            } catch (e: Exception) {
                Timber.e(e, "Inbox load failed")
                setState { copy(loading = false, error = e.message) }
            }
        }
    }

    private fun markRead(item: LemmyInboxItem) {
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                val sessions = sessionHolder.getAllSessions()
                sessions.firstOrNull()?.let { session ->
                    val client = LemmyClient()
                    client.init(session.instanceUrl)
                    try {
                        when (item.type) {
                            InboxType.REPLY, InboxType.MENTION ->
                                client.markMentionRead("", item.id, true)
                            InboxType.MESSAGE ->
                                client.markPrivateMessageRead("", item.id, true)
                        }
                    } catch (e: Exception) { Timber.e(e, "Mark read failed") }
                }
            }
        }
    }

    private fun markAllRead() {
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                val sessions = sessionHolder.getAllSessions()
                sessions.firstOrNull()?.let { session ->
                    val client = LemmyClient()
                    client.init(session.instanceUrl)
                    try { client.markAllAsRead("") } catch (e: Exception) { Timber.e(e, "Mark all failed") }
                }
            }
            loadInbox()
        }
    }
}
