package chat.pcp.app.protocol

import chat.pcp.app.lemmy.LemmySessionHolder

class ProtocolManager(
    private val lemmyHolder: LemmySessionHolder
) {

    private val sessions = mutableListOf<IProtocolSession>()
    private var matrixSession: IProtocolSession? = null

    fun registerMatrixSession(session: IProtocolSession) {
        matrixSession = session
        sessions.add(session)
    }

    fun getLemmySessions(): List<IProtocolSession> {
        return lemmyHolder.getAllSessions().also { sessions ->
            sessions.filter { it !in this.sessions }.forEach { this.sessions.add(it) }
        }
    }

    fun getLemmySession(instanceUrl: String): IProtocolSession? {
        val s = lemmyHolder.getSession(instanceUrl)?.also { session ->
            if (session !in sessions) sessions.add(session)
        }
        return s
    }

    fun getAllSessions(): List<IProtocolSession> = sessions.toList()

    fun getSessionsByType(type: ProtocolType): List<IProtocolSession> {
        return sessions.filter { it.protocolType == type }
    }

    fun getUnifiedChats(): List<IProtocolRoom> {
        return sessions.flatMap { it.getChats() }
    }

    fun getSessionForChat(protocolType: ProtocolType): IProtocolSession? {
        return sessions.find { it.protocolType == protocolType }
    }

    fun closeAll() {
        sessions.forEach { it.close() }
        sessions.clear()
        lemmyHolder.closeAll()
    }
}
