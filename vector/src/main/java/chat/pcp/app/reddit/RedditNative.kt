package chat.pcp.app.reddit

object RedditNative {
    private var loaded = false

    @JvmStatic
    fun ensureLoaded(): Boolean {
        if (loaded) return true
        try {
            System.loadLibrary("pcp_chat_reddit")
            loaded = true
            return true
        } catch (e: UnsatisfiedLinkError) {
            return false
        }
    }

    val isAvailable: Boolean get() = ensureLoaded()

    // URL builders
    @JvmStatic external fun nativeFrontpageUrl(sort: String, after: String, limit: Int): String
    @JvmStatic external fun nativeSubredditUrl(sub: String, sort: String, after: String): String
    @JvmStatic external fun nativeCommentsUrl(sub: String, postId: String): String
    @JvmStatic external fun nativeSearchSubredditsUrl(query: String): String
    @JvmStatic external fun nativeSubredditAboutUrl(sub: String): String
    @JvmStatic external fun nativeUserPostsUrl(user: String, sort: String): String

    // OAuth action URLs
    @JvmStatic external fun nativeVoteUrl(): String
    @JvmStatic external fun nativeSaveUrl(saveFlag: Boolean): String
    @JvmStatic external fun nativeHideUrl(): String
    @JvmStatic external fun nativeCommentUrl(): String
    @JvmStatic external fun nativeTokenUrl(): String

    // OAuth body builders
    @JvmStatic external fun nativeBuildVoteBody(id: String, dir: Int): String
    @JvmStatic external fun nativeBuildSaveBody(id: String): String
    @JvmStatic external fun nativeBuildHideBody(id: String): String
    @JvmStatic external fun nativeBuildCommentBody(parentId: String, text: String): String

    // Data parsing
    @JvmStatic external fun nativeParsePosts(rawJson: String, afterHolder: AfterHolder): String
    @JvmStatic external fun nativeParseComments(rawJson: String): String
    @JvmStatic external fun nativeParseSubreddits(rawJson: String): String
    @JvmStatic external fun nativeParseSubredditInfo(rawJson: String): String
}
