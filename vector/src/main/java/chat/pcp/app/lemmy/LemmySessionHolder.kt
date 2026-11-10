package chat.pcp.app.lemmy

import timber.log.Timber
import java.util.concurrent.ConcurrentHashMap

class LemmySessionHolder {

    private val sessions = ConcurrentHashMap<String, LemmySession>()

    fun getOrCreateSession(instanceUrl: String): LemmySession {
        return sessions.getOrPut(instanceUrl) {
            Timber.d("Creating Lemmy session for $instanceUrl")
            LemmySession(instanceUrl)
        }
    }

    fun getSession(instanceUrl: String): LemmySession? = sessions[instanceUrl]

    fun hasSession(instanceUrl: String): Boolean = sessions.containsKey(instanceUrl)

    fun removeSession(instanceUrl: String) {
        sessions.remove(instanceUrl)?.close()
    }

    fun getAllSessions(): List<LemmySession> = sessions.values.toList()

    fun closeAll() {
        sessions.values.forEach { it.close() }
        sessions.clear()
    }
}
