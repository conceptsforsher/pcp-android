package chat.pcp.app.protocol

import chat.pcp.app.reddit.RedditSessionHolder

class ProtocolManager(
    private val redditHolder: RedditSessionHolder? = null
) {
    private val sessions = mutableListOf<IProtocolSession>()
    private var matrixSession: IProtocolSession? = null

    fun registerMatrixSession(session: IProtocolSession) {
        matrixSession = session
        sessions.add(0, session)
    }

    fun getRedditSession(): IProtocolSession? {
        return redditHolder?.getOrCreateAnonymous()
    }

    fun getAllSessions(): List<IProtocolSession> {
        val result = mutableListOf<IProtocolSession>()
        matrixSession?.let { result.add(it) }
        redditHolder?.getAllSessions()?.let { result.addAll(it) }
        return result
    }

    fun getSessionsByType(type: ProtocolType): List<IProtocolSession> {
        return getAllSessions().filter { it.protocolType == type }
    }

    fun getUnifiedChats(): List<IProtocolRoom> {
        return getAllSessions().flatMap { it.getChats() }
    }

    fun getSessionForChat(protocolType: ProtocolType): IProtocolSession? {
        return getAllSessions().find { it.protocolType == protocolType }
    }

    fun closeAll() {
        sessions.forEach { it.close() }
        redditHolder?.closeAll()
        sessions.clear()
        matrixSession = null
    }
}
