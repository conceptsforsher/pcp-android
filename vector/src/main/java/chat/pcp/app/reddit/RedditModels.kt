package chat.pcp.app.reddit

import chat.pcp.app.protocol.ProtocolType

data class RedditPost(
    val id: String,
    val fullname: String = "",
    val title: String = "",
    val author: String = "",
    val subreddit: String = "",
    val url: String = "",
    val thumbnail: String = "",
    val domain: String = "",
    val permalink: String = "",
    val selftext: String = "",
    val score: Int = 0,
    val commentCount: Int = 0,
    val nsfw: Boolean = false,
    val spoiler: Boolean = false,
    val stickied: Boolean = false,
    val isSelf: Boolean = false,
    val isGallery: Boolean = false,
    val isVideo: Boolean = false,
    val createdUtc: Long = 0
)

data class RedditComment(
    val id: String,
    val fullname: String = "",
    val author: String = "",
    val body: String = "",
    val score: Int = 0,
    val depth: Int = 0,
    val stickied: Boolean = false,
    val isSubmitter: Boolean = false,
    val createdUtc: Long = 0
)

data class RedditSubreddit(
    val name: String,
    val title: String = "",
    val description: String = "",
    val subscriberCount: Int = 0,
    val activeUserCount: Int = 0,
    val over18: Boolean = false
)

data class RedditAccount(
    val username: String,
    val accessToken: String = "",
    val refreshToken: String = "",
    val isAnonymous: Boolean = true
)

class AfterHolder {
    var value: String = ""
}
