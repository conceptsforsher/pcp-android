package chat.pcp.app.irc

import timber.log.Timber

object IrcNative {

    private var isLoaded = false

    fun ensureLoaded(): Boolean {
        if (!isLoaded) {
            try {
                System.loadLibrary("pcp_native")
                isLoaded = true
                Timber.d("pcp_native loaded — IRC bridge available")
            } catch (e: UnsatisfiedLinkError) {
                Timber.w(e, "Failed to load pcp_native — IRC unavailable")
            }
        }
        return isLoaded
    }

    val isAvailable: Boolean get() = isLoaded

    @JvmStatic
    external fun nativeIrcCreateClient(): Long

    @JvmStatic
    external fun nativeIrcConnect(
        clientId: Long, host: String, port: Int, tls: Boolean,
        nick: String, user: String, realName: String, password: String
    )

    @JvmStatic
    external fun nativeIrcDisconnect(clientId: Long)

    @JvmStatic
    external fun nativeIrcSendRaw(clientId: Long, line: String)

    @JvmStatic
    external fun nativeIrcJoin(clientId: Long, channel: String)

    @JvmStatic
    external fun nativeIrcPart(clientId: Long, channel: String, reason: String)

    @JvmStatic
    external fun nativeIrcPrivmsg(clientId: Long, target: String, text: String)

    @JvmStatic
    external fun nativeIrcNotice(clientId: Long, target: String, text: String)

    @JvmStatic
    external fun nativeIrcNick(clientId: Long, newNick: String)

    @JvmStatic
    external fun nativeIrcTopic(clientId: Long, channel: String, newTopic: String)

    @JvmStatic
    external fun nativeIrcKick(clientId: Long, channel: String, nick: String, reason: String)

    @JvmStatic
    external fun nativeIrcDestroyClient(clientId: Long)

    @JvmStatic
    external fun nativeIrcSetCallbackObject(callback: IrcCallback)
}
