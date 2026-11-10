package chat.pcp.app.reddit

import android.content.Context
import chat.pcp.app.protocol.IProtocolSession

class RedditSessionHolder(private val context: Context) {

    private val sessions = mutableMapOf<String, RedditSession>()
    private var defaultSession: RedditSession? = null

    fun getOrCreateAnonymous(): RedditSession {
        if (defaultSession == null) {
            defaultSession = RedditSession(RedditAccount("anonymous"))
            defaultSession!!.open()
        }
        return defaultSession!!
    }

    fun getOrCreateAccount(account: RedditAccount): RedditSession {
        return sessions.getOrPut(account.username) {
            val session = RedditSession(account)
            session.open()
            session
        }
    }

    fun getAllSessions(): List<IProtocolSession> {
        val result = mutableListOf<IProtocolSession>()
        defaultSession?.let { result.add(it) }
        result.addAll(sessions.values)
        return result
    }

    fun getSession(username: String): RedditSession? {
        return sessions[username] ?: defaultSession
    }

    fun removeAccount(username: String) {
        sessions.remove(username)?.close()
    }

    fun closeAll() {
        defaultSession?.close()
        defaultSession = null
        sessions.values.forEach { it.close() }
        sessions.clear()
    }
}
