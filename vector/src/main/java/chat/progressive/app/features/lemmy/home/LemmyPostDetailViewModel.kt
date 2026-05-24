package chat.progressive.app.features.lemmy.home

import androidx.lifecycle.viewModelScope
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

class LemmyPostDetailViewModel @AssistedInject constructor(
    @Assisted val initialState: LemmyPostDetailState,
    private val sessionHolder: LemmySessionHolder
) : VectorViewModel<LemmyPostDetailState, LemmyPostDetailAction, LemmyHomeViewEvents>(
    initialState
) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<LemmyPostDetailViewModel, LemmyPostDetailState> {
        override fun create(initialState: LemmyPostDetailState): LemmyPostDetailViewModel
    }

    companion object : MavericksViewModelFactory<LemmyPostDetailViewModel, LemmyPostDetailState> by hiltMavericksViewModelFactory()

    init {
        loadPostAndComments()
    }

    override fun handle(action: LemmyPostDetailAction) {
        when (action) {
            is LemmyPostDetailAction.Refresh -> loadPostAndComments()
            is LemmyPostDetailAction.VotePost -> handleVotePost(action.score)
            is LemmyPostDetailAction.VoteComment -> handleVoteComment(action.commentId, action.score)
            is LemmyPostDetailAction.SetReplyText -> setState { copy(replyText = action.text) }
            is LemmyPostDetailAction.SendReply -> handleSendReply()
            is LemmyPostDetailAction.ReplyToComment -> {
                setState { copy(replyText = "Replying to #${action.commentId}: ") }
            }
        }
    }

    private fun loadPostAndComments() {
        val post = initialState.post ?: return
        setState { copy(loading = true, error = null) }
        viewModelScope.launch {
            try {
                withContext(Dispatchers.IO) {
                    val sessions = sessionHolder.getAllSessions()
                    if (sessions.isEmpty()) {
                        setState { copy(loading = false, error = "No Lemmy session") }
                        return@withContext
                    }
                    val session = sessions.first()
                    val client = chat.progressive.app.lemmy.LemmyClient()
                    client.init(session.instanceUrl)

                    val jwt = getSessionJwt(session)
                    val resp = withContext(Dispatchers.IO) {
                        client.listComments(jwt, post.id, "Hot", 8, 50)
                    }
                    val commentsArray = resp.optJSONArray("comments")
                    val comments = mutableListOf<LemmyCommentItem>()
                    if (commentsArray != null) {
                        for (i in 0 until commentsArray.length()) {
                            val cv = commentsArray.optJSONObject(i) ?: continue
                            val comment = cv.optJSONObject("comment") ?: continue
                            val creator = cv.optJSONObject("creator")
                            val counts = cv.optJSONObject("counts")
                            val myVote = cv.optInt("my_vote", 0)

                            comments.add(LemmyCommentItem(
                                id = comment.optInt("id", 0),
                                postId = comment.optInt("post_id", 0),
                                parentId = comment.optString("parent_id", "0").let { p ->
                                    if (p.isNotEmpty()) p.toIntOrNull() ?: 0 else 0
                                },
                                content = comment.optString("content", ""),
                                authorName = creator?.optString("name",
                                    creator?.optString("display_name", "?")) ?: "?",
                                score = counts?.optInt("score", 0) ?: 0,
                                myVote = myVote,
                                childCount = counts?.optInt("child_count", 0) ?: 0,
                                published = comment.optString("published", ""),
                                depth = comment.optString("path", "0").count { it == '.' }
                            ))
                        }
                    }
                    setState { copy(loading = false, comments = comments, error = null) }
                }
            } catch (e: Exception) {
                Timber.e(e, "Failed to load post")
                setState { copy(loading = false, error = e.message) }
            }
        }
    }

    private fun handleVotePost(score: Int) {
        val post = awaitState().post ?: return
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                val sessions = sessionHolder.getAllSessions()
                sessions.firstOrNull()?.let { session ->
                    val client = chat.progressive.app.lemmy.LemmyClient()
                    client.init(session.instanceUrl)
                    val jwt = getSessionJwt(session)
                    try {
                        client.likePost(jwt, post.id, score)
                        setState { copy(post = post.copy(myVote = score)) }
                    } catch (e: Exception) {
                        Timber.e(e, "Vote failed")
                    }
                }
            }
        }
    }

    private fun handleVoteComment(commentId: Int, score: Int) {
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                val sessions = sessionHolder.getAllSessions()
                sessions.firstOrNull()?.let { session ->
                    val client = chat.progressive.app.lemmy.LemmyClient()
                    client.init(session.instanceUrl)
                    val jwt = getSessionJwt(session)
                    try {
                        client.likeComment(jwt, commentId, score)
                        val newComments = awaitState().comments.map {
                            if (it.id == commentId) it.copy(myVote = score) else it
                        }
                        setState { copy(comments = newComments) }
                    } catch (e: Exception) {
                        Timber.e(e, "Comment vote failed")
                    }
                }
            }
        }
    }

    private fun handleSendReply() {
        val text = awaitState().replyText.trim()
        if (text.isEmpty()) return
        val post = awaitState().post ?: return

        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                val sessions = sessionHolder.getAllSessions()
                sessions.firstOrNull()?.let { session ->
                    val client = chat.progressive.app.lemmy.LemmyClient()
                    client.init(session.instanceUrl)
                    val jwt = getSessionJwt(session)
                    try {
                        client.createComment(jwt, post.id, text)
                        setState { copy(replyText = "") }
                        loadPostAndComments() // refresh
                    } catch (e: Exception) {
                        Timber.e(e, "Reply failed")
                    }
                }
            }
        }
    }

    private fun getSessionJwt(session: chat.progressive.app.lemmy.LemmySession): String {
        return "" // JWT stored internally, use empty for now (public browse)
    }
}
