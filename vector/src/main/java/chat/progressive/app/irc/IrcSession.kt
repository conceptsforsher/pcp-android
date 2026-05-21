package chat.progressive.app.irc

import chat.progressive.app.protocol.ConnectionState
import chat.progressive.app.protocol.IProtocolMessage
import chat.progressive.app.protocol.IProtocolRoom
import chat.progressive.app.protocol.IProtocolSession
import chat.progressive.app.protocol.ProtocolError
import chat.progressive.app.protocol.ProtocolSessionListener
import chat.progressive.app.protocol.ProtocolType
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONObject
import timber.log.Timber

class IrcSession(
    private val ircClient: IrcClient
) : IProtocolSession, IrcClient.IrcUpdateListener {

    override val protocolType: ProtocolType = ProtocolType.IRC

    private val _isLoggedIn = MutableStateFlow(false)
    override val isLoggedIn: StateFlow<Boolean> = _isLoggedIn

    private val _connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)
    override val connectionState: StateFlow<ConnectionState> = _connectionState

    override val userId: String get() = ircClient.currentNick

    private val listeners = mutableListOf<ProtocolSessionListener>()
    private val channels = mutableMapOf<String, IProtocolRoom>()

    init {
        ircClient.addListener(this)
    }

    override fun open() {
        _connectionState.value = ConnectionState.CONNECTING
    }

    override fun close() {
        ircClient.removeListener(this)
        ircClient.disconnect()
    }

    override suspend fun sendMessage(chatId: String, text: String) {
        ircClient.sendMessage(chatId, text)
    }

    override fun getChats(): List<IProtocolRoom> {
        return channels.values.toList()
    }

    override fun getChat(chatId: String): IProtocolRoom? {
        return channels[chatId]
    }

    override fun addListener(listener: ProtocolSessionListener) {
        listeners.add(listener)
    }

    override fun removeListener(listener: ProtocolSessionListener) {
        listeners.remove(listener)
    }

    override fun onStateChanged(state: IrcClient.State) {
        _connectionState.value = when (state) {
            IrcClient.State.DISCONNECTED -> ConnectionState.DISCONNECTED
            IrcClient.State.CONNECTING -> ConnectionState.CONNECTING
            IrcClient.State.CONNECTED -> ConnectionState.CONNECTED
            IrcClient.State.REGISTERED -> ConnectionState.REGISTERED
            IrcClient.State.ERROR -> ConnectionState.ERROR
        }
        if (state == IrcClient.State.REGISTERED) {
            _isLoggedIn.value = true
        }
        listeners.forEach { it.onConnectionStateChanged(connectionState.value) }
    }

    override fun onNewMessage(channel: String, message: IrcMessage) {
        if (!channels.containsKey(channel) && channel.startsWith("#")) {
            val room = IrcRoom(
                id = channel,
                title = channel,
                isGroup = true,
                ircClient = ircClient
            )
            channels[channel] = room
            listeners.forEach { it.onChatListChanged() }
        }
        listeners.forEach { it.onNewMessage(channel, message) }
    }

    override fun onChannelChanged(channel: String) {
        listeners.forEach { it.onChatListChanged() }
    }

    override fun onError(code: Int, message: String) {
        listeners.forEach { it.onError(ProtocolError(code, message)) }
    }
}
