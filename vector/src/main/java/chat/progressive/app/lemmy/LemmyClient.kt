package chat.progressive.app.lemmy

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import org.json.JSONObject
import timber.log.Timber

/**
 * Kotlin wrapper around the native Lemmy API client.
 * All functions are suspend — call from a coroutine scope.
 * Auth token must be obtained via [login] and passed explicitly.
 */
class LemmyClient {

    // ---- Initialization ----

    fun init(instanceUrl: String) {
        if (!LemmyNative.ensureLoaded()) {
            throw IllegalStateException("progressive_native library not loaded")
        }
        LemmyNative.nativeLemmySetInstance(instanceUrl)
        Timber.d("LemmyClient initialized for $instanceUrl")
    }

    val instanceUrl: String get() = LemmyNative.nativeLemmyGetInstance()

    // ---- Site ----

    suspend fun getSite(auth: String = ""): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetSite(auth))
    }

    // ---- Auth ----

    suspend fun login(username: String, password: String, totpToken: String = ""): JSONObject = withIo {
        j(LemmyNative.nativeLemmyLogin(username, password, totpToken))
    }

    suspend fun register(
        username: String, password: String, email: String = "",
        captchaUuid: String = "", captchaAnswer: String = "", answer: String = "",
        showNsfw: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyRegister(
            username, password, email,
            captchaUuid, captchaAnswer, answer, showNsfw
        ))
    }

    suspend fun getCaptcha(auth: String = ""): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetCaptcha(auth))
    }

    // ---- Community ----

    suspend fun listCommunities(
        auth: String = "",
        type: String = "Local", sort: String = "Active",
        page: Int = 1, limit: Int = 20, showNsfw: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyListCommunities(auth, type, sort, page, limit, showNsfw))
    }

    suspend fun getCommunity(auth: String = "", id: Int = 0, name: String = ""): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetCommunity(auth, id, name))
    }

    suspend fun followCommunity(auth: String, communityId: Int, follow: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmyFollowCommunity(auth, communityId, follow))
    }

    suspend fun blockCommunity(auth: String, communityId: Int, block: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmyBlockCommunity(auth, communityId, block))
    }

    // ---- Post ----

    suspend fun listPosts(
        auth: String = "", type: String = "Local", sort: String = "Active",
        page: Int = 1, limit: Int = 20, communityId: Int = 0, communityName: String = "",
        savedOnly: Boolean = false, likedOnly: Boolean = false,
        dislikedOnly: Boolean = false, showNsfw: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyListPosts(
            auth, type, sort, page, limit, communityId, communityName,
            savedOnly, likedOnly, dislikedOnly, showNsfw
        ))
    }

    suspend fun getPost(auth: String = "", postId: Int): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetPost(auth, postId))
    }

    suspend fun createPost(
        auth: String, communityId: Int, name: String,
        body: String = "", url: String = "",
        nsfw: Boolean = false, languageId: Int = 0
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyCreatePost(auth, communityId, name, body, url, nsfw, languageId))
    }

    suspend fun editPost(
        auth: String, postId: Int, name: String = "",
        body: String = "", url: String = "",
        nsfw: Boolean = false, languageId: Int = 0
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyEditPost(auth, postId, name, body, url, nsfw, languageId))
    }

    suspend fun deletePost(auth: String, postId: Int, deleted: Boolean = true): JSONObject = withIo {
        j(LemmyNative.nativeLemmyDeletePost(auth, postId, deleted))
    }

    suspend fun removePost(
        auth: String, postId: Int, removed: Boolean = true, reason: String = ""
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyRemovePost(auth, postId, removed, reason))
    }

    suspend fun likePost(auth: String, postId: Int, score: Int): JSONObject = withIo {
        j(LemmyNative.nativeLemmyLikePost(auth, postId, score))
    }

    suspend fun savePost(auth: String, postId: Int, save: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmySavePost(auth, postId, save))
    }

    suspend fun reportPost(auth: String, postId: Int, reason: String): JSONObject = withIo {
        j(LemmyNative.nativeLemmyReportPost(auth, postId, reason))
    }

    suspend fun lockPost(auth: String, postId: Int, locked: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmyLockPost(auth, postId, locked))
    }

    suspend fun featurePost(
        auth: String, postId: Int, featured: Boolean, featureType: String = "Local"
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyFeaturePost(auth, postId, featured, featureType))
    }

    suspend fun markPostRead(auth: String, postId: Int, read: Boolean = true): JSONObject = withIo {
        j(LemmyNative.nativeLemmyMarkPostRead(auth, postId, read))
    }

    suspend fun getSiteMetadata(auth: String, url: String): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetSiteMetadata(auth, url))
    }

    // ---- Comment ----

    suspend fun listComments(
        auth: String = "", postId: Int, sort: String = "Hot",
        maxDepth: Int = 8, limit: Int = 50, page: Int = 1,
        savedOnly: Boolean = false, likedOnly: Boolean = false, dislikedOnly: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyListComments(
            auth, postId, sort, maxDepth, limit, page, savedOnly, likedOnly, dislikedOnly
        ))
    }

    suspend fun getComment(auth: String = "", commentId: Int): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetComment(auth, commentId))
    }

    suspend fun createComment(
        auth: String, postId: Int, content: String,
        parentId: Int = 0, languageId: Int = 0
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyCreateComment(auth, postId, content, parentId, languageId))
    }

    suspend fun editComment(
        auth: String, commentId: Int, content: String, languageId: Int = 0
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyEditComment(auth, commentId, content, languageId))
    }

    suspend fun deleteComment(
        auth: String, commentId: Int, deleted: Boolean = true
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyDeleteComment(auth, commentId, deleted))
    }

    suspend fun removeComment(
        auth: String, commentId: Int, removed: Boolean = true, reason: String = ""
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyRemoveComment(auth, commentId, removed, reason))
    }

    suspend fun likeComment(auth: String, commentId: Int, score: Int): JSONObject = withIo {
        j(LemmyNative.nativeLemmyLikeComment(auth, commentId, score))
    }

    suspend fun saveComment(auth: String, commentId: Int, save: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmySaveComment(auth, commentId, save))
    }

    suspend fun reportComment(auth: String, commentId: Int, reason: String): JSONObject = withIo {
        j(LemmyNative.nativeLemmyReportComment(auth, commentId, reason))
    }

    suspend fun distinguishComment(
        auth: String, commentId: Int, distinguished: Boolean
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyDistinguishComment(auth, commentId, distinguished))
    }

    suspend fun markCommentRead(
        auth: String, commentId: Int, read: Boolean = true
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyMarkCommentRead(auth, commentId, read))
    }

    // ---- User ----

    suspend fun getUser(
        auth: String = "", username: String = "", personId: Int = 0,
        sort: String = "New", page: Int = 1, limit: Int = 20,
        communityId: Int = 0, savedOnly: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetUser(
            auth, username, personId, sort, page, limit, communityId, savedOnly
        ))
    }

    suspend fun blockUser(auth: String, personId: Int, block: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmyBlockUser(auth, personId, block))
    }

    suspend fun getMentions(
        auth: String, sort: String = "New", page: Int = 1,
        limit: Int = 20, unreadOnly: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetMentions(auth, sort, page, limit, unreadOnly))
    }

    suspend fun markMentionRead(auth: String, mentionId: Int, read: Boolean = true): JSONObject = withIo {
        j(LemmyNative.nativeLemmyMarkMentionRead(auth, mentionId, read))
    }

    suspend fun getReplies(
        auth: String, sort: String = "New", page: Int = 1,
        limit: Int = 20, unreadOnly: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetReplies(auth, sort, page, limit, unreadOnly))
    }

    suspend fun markAllAsRead(auth: String): JSONObject = withIo {
        j(LemmyNative.nativeLemmyMarkAllAsRead(auth))
    }

    suspend fun getUnreadCount(auth: String): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetUnreadCount(auth))
    }

    suspend fun saveUserSettings(auth: String, settings: JSONObject): JSONObject = withIo {
        j(LemmyNative.nativeLemmySaveUserSettings(auth, settings.toString()))
    }

    suspend fun deleteAccount(
        auth: String, password: String = "", deleteContent: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyDeleteAccount(auth, password, deleteContent))
    }

    suspend fun changePassword(
        auth: String, newPassword: String, newPasswordVerify: String, oldPassword: String
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyChangePassword(auth, newPassword, newPasswordVerify, oldPassword))
    }

    // ---- Private Messages ----

    suspend fun listPrivateMessages(
        auth: String, page: Int = 1, limit: Int = 20, unreadOnly: Boolean = false
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyListPrivateMessages(auth, page, limit, unreadOnly))
    }

    suspend fun sendPrivateMessage(
        auth: String, recipientId: Int, content: String
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmySendPrivateMessage(auth, recipientId, content))
    }

    suspend fun editPrivateMessage(
        auth: String, messageId: Int, content: String
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyEditPrivateMessage(auth, messageId, content))
    }

    suspend fun deletePrivateMessage(
        auth: String, messageId: Int, deleted: Boolean = true
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyDeletePrivateMessage(auth, messageId, deleted))
    }

    suspend fun markPrivateMessageRead(
        auth: String, messageId: Int, read: Boolean = true
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyMarkPrivateMessageRead(auth, messageId, read))
    }

    // ---- Search ----

    suspend fun search(
        auth: String = "", query: String, type: String = "All",
        sort: String = "TopAll", listingType: String = "All",
        page: Int = 1, limit: Int = 20,
        communityId: Int = 0, communityName: String = "", creatorId: Int = 0
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmySearch(
            auth, query, type, sort, listingType, page, limit,
            communityId, communityName, creatorId
        ))
    }

    suspend fun resolveObject(auth: String = "", query: String): JSONObject = withIo {
        j(LemmyNative.nativeLemmyResolveObject(auth, query))
    }

    // ---- Moderation ----

    suspend fun getModlog(
        auth: String, modPersonId: Int = 0, communityId: Int = 0,
        page: Int = 1, limit: Int = 20, type: String = "All", otherPersonId: Int = 0
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyGetModlog(
            auth, modPersonId, communityId, page, limit, type, otherPersonId
        ))
    }

    // ---- Admin ----

    suspend fun addAdmin(auth: String, personId: Int, added: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmyAddAdmin(auth, personId, added))
    }

    suspend fun listRegistrationApplications(
        auth: String, unreadOnly: Boolean = false, page: Int = 1, limit: Int = 20
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyListRegistrationApplications(auth, unreadOnly, page, limit))
    }

    suspend fun approveRegistration(
        auth: String, id: Int, approve: Boolean, denyReason: String = ""
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyApproveRegistration(auth, id, approve, denyReason))
    }

    suspend fun blockInstance(auth: String, instanceId: Int, block: Boolean): JSONObject = withIo {
        j(LemmyNative.nativeLemmyBlockInstance(auth, instanceId, block))
    }

    // ---- Image Upload ----

    suspend fun uploadImage(
        auth: String, imageData: ByteArray, filename: String, mimeType: String = ""
    ): JSONObject = withIo {
        j(LemmyNative.nativeLemmyUploadImage(auth, imageData, filename, mimeType))
    }

    // ---- Helpers ----

    private suspend fun withIo(block: () -> JSONObject): JSONObject =
        withContext(Dispatchers.IO) { block() }

    private fun j(raw: String): JSONObject {
        if (raw.isEmpty() || raw == "null") return JSONObject()
        return JSONObject(raw)
    }

    companion object {
        /** Extract JWT token from a login or register response. */
        fun extractJwt(response: JSONObject): String? =
            response.optString("jwt", null)

        /** Extract error message from a Lemmy API error response. */
        fun extractError(response: JSONObject): String? =
            response.optString("error", null)
    }
}
