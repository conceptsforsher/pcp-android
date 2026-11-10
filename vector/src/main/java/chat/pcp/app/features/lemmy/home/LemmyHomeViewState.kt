package chat.pcp.app.features.lemmy.home

import com.airbnb.mvrx.MavericksState

data class LemmyHomeViewState(
    val loading: Boolean = false,
    val posts: List<LemmyPostItem> = emptyList(),
    val communities: List<LemmyCommunityItem> = emptyList(),
    val error: String? = null,
    val selectedTab: Int = 0 // 0=posts, 1=communities
) : MavericksState

data class LemmyPostItem(
    val id: Int,
    val title: String,
    val body: String,
    val communityName: String,
    val creatorName: String,
    val score: Int,
    val upvotes: Int,
    val downvotes: Int,
    val commentCount: Int,
    val url: String?,
    val published: String,
    val myVote: Int = 0 // -1, 0, 1
)

data class LemmyCommunityItem(
    val id: Int,
    val name: String,
    val title: String,
    val description: String?,
    val subscriberCount: Int,
    val postCount: Int
)
