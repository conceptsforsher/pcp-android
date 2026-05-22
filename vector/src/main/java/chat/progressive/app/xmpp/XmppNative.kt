package chat.progressive.app.xmpp

import timber.log.Timber

object XmppNative {

    private var isLoaded = false
    interface XmppCallback {
        fun onConnectionChanged(json: String)
        fun onMessage(json: String)
        fun onMamMessage(json: String)
        fun onMamComplete(json: String)
        fun onPresence(json: String)
        fun onRoster(json: String)
        fun onError(json: String)
    }

    private var callback: XmppCallback? = null

    fun setCallback(cb: XmppCallback?) {
        callback = cb
    }

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
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
    external fun nativeXmppRequestMam(handle: Long, chatJid: String, beforeId: String, limit: Int)

    @JvmStatic
    external fun nativeXmppRequestMamPage(handle: Long, chatJid: String, afterId: String, limit: Int)

    @JvmStatic
    external fun nativeXmppGetRooms(handle: Long): String

    @JvmStatic
    external fun nativeXmppRequestRoomInfo(handle: Long, roomJid: String)

    @JvmStatic
    external fun nativeXmppRequestVCard(handle: Long, jid: String)

    @JvmStatic
    external fun nativeXmppGetAvatarHash(handle: Long, jid: String): String

    @JvmStatic
    fun xmppOnConnection(json: String) {
        Timber.d("xmppOnConnection: %s", json)
        callback?.onConnectionChanged(json)
    }

    @JvmStatic
    fun xmppOnMessage(json: String) {
        Timber.d("xmppOnMessage: %s", json)
        callback?.onMessage(json)
    }

    @JvmStatic
    fun xmppOnPresence(json: String) {
        Timber.d("xmppOnPresence: %s", json)
        callback?.onPresence(json)
    }

    @JvmStatic
    fun xmppOnRoster(json: String) {
        Timber.d("xmppOnRoster: %s", json)
        callback?.onRoster(json)
    }

    @JvmStatic
    fun xmppOnError(json: String) {
        Timber.d("xmppOnError: %s", json)
        callback?.onError(json)
    }

    @JvmStatic
    fun xmppOnMamMessage(json: String) {
        Timber.d("xmppOnMamMessage: %s", json)
        callback?.onMamMessage(json)
    }

    @JvmStatic
    fun xmppOnMamComplete(json: String) {
        Timber.d("xmppOnMamComplete: %s", json)
        callback?.onMamComplete(json)
    }
}
