package chat.progressive.app.lemmy

import chat.progressive.app.protocol.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.withContext
import org.json.JSONArray
import org.json.JSONObject
import timber.log.Timber

class LemmySession(
    private val instanceUrl: String
) : IProtocolSession {

    override val protocolType: ProtocolType = ProtocolType.LEMMY
    override val userId: String get() = _username
    override val isLoggedIn = MutableStateFlow(false)
    override val connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)

    private val client = LemmyClient()
    private var jwt: String = ""
    private var _username: String = ""
    private val listeners = mutableListOf<ProtocolSessionListener>()
    private var currentUserId: Int = 0

    init {
        client.init(instanceUrl)
    }

    // ---- Auth ----

    suspend fun login(username: String, password: String): Boolean = withContext(Dispatchers.IO) {
        try {
            val resp = client.login(username, password)
            jwt = LemmyClient.extractJwt(resp) ?: ""
            if (jwt.isEmpty()) {
                val err = LemmyClient.extractError(resp)
                notifyError(-1, err ?: "Login failed")
                return@withContext false
            }
            _username = username

            // Fetch own user info
            val siteResp = client.getSite(jwt)
            val myUser = siteResp.optJSONObject("my_user")
            if (myUser != null) {
                val localUser = myUser.optJSONObject("local_user_view")
                val person = localUser?.optJSONObject("person")
                currentUserId = person?.optInt("id", 0) ?: 0
                _username = person?.optString("name", username) ?: username
            }

            isLoggedIn.value = true
            connectionState.value = ConnectionState.CONNECTED
            notifyState(ConnectionState.CONNECTED)
            true
        } catch (e: Exception) {
            Timber.e(e, "Lemmy login failed")
            notifyError(-1, e.message ?: "Login error")
            false
        }
    }

    suspend fun register(username: String, password: String, email: String): Boolean = withContext(Dispatchers.IO) {
        try {
            val resp = client.register(username, password, email)
            jwt = LemmyClient.extractJwt(resp) ?: ""
            if (jwt.isEmpty()) {
                val err = LemmyClient.extractError(resp)
                notifyError(-2, err ?: "Registration failed")
                return@withContext false
            }
            _username = username
            isLoggedIn.value = true
            connectionState.value = ConnectionState.CONNECTED
            true
        } catch (e: Exception) {
            Timber.e(e, "Lemmy registration failed")
            notifyError(-2, e.message ?: "Registration error")
            false
        }
    }

    // ---- IProtocolSession ----

    override fun open() {
        connectionState.value = ConnectionState.CONNECTED
    }

    override fun close() {
        jwt = ""
        connectionState.value = ConnectionState.DISCONNECTED
        isLoggedIn.value = false
    }

    override suspend fun sendMessage(chatId: Long, text: String) {
        sendPost(chatId, text)
    }

    override fun getChats(): List<IProtocolRoom> {
        return emptyList() // populated async
    }

    override fun getChat(chatId: Long): IProtocolRoom? {
        return null
    }

    override fun addListener(listener: ProtocolSessionListener) {
        listeners.add(listener)
    }

    override fun removeListener(listener: ProtocolSessionListener) {
        listeners.remove(listener)
    }

    // ---- Communities (chat rooms) ----

    suspend fun getSubscribedCommunities(page: Int = 1): List<LemmyRoom> = withContext(Dispatchers.IO) {
        try {
            val resp = client.listCommunities(jwt, "Subscribed", "Active", page, 50)
            val communities = resp.optJSONArray("communities") ?: return@withContext emptyList()
            val rooms = mutableListOf<LemmyRoom>()
            for (i in 0 until communities.length()) {
                val c = communities.getJSONObject(i)
                val cv = c.optJSONObject("community")
                val counts = c.optJSONObject("counts")
                if (cv != null) {
                    rooms.add(LemmyRoom(
                        id = cv.optLong("id", 0),
                        title = cv.optString("title", cv.optString("name", "?")),
                        unreadCount = counts?.optInt("posts", 0) ?: 0,
                        avatarUrl = cv.optString("icon", "").takeIf { it.isNotEmpty() },
                        session = this@LemmySession
                    ))
                }
            }
            rooms
        } catch (e: Exception) {
            Timber.e(e, "Failed to fetch communities")
            emptyList()
        }
    }

    suspend fun getCommunityPosts(communityId: Long, limit: Int = 30): List<LemmyMessage> = withContext(Dispatchers.IO) {
        try {
            val resp = client.listPosts(jwt, "All", "Active", 1, limit, communityId = communityId.toInt())
            val posts = resp.optJSONArray("posts") ?: return@withContext emptyList()
            val msgs = mutableListOf<LemmyMessage>()
            for (i in 0 until posts.length()) {
                val pv = posts.getJSONObject(i)
                val post = pv.optJSONObject("post")
                val creator = pv.optJSONObject("creator")
                if (post != null) {
                    msgs.add(LemmyMessage(
                        id = post.optLong("id", 0),
                        chatId = communityId,
                        senderId = creator?.optString("name", "") ?: "",
                        senderName = creator?.optString("display_name",
                            creator?.optString("name", "?")) ?: "?",
                        text = post.optString("body", post.optString("name", "")),
                        timestamp = post.optString("published", "0")
                            .let { parseIsoDate(it) },
                        isOutgoing = creator?.optInt("id", -1) == currentUserId
                    ))
                }
            }
            msgs
        } catch (e: Exception) {
            Timber.e(e, "Failed to fetch posts")
            emptyList()
        }
    }

    suspend fun sendPost(communityId: Long, text: String): Boolean = withContext(Dispatchers.IO) {
        try {
            val resp = client.createPost(jwt, communityId.toInt(), text, text)
            resp.optJSONObject("post_view") != null
        } catch (e: Exception) {
            Timber.e(e, "Failed to create post")
            false
        }
    }

    suspend fun unfollowCommunity(communityId: Long) {
        withContext(Dispatchers.IO) {
            client.followCommunity(jwt, communityId.toInt(), false)
        }
    }

    suspend fun getUnreadCount(): Int = withContext(Dispatchers.IO) {
        try {
            val resp = client.getUnreadCount(jwt)
            resp.optInt("replies", 0) + resp.optInt("mentions", 0)
        } catch (e: Exception) { 0 }
    }

    // ---- Listeners ----

    private fun notifyState(state: ConnectionState) {
        listeners.forEach { it.onConnectionStateChanged(state) }
    }

    private fun notifyError(code: Int, message: String) {
        listeners.forEach { it.onError(ProtocolError(code, message)) }
    }

    companion object {
        private fun parseIsoDate(iso: String): Long {
            return try {
                java.time.Instant.parse(iso).toEpochMilli()
            } catch (e: Exception) { 0L }
        }
    }
}
