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

class LemmyHomeViewModel @AssistedInject constructor(
    @Assisted val initialState: LemmyHomeViewState,
    private val sessionHolder: LemmySessionHolder
) : VectorViewModel<LemmyHomeViewState, LemmyHomeAction, LemmyHomeViewEvents>(initialState) {

    @AssistedFactory
    interface Factory : MavericksAssistedViewModelFactory<LemmyHomeViewModel, LemmyHomeViewState> {
        override fun create(initialState: LemmyHomeViewState): LemmyHomeViewModel
    }

    companion object : MavericksViewModelFactory<LemmyHomeViewModel, LemmyHomeViewState> by hiltMavericksViewModelFactory()

    init {
        loadAll()
    }

    override fun handle(action: LemmyHomeAction) {
        when (action) {
            is LemmyHomeAction.Refresh -> loadAll()
            is LemmyHomeAction.SelectPost -> {
                _viewEvents.post(LemmyHomeViewEvents.OpenPost(action.post))
            }
            is LemmyHomeAction.SelectCommunity -> {
                _viewEvents.post(LemmyHomeViewEvents.OpenCommunity(action.community))
            }
            is LemmyHomeAction.VotePost -> handleVote(action)
            is LemmyHomeAction.SwitchTab -> {
                setState { copy(selectedTab = action.tab) }
            }
        }
    }

    private fun handleVote(action: LemmyHomeAction.VotePost) {
        viewModelScope.launch {
            withContext(Dispatchers.IO) {
                val sessions = sessionHolder.getAllSessions()
                sessions.forEach { session ->
                    try {
                        val client = chat.progressive.app.lemmy.LemmyClient()
                        client.init(sessionHolder.getSession(session.instanceUrl)?.let { return@forEach } ?: return@forEach)
                        // Update local state
                        val newPosts = awaitState().posts.map { post ->
                            if (post.id == action.postId) post.copy(myVote = action.score) else post
                        }
                        setState { copy(posts = newPosts) }
                    } catch (e: Exception) {
                        Timber.e(e, "Vote failed")
                    }
                }
            }
        }
    }

    private fun loadAll() {
        setState { copy(loading = true, error = null) }
        viewModelScope.launch {
            try {
                withContext(Dispatchers.IO) {
                    val sessions = sessionHolder.getAllSessions()
                    if (sessions.isEmpty()) {
                        setState { copy(loading = false, error = "No Lemmy sessions. Connect from Settings.") }
                        return@withContext
                    }
                    val allPosts = mutableListOf<LemmyPostItem>()
                    val allCommunities = mutableListOf<LemmyCommunityItem>()

                    sessions.forEach { session ->
                        try {
                            // Load subscribed communities
                            val communities = withContext(Dispatchers.IO) {
                                session.getSubscribedCommunities()
                            }
                            allCommunities.addAll(communities.map { c ->
                                LemmyCommunityItem(
                                    id = c.id.toInt(),
                                    name = c.title,
                                    title = c.title,
                                    description = null,
                                    subscriberCount = 0,
                                    postCount = 0
                                )
                            })

                            // Load front-page posts
                            val client = chat.progressive.app.lemmy.LemmyClient()
                            client.init(session.instanceUrl)
                            val resp = withContext(Dispatchers.IO) {
                                client.listPosts(
                                    "", // auth not needed for browsing
                                    "Subscribed", "Active", 1, 30, showNsfw = false
                                )
                            }
                            val postsArray = resp.optJSONArray("posts") ?: JSONObject.NULL.let { null }
                            if (postsArray != null) {
                                for (i in 0 until postsArray.length()) {
                                    val pv = postsArray.optJSONObject(i) ?: continue
                                    val post = pv.optJSONObject("post") ?: continue
                                    val creator = pv.optJSONObject("creator")
                                    val community = pv.optJSONObject("community")
                                    val counts = pv.optJSONObject("counts")
                                    val myVote = pv.optInt("my_vote", 0)

                                    allPosts.add(LemmyPostItem(
                                        id = post.optInt("id", 0),
                                        title = post.optString("name", ""),
                                        body = post.optString("body", "").takeIf { it.isNotEmpty() } ?: "",
                                        communityName = community?.optString("name", "?") ?: "?",
                                        creatorName = creator?.optString("name",
                                            creator?.optString("display_name", "?")) ?: "?",
                                        score = counts?.optInt("score", 0) ?: 0,
                                        upvotes = counts?.optInt("upvotes", 0) ?: 0,
                                        downvotes = counts?.optInt("downvotes", 0) ?: 0,
                                        commentCount = counts?.optInt("comments", 0) ?: 0,
                                        url = post.optString("url", "").takeIf { it.isNotEmpty() },
                                        published = post.optString("published", ""),
                                        myVote = myVote
                                    ))
                                }
                            }
                        } catch (e: Exception) {
                            Timber.e(e, "Failed to load from session")
                        }
                    }

                    setState {
                        copy(
                            loading = false,
                            posts = allPosts,
                            communities = allCommunities,
                            error = if (allPosts.isEmpty() && allCommunities.isEmpty()) "No content yet" else null
                        )
                    }
                }
            } catch (e: Exception) {
                Timber.e(e, "Load failed")
                setState { copy(loading = false, error = e.message ?: "Unknown error") }
                _viewEvents.post(LemmyHomeViewEvents.Failure(e))
            }
        }
    }
}
