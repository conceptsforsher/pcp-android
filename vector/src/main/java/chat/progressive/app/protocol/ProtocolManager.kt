package chat.progressive.app.protocol

import chat.progressive.app.irc.IrcSessionHolder
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class ProtocolManager @Inject constructor(
    private val ircHolder: IrcSessionHolder
) {

    private val sessions = mutableListOf<IProtocolSession>()
    private var matrixSession: IProtocolSession? = null

    fun registerMatrixSession(session: IProtocolSession) {
        matrixSession = session
        if (session !in sessions) {
            sessions.add(session)
        }
    }

    fun getIrcSession(): IProtocolSession? {
        return ircHolder.getOrCreateSession()?.also { session ->
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
