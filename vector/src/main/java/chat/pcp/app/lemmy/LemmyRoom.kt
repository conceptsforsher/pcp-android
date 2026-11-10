package chat.pcp.app.lemmy

import chat.pcp.app.protocol.*

data class LemmyRoom(
    override val id: Long,
    override val title: String,
    override val lastMessage: String? = null,
    override val lastMessageTime: Long? = null,
    override val unreadCount: Int = 0,
    override val isGroup: Boolean = true,
    override val avatarUrl: String? = null,
    override val protocolType: ProtocolType = ProtocolType.LEMMY,
    val session: LemmySession? = null
) : IProtocolRoom {

    override suspend fun getMessages(limit: Int, fromMessageId: Long): List<IProtocolMessage> {
        return session?.getCommunityPosts(id, limit) ?: emptyList()
    }

    override suspend fun sendTextMessage(text: String) {
        session?.sendPost(id, text)
    }

    override suspend fun markAsRead() {
        // Lemmy communities don't have per-community mark-as-read in the API
    }

    override suspend fun leave() {
        session?.unfollowCommunity(id)
    }
}
