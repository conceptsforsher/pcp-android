package chat.pcp.app.xmpp

import chat.pcp.app.protocol.ConnectionState
import org.json.JSONObject
import timber.log.Timber

class XmppSessionHolder {

    private var session: XmppSession? = null
    private var nativeHandle: Long = 0

    fun getOrCreateSession(): XmppSession? {
        if (session != null && nativeHandle != 0L) return session

        XmppNative.ensureLoaded()

        val config = buildDefaultConfig()
        if (config.isEmpty()) {
            Timber.w("XMPP config is empty, skipping session creation")
            return null
        }

        nativeHandle = XmppNative.nativeXmppConnect(config)
        if (nativeHandle == 0L) {
            Timber.e("Failed to create XMPP native client")
            return null
        }

        val jid = XmppNative.nativeXmppGetOwnJid(nativeHandle)
        session = XmppSession(nativeHandle, jid)
        session!!.open()
        session!!.updateConnectionState(ConnectionState.CONNECTING)

        return session
    }

    fun connectWithAccount(jid: String, password: String, server: String = "", port: Int = 5222): XmppSession? {
        XmppNative.ensureLoaded()

        val config = JSONObject().apply {
            put("jid", jid)
            put("password", password)
            put("server", server)
            put("port", port)
            put("resource", "PCP")
            put("useTls", true)
        }.toString()

        nativeHandle = XmppNative.nativeXmppConnect(config)
        if (nativeHandle == 0L) {
            Timber.e("Failed to connect XMPP client")
            return null
        }

        session = XmppSession(nativeHandle, jid)
        session!!.open()
        session!!.updateConnectionState(ConnectionState.CONNECTING)

        return session
    }

    fun getSession(): XmppSession? = session
    fun getHandle(): Long = nativeHandle

    fun close() {
        session?.close()
        session = null
        nativeHandle = 0
    }

    private fun buildDefaultConfig(): String {
        return try {
            JSONObject().apply {
                put("jid", "")
                put("password", "")
                put("server", "")
                put("port", 5222)
                put("resource", "PCP")
                put("useTls", true)
            }.toString()
        } catch (e: Exception) {
            ""
        }
    }
}
