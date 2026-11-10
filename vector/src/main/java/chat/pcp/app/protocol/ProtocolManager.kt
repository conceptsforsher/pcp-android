package chat.pcp.app.protocol

import chat.pcp.app.delta.DeltaSessionHolder

class ProtocolManager(
    private val deltaHolder: DeltaSessionHolder?
) {

    private val sessions = mutableListOf<IProtocolSession>()
    private var matrixSession: IProtocolSession? = null

    fun registerMatrixSession(session: IProtocolSession) {
        matrixSession = session
        sessions.add(session)
    }

    fun getDeltaSession(): IProtocolSession? {
        return deltaHolder?.getOrCreateSession()?.also { session ->
            if (session !in sessions) {
                sessions.add(session)
            }
        }
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
    }
}
