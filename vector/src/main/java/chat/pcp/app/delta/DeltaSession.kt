package chat.pcp.app.delta

import android.content.Context
import chat.pcp.app.native.PcpNative
import chat.pcp.app.protocol.*
import org.json.JSONArray
import org.json.JSONObject
import kotlinx.coroutines.flow.MutableStateFlow

class DeltaSession(
    private val context: Context,
    private val nativeHandle: Long
) : IProtocolSession {

    override val protocolType: ProtocolType = ProtocolType.DELTACHAT
    override val userId: String
        get() = try {
            val info = JSONObject(PcpNative.dcGetInfo(nativeHandle) ?: "{}")
            info.optString("displayname", info.optString("addr", "unknown"))
        } catch (e: Exception) {
            "unknown"
        }

    override val isLoggedIn = MutableStateFlow(false)
    override val connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)

    private val listeners = mutableListOf<ProtocolSessionListener>()

    override fun open() {
        if (PcpNative.dcIsConfigured(nativeHandle)) {
            connectionState.value = ConnectionState.CONNECTING
            PcpNative.dcStartIo(nativeHandle)
            isLoggedIn.value = true
            connectionState.value = ConnectionState.CONNECTED
        }
    }

    override fun close() {
        PcpNative.dcStopIo(nativeHandle)
        connectionState.value = ConnectionState.DISCONNECTED
        isLoggedIn.value = false
    }

    override suspend fun sendMessage(chatId: Long, text: String) {
        PcpNative.dcSendText(nativeHandle, chatId, text)
    }

    override fun getChats(): List<IProtocolRoom> {
        val json = PcpNative.dcGetChats(nativeHandle) ?: return emptyList()
        val array = JSONArray(json)
        val chats = mutableListOf<DeltaRoom>()
        for (i in 0 until array.length()) {
            val obj = array.getJSONObject(i)
            val chatId = obj.getLong("id")
            chats.add(DeltaRoom(
                id = chatId,
                title = obj.optString("name", "Chat $chatId"),
                lastMessage = null,
                lastMessageTime = null,
                unreadCount = obj.optInt("unread_count", 0),
                isGroup = obj.optInt("type", 0) != 100,
                avatarUrl = obj.optString("profile_image", "").takeIf { it.isNotEmpty() },
                session = this
            ))
        }
        return chats
    }

    override fun getChat(chatId: Long): IProtocolRoom? {
        return getChats().find { it.id == chatId }
    }

    override fun addListener(listener: ProtocolSessionListener) {
        listeners.add(listener)
    }

    override fun removeListener(listener: ProtocolSessionListener) {
        listeners.remove(listener)
    }

    fun configure(addr: String, password: String): Boolean {
        return PcpNative.dcConfigure(nativeHandle, addr, password)
    }

    fun isConfigured(): Boolean {
        return PcpNative.dcIsConfigured(nativeHandle)
    }

    fun getMessages(chatId: Long, limit: Int): List<IProtocolMessage> {
        val json = PcpNative.dcGetMessages(nativeHandle, chatId) ?: return emptyList()
        val array = JSONArray(json)
        val msgs = mutableListOf<DeltaMessage>()
        for (i in 0 until array.length()) {
            val obj = array.getJSONObject(i)
            msgs.add(DeltaMessage(
                id = obj.getLong("id"),
                chatId = obj.getLong("chat_id"),
                senderId = obj.optLong("from_id", 0).toString(),
                senderName = obj.optString("sender_name", "Unknown"),
                text = obj.optString("text", ""),
                timestamp = obj.optLong("timestamp", 0),
                isOutgoing = obj.optInt("is_outgoing", 0) == 1,
                contentType = ProtocolContentType.TEXT
            ))
        }
        return msgs
    }

    fun markAsRead(chatId: Long) {
        PcpNative.dcMarkSeenMsgs(nativeHandle, chatId)
    }

    fun leaveChat(chatId: Long) {
        PcpNative.dcLeaveChat(nativeHandle, chatId)
    }

    fun deleteChat(chatId: Long) {
        PcpNative.dcDeleteChat(nativeHandle, chatId)
    }

    fun checkQr(qr: String): Int {
        return PcpNative.dcCheckQr(nativeHandle, qr)
    }

    // --- Contacts ---

    fun getContacts(): List<DeltaContact> {
        val json = PcpNative.dcGetContacts(nativeHandle) ?: return emptyList()
        val array = JSONArray(json)
        val contacts = mutableListOf<DeltaContact>()
        for (i in 0 until array.length()) {
            val obj = array.getJSONObject(i)
            contacts.add(DeltaContact(
                id = obj.getLong("id"),
                name = obj.optString("name", ""),
                address = obj.optString("address", ""),
                color = obj.optString("color", "")
            ))
        }
        return contacts
    }

    fun createContact(name: String, addr: String): Long {
        return PcpNative.dcCreateContact(nativeHandle, name, addr)
    }

    fun blockContact(contactId: Long, block: Boolean) {
        PcpNative.dcBlockContact(nativeHandle, contactId, if (block) 1 else 0)
    }

    fun getContactEncryptionInfo(contactId: Long): String {
        return PcpNative.dcGetContactEncrinfo(nativeHandle, contactId) ?: ""
    }

    // --- Groups ---

    fun createGroupChat(verified: Boolean, name: String): Long {
        return PcpNative.dcCreateGroupChat(nativeHandle, if (verified) 1 else 0, name)
    }

    fun addContactToChat(chatId: Long, contactId: Long) {
        PcpNative.dcAddContactToChat(nativeHandle, chatId, contactId)
    }

    fun removeContactFromChat(chatId: Long, contactId: Long) {
        PcpNative.dcRemoveContactFromChat(nativeHandle, chatId, contactId)
    }

    fun setChatName(chatId: Long, name: String): Boolean {
        return PcpNative.dcSetChatName(nativeHandle, chatId, name) != 0
    }

    fun setChatProfileImage(chatId: Long, imagePath: String) {
        PcpNative.dcSetChatProfileImage(nativeHandle, chatId, imagePath)
    }

    // --- SecureJoin ---

    fun getSecureJoinQr(chatId: Long): String {
        return PcpNative.dcGetSecurejoinQr(nativeHandle, chatId) ?: ""
    }

    fun joinSecureJoin(qr: String): Int {
        return PcpNative.dcJoinSecurejoin(nativeHandle, qr)
    }

    // --- Backup ---

    fun imex(what: Int, param1: String = "", param2: String = ""): String {
        return PcpNative.dcImex(nativeHandle, what, param1, param2) ?: ""
    }

    fun initiateKeyTransfer(): String {
        return PcpNative.dcInitiateKeyTransfer(nativeHandle) ?: ""
    }

    // --- Config ---

    fun setConfig(key: String, value: String) {
        PcpNative.dcSetConfig(nativeHandle, key, value)
    }

    fun getConfig(key: String): String {
        return PcpNative.dcGetConfig(nativeHandle, key) ?: ""
    }

    // --- Files ---

    fun sendFile(chatId: Long, filePath: String, mimeType: String = "application/octet-stream"): Long {
        return PcpNative.dcSendFile(nativeHandle, chatId, filePath, mimeType)
    }

    // --- Fresh messages ---

    fun getFreshMessages(): String {
        return PcpNative.dcGetFreshMsgs(nativeHandle) ?: "[]"
    }

    // --- Reactions ---

    fun sendReaction(chatId: Long, msgId: Long, reaction: String) {
        PcpNative.dcSendReaction(nativeHandle, chatId, msgId, reaction)
    }

    // --- Edit ---

    fun editMessage(chatId: Long, msgId: Long, newText: String) {
        PcpNative.dcEditMessage(nativeHandle, chatId, msgId, newText)
    }

    // --- Voice ---

    fun sendVoice(chatId: Long, filePath: String) {
        PcpNative.dcSendVoice(nativeHandle, chatId, filePath)
    }

    // --- Mute ---

    fun muteChat(chatId: Long, durationMs: Long) {
        PcpNative.dcMuteChat(nativeHandle, chatId, durationMs)
    }

    // --- Get chat media ---

    fun getChatMedia(chatId: Long): String {
        return PcpNative.dcGetChatMedia(nativeHandle, chatId) ?: "[]"
    }

    // --- Connectivity ---

    fun getConnectivityHtml(): String {
        return PcpNative.dcGetConnectivityHtml(nativeHandle) ?: ""
    }

    fun getNativeHandle(): Long = nativeHandle
}

data class DeltaContact(
    val id: Long,
    val name: String,
    val address: String,
    val color: String
)
