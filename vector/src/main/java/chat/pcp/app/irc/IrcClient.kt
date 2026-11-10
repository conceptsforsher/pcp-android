package chat.pcp.app.irc

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONObject
import timber.log.Timber
import java.time.Instant
import java.util.concurrent.ConcurrentHashMap

class IrcClient : IrcCallback {

    enum class State {
        DISCONNECTED, CONNECTING, CONNECTED, REGISTERED, ERROR
    }

    private var clientPtr: Long = 0

    private val _state = MutableStateFlow(State.DISCONNECTED)
    val state: StateFlow<State> = _state

    private val _serverHost = MutableStateFlow("")
    val serverHost: StateFlow<String> = _serverHost

    val currentNick: String get() = _currentNick
    private var _currentNick = ""

    private val messageQueue = ConcurrentHashMap<String, MutableList<IrcMessage>>()
    private val listeners = mutableListOf<IrcUpdateListener>()

    fun connect(info: IrcServerInfo) {
        if (!IrcNative.ensureLoaded()) {
            Timber.e("IRC native library not available")
            _state.value = State.ERROR
            return
        }

        clientPtr = IrcNative.nativeIrcCreateClient()
        if (clientPtr == 0L) {
            Timber.e("Failed to create IRC client")
            _state.value = State.ERROR
            return
        }

        IrcNative.nativeIrcSetCallbackObject(this)
        _currentNick = info.nick
        _serverHost.value = info.host

        _state.value = State.CONNECTING

        val user = info.user.ifEmpty { info.nick }
        val realName = info.realName.ifEmpty { info.nick }

        IrcNative.nativeIrcConnect(
            clientPtr, info.host, info.port, info.tls,
            info.nick, user, realName, info.password
        )

        Timber.d("IRC client created: $clientPtr, connecting to ${info.host}:${info.port}")
    }

    fun disconnect() {
        if (clientPtr != 0L) {
            IrcNative.nativeIrcDisconnect(clientPtr)
            IrcNative.nativeIrcDestroyClient(clientPtr)
            clientPtr = 0
        }
        _state.value = State.DISCONNECTED
    }

    fun join(channel: String) {
        if (clientPtr != 0L) {
            val ch = if (channel.startsWith("#")) channel else "#$channel"
            IrcNative.nativeIrcJoin(clientPtr, ch)
        }
    }

    fun part(channel: String, reason: String = "") {
        if (clientPtr != 0L) {
            IrcNative.nativeIrcPart(clientPtr, channel, reason)
        }
    }

    fun sendMessage(target: String, text: String) {
        if (clientPtr != 0L) {
            IrcNative.nativeIrcPrivmsg(clientPtr, target, text)
        }
    }

    fun sendNotice(target: String, text: String) {
        if (clientPtr != 0L) {
            IrcNative.nativeIrcNotice(clientPtr, target, text)
        }
    }

    fun changeNick(newNick: String) {
        if (clientPtr != 0L) {
            _currentNick = newNick
            IrcNative.nativeIrcNick(clientPtr, newNick)
        }
    }

    fun sendRaw(line: String) {
        if (clientPtr != 0L) {
            IrcNative.nativeIrcSendRaw(clientPtr, line)
        }
    }

    fun getMessagesForChannel(channel: String): List<IrcMessage> {
        return messageQueue[channel]?.toList() ?: emptyList()
    }

    fun addListener(listener: IrcUpdateListener) {
        listeners.add(listener)
    }

    fun removeListener(listener: IrcUpdateListener) {
        listeners.remove(listener)
    }

    override fun onIrcMessage(clientId: Long, json: String) {
        try {
            val obj = JSONObject(json)
            val cmd = obj.optString("cmd", "")
            val code = obj.optInt("code", 0)
            val nick = obj.optString("nick", "")
            val trailing = obj.optString("trailing", "")
            val params = obj.optJSONArray("params")

            val channel = if (params != null && params.length() > 0) {
                params.optString(0, "")
            } else ""

            val contentType = when {
                cmd == "JOIN" -> chat.pcp.app.protocol.ProtocolContentType.JOIN
                cmd == "PART" -> chat.pcp.app.protocol.ProtocolContentType.PART
                cmd == "QUIT" -> chat.pcp.app.protocol.ProtocolContentType.QUIT
                cmd == "KICK" -> chat.pcp.app.protocol.ProtocolContentType.KICK
                cmd == "MODE" -> chat.pcp.app.protocol.ProtocolContentType.MODE
                cmd == "TOPIC" || code == 332 -> chat.pcp.app.protocol.ProtocolContentType.TOPIC
                cmd == "NICK" -> chat.pcp.app.protocol.ProtocolContentType.NICK
                cmd == "NOTICE" -> chat.pcp.app.protocol.ProtocolContentType.NOTICE
                cmd == "PRIVMSG" && trailing.startsWith("\u0001ACTION") ->
                    chat.pcp.app.protocol.ProtocolContentType.ACTION
                cmd == "PRIVMSG" && trailing.startsWith("\u0001") ->
                    chat.pcp.app.protocol.ProtocolContentType.CTCP
                cmd == "PRIVMSG" -> chat.pcp.app.protocol.ProtocolContentType.TEXT
                code in 1..5 -> chat.pcp.app.protocol.ProtocolContentType.SYSTEM
                else -> chat.pcp.app.protocol.ProtocolContentType.UNKNOWN
            }

            var text = trailing
            if (contentType == chat.pcp.app.protocol.ProtocolContentType.ACTION) {
                text = trailing.removePrefix("\u0001ACTION ").removeSuffix("\u0001")
            } else if (contentType == chat.pcp.app.protocol.ProtocolContentType.CTCP) {
                text = trailing.removePrefix("\u0001").removeSuffix("\u0001")
            }

            val target = when (cmd) {
                "PRIVMSG", "NOTICE" -> {
                    if (channel == _currentNick) nick else channel
                }
                else -> channel
            }

            val msg = IrcMessage(
                id = "${clientId}_${Instant.now().toEpochMilli()}_${code}",
                chatId = target,
                senderId = nick,
                senderName = nick,
                text = text,
                timestamp = Instant.now().toEpochMilli(),
                isOutgoing = nick == _currentNick,
                contentType = contentType
            )

            val key = target.ifEmpty { "server" }
            messageQueue.getOrPut(key) { mutableListOf() }.add(msg)

            listeners.forEach { it.onNewMessage(target, msg) }

            if (cmd == "JOIN" || cmd == "PART" || cmd == "QUIT") {
                listeners.forEach { it.onChannelChanged(target) }
            }
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse IRC message")
        }
    }

    override fun onIrcState(clientId: Long, state: Int) {
        val newState = when (state) {
            0 -> State.DISCONNECTED
            1 -> State.CONNECTING
            2 -> State.CONNECTED
            3 -> State.REGISTERED
            4 -> State.ERROR
            else -> State.DISCONNECTED
        }
        _state.value = newState
        listeners.forEach { it.onStateChanged(newState) }
    }

    override fun onIrcError(clientId: Long, code: Int, message: String) {
        Timber.e("IRC error: code=$code, message=$message")
        listeners.forEach { it.onError(code, message) }
    }

    interface IrcUpdateListener {
        fun onNewMessage(channel: String, message: IrcMessage) {}
        fun onStateChanged(state: State) {}
        fun onChannelChanged(channel: String) {}
        fun onError(code: Int, message: String) {}
    }
}
