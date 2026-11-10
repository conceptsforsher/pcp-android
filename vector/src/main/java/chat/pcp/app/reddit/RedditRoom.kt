package chat.pcp.app.reddit

import chat.pcp.app.protocol.*

class RedditRoom(
    override val id: String,
    val subreddit: String,
    private var postTitle: String,
    private val session: RedditSession
) : IProtocolRoom {

    override val title: String get() = "r/$subreddit - $postTitle"
    override val topic: String get() = ""
    override val lastMessage: String? get() = _messages.lastOrNull()?.text
    override val lastMessageTime: Long get() = _messages.lastOrNull()?.timestamp ?: 0L
    override val unreadCount: Int = 0
    override val isGroup: Boolean = false
    override val protocolType = ProtocolType.REDDIT

    private val _messages = mutableListOf<RedditMessage>()
    private var post: RedditPost? = null

    fun updatePost(p: RedditPost) {
        post = p
        postTitle = p.title

        if (_messages.isEmpty()) {
            _messages.add(RedditMessage(
                id = "post_${p.id}",
                chatId = id,
                senderId = p.author,
                senderName = p.author,
                text = "${p.title}\n\n${p.selftext}",
                url = p.url,
                score = p.score,
                commentCount = p.commentCount
            ))
        }
    }

    fun addComments(json: String) {
        val comments = RedditSession.parsePostsFromJson(json)
        // Actually this is comments - parse as RedditComment
        try {
            val arr = org.json.JSONArray(json)
            for (i in 0 until arr.length()) {
                val obj = arr.getJSONObject(i)
                _messages.add(RedditMessage(
                    id = obj.optString("id", "comment_$i"),
                    chatId = id,
                    senderId = obj.optString("author"),
                    senderName = obj.optString("author"),
                    text = obj.optString("body"),
                    timestamp = obj.optLong("createdUtc") * 1000,
                    score = obj.optInt("score"),
                    depth = obj.optInt("depth")
                ))
            }
        } catch (e: Exception) { /* ignore */ }
    }

    override fun getMessages(limit: Int, fromMessageId: String?): List<IProtocolMessage> {
        return _messages.toList()
    }

    override fun sendTextMessage(text: String) {
        session.sendMessage("t3_$id", text)
    }

    override fun markAsRead() {}

    override fun leave() {
        session.getChats()
    }
}
