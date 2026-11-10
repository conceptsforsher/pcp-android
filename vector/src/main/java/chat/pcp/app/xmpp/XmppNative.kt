package chat.pcp.app.xmpp

import timber.log.Timber

object XmppNative {

    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("pcp_native")
                isLoaded = true
                nativeXmppInit()
                Timber.d("XmppNative loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load XmppNative")
            }
        }
    }

    @JvmStatic
    external fun nativeXmppInit(): Long

    @JvmStatic
    external fun nativeXmppConnect(accountJson: String): Long

    @JvmStatic
    external fun nativeXmppDisconnect(handle: Long)

    @JvmStatic
    external fun nativeXmppIsConnected(handle: Long): Boolean

    @JvmStatic
    external fun nativeXmppIsAuthenticated(handle: Long): Boolean

    @JvmStatic
    external fun nativeXmppSend(handle: Long, to: String, body: String)

    @JvmStatic
    external fun nativeXmppJoinRoom(handle: Long, roomJid: String)

    @JvmStatic
    external fun nativeXmppLeaveRoom(handle: Long, roomJid: String)

    @JvmStatic
    external fun nativeXmppGetRoster(handle: Long): String

    @JvmStatic
    external fun nativeXmppGetChats(handle: Long): String

    @JvmStatic
    external fun nativeXmppSendPresence(handle: Long, status: String, show: String)

    @JvmStatic
    external fun nativeXmppGetOwnJid(handle: Long): String

    @JvmStatic
    external fun nativeXmppRunOnce(handle: Long, timeoutMs: Int)

    @JvmStatic
    external fun nativeXmppSendTyping(handle: Long, to: String, isTyping: Boolean)

    @JvmStatic
    external fun nativeXmppSetNickname(handle: Long, roomJid: String, nick: String)

    @JvmStatic
    fun xmppOnConnection(json: String) {
        Timber.d("xmppOnConnection: %s", json)
    }

    @JvmStatic
    fun xmppOnMessage(json: String) {
        Timber.d("xmppOnMessage: %s", json)
    }

    @JvmStatic
    fun xmppOnPresence(json: String) {
        Timber.d("xmppOnPresence: %s", json)
    }

    @JvmStatic
    fun xmppOnRoster(json: String) {
        Timber.d("xmppOnRoster: %s", json)
    }

    @JvmStatic
    fun xmppOnError(json: String) {
        Timber.d("xmppOnError: %s", json)
    }
}
