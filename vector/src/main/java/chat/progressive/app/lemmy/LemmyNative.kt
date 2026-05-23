package chat.progressive.app.lemmy

import timber.log.Timber

object LemmyNative {

    private var isLoaded = false

    fun ensureLoaded(): Boolean {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
                isLoaded = true
                Timber.d("progressive_native loaded — Lemmy bridge available")
            } catch (e: UnsatisfiedLinkError) {
                Timber.w(e, "Failed to load progressive_native — Lemmy unavailable")
            }
        }
        return isLoaded
    }

    val isAvailable: Boolean get() = isLoaded

    // ---- Instance ----

    @JvmStatic external fun nativeLemmySetInstance(url: String)

    @JvmStatic external fun nativeLemmyGetInstance(): String

    // ---- Site ----

    @JvmStatic external fun nativeLemmyGetSite(auth: String): String

    // ---- Auth ----

    @JvmStatic external fun nativeLemmyLogin(username: String, password: String, totp: String): String

    @JvmStatic external fun nativeLemmyRegister(
        username: String, password: String, email: String,
        captchaUuid: String, captchaAnswer: String, answer: String,
        showNsfw: Boolean
    ): String

    @JvmStatic external fun nativeLemmyGetCaptcha(auth: String): String

    // ---- Community ----

    @JvmStatic external fun nativeLemmyListCommunities(
        auth: String, type: String, sort: String,
        page: Int, limit: Int, showNsfw: Boolean
    ): String

    @JvmStatic external fun nativeLemmyGetCommunity(auth: String, id: Int, name: String): String

    @JvmStatic external fun nativeLemmyFollowCommunity(auth: String, communityId: Int, follow: Boolean): String

    @JvmStatic external fun nativeLemmyBlockCommunity(auth: String, communityId: Int, block: Boolean): String

    @JvmStatic external fun nativeLemmyHideCommunity(
        auth: String, communityId: Int, hide: Boolean, reason: String
    ): String

    // ---- Post ----

    @JvmStatic external fun nativeLemmyListPosts(
        auth: String, type: String, sort: String, page: Int, limit: Int,
        communityId: Int, communityName: String,
        savedOnly: Boolean, likedOnly: Boolean, dislikedOnly: Boolean, showNsfw: Boolean
    ): String

    @JvmStatic external fun nativeLemmyGetPost(auth: String, postId: Int): String

    @JvmStatic external fun nativeLemmyCreatePost(
        auth: String, communityId: Int, name: String,
        body: String, url: String, nsfw: Boolean, languageId: Int
    ): String

    @JvmStatic external fun nativeLemmyEditPost(
        auth: String, postId: Int, name: String,
        body: String, url: String, nsfw: Boolean, languageId: Int
    ): String

    @JvmStatic external fun nativeLemmyDeletePost(auth: String, postId: Int, deleted: Boolean): String

    @JvmStatic external fun nativeLemmyRemovePost(
        auth: String, postId: Int, removed: Boolean, reason: String
    ): String

    @JvmStatic external fun nativeLemmyLikePost(auth: String, postId: Int, score: Int): String

    @JvmStatic external fun nativeLemmySavePost(auth: String, postId: Int, save: Boolean): String

    @JvmStatic external fun nativeLemmyReportPost(auth: String, postId: Int, reason: String): String

    @JvmStatic external fun nativeLemmyLockPost(auth: String, postId: Int, locked: Boolean): String

    @JvmStatic external fun nativeLemmyFeaturePost(
        auth: String, postId: Int, featured: Boolean, featureType: String
    ): String

    @JvmStatic external fun nativeLemmyMarkPostRead(auth: String, postId: Int, read: Boolean): String

    @JvmStatic external fun nativeLemmyGetSiteMetadata(auth: String, url: String): String

    // ---- Comment ----

    @JvmStatic external fun nativeLemmyListComments(
        auth: String, postId: Int, sort: String, maxDepth: Int,
        limit: Int, page: Int, savedOnly: Boolean, likedOnly: Boolean, dislikedOnly: Boolean
    ): String

    @JvmStatic external fun nativeLemmyGetComment(auth: String, commentId: Int): String

    @JvmStatic external fun nativeLemmyCreateComment(
        auth: String, postId: Int, content: String, parentId: Int, languageId: Int
    ): String

    @JvmStatic external fun nativeLemmyEditComment(
        auth: String, commentId: Int, content: String, languageId: Int
    ): String

    @JvmStatic external fun nativeLemmyDeleteComment(auth: String, commentId: Int, deleted: Boolean): String

    @JvmStatic external fun nativeLemmyRemoveComment(
        auth: String, commentId: Int, removed: Boolean, reason: String
    ): String

    @JvmStatic external fun nativeLemmyLikeComment(auth: String, commentId: Int, score: Int): String

    @JvmStatic external fun nativeLemmySaveComment(auth: String, commentId: Int, save: Boolean): String

    @JvmStatic external fun nativeLemmyReportComment(auth: String, commentId: Int, reason: String): String

    @JvmStatic external fun nativeLemmyDistinguishComment(
        auth: String, commentId: Int, distinguished: Boolean
    ): String

    @JvmStatic external fun nativeLemmyMarkCommentRead(auth: String, commentId: Int, read: Boolean): String

    // ---- User ----

    @JvmStatic external fun nativeLemmyGetUser(
        auth: String, username: String, personId: Int, sort: String,
        page: Int, limit: Int, communityId: Int, savedOnly: Boolean
    ): String

    @JvmStatic external fun nativeLemmyBlockUser(auth: String, personId: Int, block: Boolean): String

    @JvmStatic external fun nativeLemmyGetMentions(
        auth: String, sort: String, page: Int, limit: Int, unreadOnly: Boolean
    ): String

    @JvmStatic external fun nativeLemmyMarkMentionRead(auth: String, mentionId: Int, read: Boolean): String

    @JvmStatic external fun nativeLemmyGetReplies(
        auth: String, sort: String, page: Int, limit: Int, unreadOnly: Boolean
    ): String

    @JvmStatic external fun nativeLemmyMarkAllAsRead(auth: String): String

    @JvmStatic external fun nativeLemmyGetUnreadCount(auth: String): String

    @JvmStatic external fun nativeLemmySaveUserSettings(auth: String, settingsJson: String): String

    @JvmStatic external fun nativeLemmyDeleteAccount(
        auth: String, password: String, deleteContent: Boolean
    ): String

    @JvmStatic external fun nativeLemmyChangePassword(
        auth: String, newPass: String, newPassVerify: String, oldPass: String
    ): String

    // ---- Private Messages ----

    @JvmStatic external fun nativeLemmyListPrivateMessages(
        auth: String, page: Int, limit: Int, unreadOnly: Boolean
    ): String

    @JvmStatic external fun nativeLemmySendPrivateMessage(
        auth: String, recipientId: Int, content: String
    ): String

    @JvmStatic external fun nativeLemmyEditPrivateMessage(
        auth: String, messageId: Int, content: String
    ): String

    @JvmStatic external fun nativeLemmyDeletePrivateMessage(
        auth: String, messageId: Int, deleted: Boolean
    ): String

    @JvmStatic external fun nativeLemmyMarkPrivateMessageRead(
        auth: String, messageId: Int, read: Boolean
    ): String

    // ---- Search ----

    @JvmStatic external fun nativeLemmySearch(
        auth: String, query: String, type: String, sort: String,
        listingType: String, page: Int, limit: Int,
        communityId: Int, communityName: String, creatorId: Int
    ): String

    @JvmStatic external fun nativeLemmyResolveObject(auth: String, query: String): String

    // ---- Moderation ----

    @JvmStatic external fun nativeLemmyGetModlog(
        auth: String, modPersonId: Int, communityId: Int,
        page: Int, limit: Int, type: String, otherPersonId: Int
    ): String

    // ---- Admin ----

    @JvmStatic external fun nativeLemmyAddAdmin(auth: String, personId: Int, added: Boolean): String

    @JvmStatic external fun nativeLemmyListRegistrationApplications(
        auth: String, unreadOnly: Boolean, page: Int, limit: Int
    ): String

    @JvmStatic external fun nativeLemmyApproveRegistration(
        auth: String, id: Int, approve: Boolean, denyReason: String
    ): String

    @JvmStatic external fun nativeLemmyBlockInstance(
        auth: String, instanceId: Int, block: Boolean
    ): String

    // ---- Utility ----

    @JvmStatic external fun nativeLemmyBuildJson(keys: String, values: String): String

    // ---- Image Upload ----

    @JvmStatic external fun nativeLemmyUploadImage(
        auth: String, imageData: ByteArray, filename: String, mimeType: String
    ): String
}
