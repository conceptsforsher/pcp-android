package chat.progressive.app.features.lemmy.home

import com.airbnb.mvrx.MavericksState

data class LemmyPostDetailState(
    val loading: Boolean = false,
    val post: LemmyPostItem? = null,
    val comments: List<LemmyCommentItem> = emptyList(),
    val error: String? = null,
    val replyText: String = ""
) : MavericksState

data class LemmyCommentItem(
    val id: Int,
    val postId: Int,
    val parentId: Int,     // 0 = top-level
    val content: String,
    val authorName: String,
    val score: Int,
    val myVote: Int,       // -1, 0, 1
    val childCount: Int,
    val published: String,
    val depth: Int = 0     // 0 = top-level, 1 = child of top-level, etc.
)
