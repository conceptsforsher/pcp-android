package chat.pcp.app.reddit

import chat.pcp.app.protocol.*
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONArray
import org.json.JSONObject
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL

class RedditSession(
    val account: RedditAccount = RedditAccount("anonymous")
) : IProtocolSession {

    override val protocolType = ProtocolType.REDDIT
    override val userId: String get() = if (account.isAnonymous) "anonymous" else account.username

    private val _isLoggedIn = MutableStateFlow(!account.isAnonymous)
    override val isLoggedIn: StateFlow<Boolean> = _isLoggedIn

    private val _connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)
    override val connectionState: StateFlow<ConnectionState> = _connectionState

    private val listeners = mutableListOf<ProtocolSessionListener>()
    private val rooms = mutableMapOf<String, RedditRoom>()
    private var mAfter = ""

    override fun open() {
        _connectionState.value = ConnectionState.CONNECTED
        listeners.forEach { it.onConnectionStateChanged(ConnectionState.CONNECTED) }
    }

    override fun close() {
        _connectionState.value = ConnectionState.DISCONNECTED
        rooms.clear()
    }

    override fun sendMessage(chatId: String, text: String) {
        if (account.isAnonymous) return
        val urlStr = RedditNative.nativeCommentUrl()
        val body = RedditNative.nativeBuildCommentBody(chatId, text)
        val result = httpPost(urlStr, body)
        if (result != null) {
            refreshRoom(chatId)
        }
    }

    override fun getChats(): List<IProtocolRoom> = rooms.values.toList()

    override fun getChat(chatId: String): IProtocolRoom? = rooms[chatId]

    override fun addListener(listener: ProtocolSessionListener) {
        listeners.add(listener)
    }

    override fun removeListener(listener: ProtocolSessionListener) {
        listeners.remove(listener)
    }

    // ==== Reddit-specific API ====

    fun fetchFrontpage(sort: String = "hot") {
        val url = RedditNative.nativeFrontpageUrl(sort, mAfter, 25)
        fetchPosts(url)
    }

    fun fetchSubreddit(sub: String, sort: String = "hot") {
        val url = RedditNative.nativeSubredditUrl(sub, sort, mAfter)
        fetchPosts(url)
    }

    fun fetchComments(sub: String, postId: String) {
        val url = RedditNative.nativeCommentsUrl(sub, postId)
        val json = httpGet(url) ?: return
        val parsed = RedditNative.nativeParseComments(json)
        // Store comments in the room
        val room = rooms.getOrPut(postId) {
            RedditRoom(postId, sub, "Post", this)
        }
        room.addComments(parsed)
        listeners.forEach { it.onChatListChanged() }
    }

    fun searchSubreddits(query: String): List<RedditSubreddit> {
        val url = RedditNative.nativeSearchSubredditsUrl(query)
        val json = httpGet(url) ?: return emptyList()
        val parsed = RedditNative.nativeParseSubreddits(json)
        return parseSubredditsFromJson(parsed)
    }

    fun vote(fullname: String, dir: Int) {
        if (account.isAnonymous) return
        val url = RedditNative.nativeVoteUrl()
        val body = RedditNative.nativeBuildVoteBody(fullname, dir)
        httpPost(url, body)
    }

    fun save(fullname: String) {
        if (account.isAnonymous) return
        val url = RedditNative.nativeSaveUrl(true)
        val body = RedditNative.nativeBuildSaveBody(fullname)
        httpPost(url, body)
    }

    fun hide(fullname: String) {
        if (account.isAnonymous) return
        val url = RedditNative.nativeHideUrl()
        val body = RedditNative.nativeBuildHideBody(fullname)
        httpPost(url, body)
    }

    // ==== Private helpers ====

    private fun fetchPosts(urlStr: String) {
        val json = httpGet(urlStr) ?: return
        val after = AfterHolder()
        val parsed = RedditNative.nativeParsePosts(json, after)
        mAfter = after.value

        val posts = parsePostsFromJson(parsed)
        for (post in posts) {
            val room = rooms.getOrPut(post.id) {
                RedditRoom(post.id, post.subreddit, post.title, this)
            }
            room.updatePost(post)
        }
        listeners.forEach { it.onChatListChanged() }
    }

    private fun refreshRoom(chatId: String) {
        // Placeholder
    }

    private fun httpGet(urlStr: String): String? {
        try {
            val url = URL(urlStr)
            val conn = url.openConnection() as HttpURLConnection
            conn.setRequestProperty("User-Agent", "PCP-Reddit/0.1")
            conn.connectTimeout = 15000
            conn.readTimeout = 15000

            val code = conn.responseCode
            if (code != 200) {
                listeners.forEach { it.onError(ProtocolError(code, conn.responseMessage ?: "HTTP error")) }
                return null
            }

            val reader = BufferedReader(InputStreamReader(conn.inputStream))
            val sb = StringBuilder()
            var line: String?
            while (reader.readLine().also { line = it } != null) {
                sb.append(line)
            }
            reader.close()
            conn.disconnect()
            return sb.toString()
        } catch (e: Exception) {
            listeners.forEach { it.onError(ProtocolError(-1, e.message ?: "Network error")) }
            return null
        }
    }

    private fun httpPost(urlStr: String, body: String): String? {
        if (account.isAnonymous) return null
        try {
            val url = URL(urlStr)
            val conn = url.openConnection() as HttpURLConnection
            conn.requestMethod = "POST"
            conn.setRequestProperty("User-Agent", "PCP-Reddit/0.1")
            conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded")
            if (account.accessToken.isNotEmpty()) {
                conn.setRequestProperty("Authorization", "Bearer ${account.accessToken}")
            }
            conn.doOutput = true
            conn.connectTimeout = 15000
            conn.readTimeout = 15000

            conn.outputStream.write(body.toByteArray())
            conn.outputStream.flush()
            conn.outputStream.close()

            val code = conn.responseCode
            val reader = BufferedReader(InputStreamReader(
                if (code in 200..299) conn.inputStream else conn.errorStream
            ))
            val sb = StringBuilder()
            var line: String?
            while (reader.readLine().also { line = it } != null) {
                sb.append(line)
            }
            reader.close()
            conn.disconnect()
            return sb.toString()
        } catch (e: Exception) {
            listeners.forEach { it.onError(ProtocolError(-1, e.message ?: "Network error")) }
            return null
        }
    }

    companion object {
        fun parsePostsFromJson(json: String): List<RedditPost> {
            val result = mutableListOf<RedditPost>()
            try {
                val arr = JSONArray(json)
                for (i in 0 until arr.length()) {
                    val obj = arr.getJSONObject(i)
                    result.add(RedditPost(
                        id = obj.optString("id"),
                        fullname = obj.optString("fullname"),
                        title = obj.optString("title"),
                        author = obj.optString("author"),
                        subreddit = obj.optString("subreddit"),
                        url = obj.optString("url"),
                        thumbnail = obj.optString("thumbnail"),
                        domain = obj.optString("domain"),
                        permalink = obj.optString("permalink"),
                        selftext = obj.optString("selftext"),
                        score = obj.optInt("score"),
                        commentCount = obj.optInt("commentCount"),
                        nsfw = obj.optBoolean("nsfw"),
                        spoiler = obj.optBoolean("spoiler"),
                        stickied = obj.optBoolean("stickied"),
                        isSelf = obj.optBoolean("isSelf"),
                        isGallery = obj.optBoolean("isGallery"),
                        isVideo = obj.optBoolean("isVideo"),
                        createdUtc = obj.optLong("createdUtc")
                    ))
                }
            } catch (e: Exception) { /* ignore parse errors */ }
            return result
        }

        fun parseSubredditsFromJson(json: String): List<RedditSubreddit> {
            val result = mutableListOf<RedditSubreddit>()
            try {
                val arr = JSONArray(json)
                for (i in 0 until arr.length()) {
                    val obj = arr.getJSONObject(i)
                    result.add(RedditSubreddit(
                        name = obj.optString("name"),
                        title = obj.optString("title"),
                        description = obj.optString("description"),
                        subscriberCount = obj.optInt("subscriberCount"),
                        activeUserCount = obj.optInt("activeUserCount"),
                        over18 = obj.optBoolean("over18")
                    ))
                }
            } catch (e: Exception) { /* ignore */ }
            return result
        }
    }
}
