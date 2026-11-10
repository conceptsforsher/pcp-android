package chat.pcp.app.irc

import android.content.Context
import dagger.hilt.android.qualifiers.ApplicationContext
import javax.inject.Inject
import javax.inject.Singleton

@Singleton
class IrcSessionHolder @Inject constructor(
    @ApplicationContext private val context: Context
) {

    private var session: IrcSession? = null
    private var ircClient: IrcClient? = null

    fun getOrCreateSession(): IrcSession? {
        if (!IrcNative.ensureLoaded()) {
            android.util.Log.w("IrcSessionHolder", "IRC native not available")
            return null
        }

        if (session == null) {
            ircClient = IrcClient()
            session = IrcSession(ircClient!!)
        }

        return session
    }

    fun getExistingSession(): IrcSession? = session
    fun getIrcClient(): IrcClient? = ircClient

    fun close() {
        ircClient?.disconnect()
        session = null
        ircClient = null
    }
}
