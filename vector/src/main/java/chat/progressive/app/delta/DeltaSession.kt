package chat.progressive.app.delta

import android.content.Context
import chat.progressive.app.native.ProgressiveNative
import chat.progressive.app.protocol.*
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
            val info = JSONObject(ProgressiveNative.dcGetInfo(nativeHandle) ?: "{}")
            info.optString("displayname", info.optString("addr", "unknown"))
        } catch (e: Exception) {
            "unknown"
        }

    override val isLoggedIn = MutableStateFlow(false)
    override val connectionState = MutableStateFlow(ConnectionState.DISCONNECTED)

    private val listeners = mutableListOf<ProtocolSessionListener>()

    override fun open() {
        if (ProgressiveNative.dcIsConfigured(nativeHandle)) {
            connectionState.value = ConnectionState.CONNECTING
            ProgressiveNative.dcStartIo(nativeHandle)
            isLoggedIn.value = true
            connectionState.value = ConnectionState.CONNECTED
        }
    }

    override fun close() {
        ProgressiveNative.dcStopIo(nativeHandle)
        connectionState.value = ConnectionState.DISCONNECTED
        isLoggedIn.value = false
    }

    override suspend fun sendMessage(chatId: Long, text: String) {
        ProgressiveNative.dcSendText(nativeHandle, chatId, text)
    }

    override fun getChats(): List<IProtocolRoom> {
        val json = ProgressiveNative.dcGetChats(nativeHandle) ?: return emptyList()
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
        return ProgressiveNative.dcConfigure(nativeHandle, addr, password)
    }

    fun isConfigured(): Boolean {
        return ProgressiveNative.dcIsConfigured(nativeHandle)
    }

    fun getMessages(chatId: Long, limit: Int): List<IProtocolMessage> {
        val json = ProgressiveNative.dcGetMessages(nativeHandle, chatId) ?: return emptyList()
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
        ProgressiveNative.dcMarkSeenMsgs(nativeHandle, chatId)
    }

    fun leaveChat(chatId: Long) {
        ProgressiveNative.dcLeaveChat(nativeHandle, chatId)
    }

    fun deleteChat(chatId: Long) {
        ProgressiveNative.dcDeleteChat(nativeHandle, chatId)
    }

    fun checkQr(qr: String): Int {
        return ProgressiveNative.dcCheckQr(nativeHandle, qr)
    }

    // --- Contacts ---

    fun getContacts(): List<DeltaContact> {
        val json = ProgressiveNative.dcGetContacts(nativeHandle) ?: return emptyList()
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
        return ProgressiveNative.dcCreateContact(nativeHandle, name, addr)
    }

    fun blockContact(contactId: Long, block: Boolean) {
        ProgressiveNative.dcBlockContact(nativeHandle, contactId, if (block) 1 else 0)
    }

    fun getContactEncryptionInfo(contactId: Long): String {
        return ProgressiveNative.dcGetContactEncrinfo(nativeHandle, contactId) ?: ""
    }

    // --- Groups ---

    fun createGroupChat(verified: Boolean, name: String): Long {
        return ProgressiveNative.dcCreateGroupChat(nativeHandle, if (verified) 1 else 0, name)
    }

    fun addContactToChat(chatId: Long, contactId: Long) {
        ProgressiveNative.dcAddContactToChat(nativeHandle, chatId, contactId)
    }

    fun removeContactFromChat(chatId: Long, contactId: Long) {
        ProgressiveNative.dcRemoveContactFromChat(nativeHandle, chatId, contactId)
    }

    fun setChatName(chatId: Long, name: String): Boolean {
        return ProgressiveNative.dcSetChatName(nativeHandle, chatId, name) != 0
    }

    fun setChatProfileImage(chatId: Long, imagePath: String) {
        ProgressiveNative.dcSetChatProfileImage(nativeHandle, chatId, imagePath)
    }

    // --- SecureJoin ---

    fun getSecureJoinQr(chatId: Long): String {
        return ProgressiveNative.dcGetSecurejoinQr(nativeHandle, chatId) ?: ""
    }

    fun joinSecureJoin(qr: String): Int {
        return ProgressiveNative.dcJoinSecurejoin(nativeHandle, qr)
    }

    // --- Backup ---

    fun imex(what: Int, param1: String = "", param2: String = ""): String {
        return ProgressiveNative.dcImex(nativeHandle, what, param1, param2) ?: ""
    }

    fun initiateKeyTransfer(): String {
        return ProgressiveNative.dcInitiateKeyTransfer(nativeHandle) ?: ""
    }

    // --- Config ---

    fun setConfig(key: String, value: String) {
        ProgressiveNative.dcSetConfig(nativeHandle, key, value)
    }

    fun getConfig(key: String): String {
        return ProgressiveNative.dcGetConfig(nativeHandle, key) ?: ""
    }

    // --- Files ---

    fun sendFile(chatId: Long, filePath: String, mimeType: String = "application/octet-stream"): Long {
        return ProgressiveNative.dcSendFile(nativeHandle, chatId, filePath, mimeType)
    }

    // --- Fresh messages ---

    fun getFreshMessages(): String {
        return ProgressiveNative.dcGetFreshMsgs(nativeHandle) ?: "[]"
    }

    // --- Reactions ---

    fun sendReaction(chatId: Long, msgId: Long, reaction: String) {
        ProgressiveNative.dcSendReaction(nativeHandle, chatId, msgId, reaction)
    }

    // --- Edit ---

    fun editMessage(chatId: Long, msgId: Long, newText: String) {
        ProgressiveNative.dcEditMessage(nativeHandle, chatId, msgId, newText)
    }

    // --- Voice ---

    fun sendVoice(chatId: Long, filePath: String) {
        ProgressiveNative.dcSendVoice(nativeHandle, chatId, filePath)
    }

    // --- Mute ---

    fun muteChat(chatId: Long, durationMs: Long) {
        ProgressiveNative.dcMuteChat(nativeHandle, chatId, durationMs)
    }

    // --- Get chat media ---

    fun getChatMedia(chatId: Long): String {
        return ProgressiveNative.dcGetChatMedia(nativeHandle, chatId) ?: "[]"
    }

    // --- Connectivity ---

    fun getConnectivityHtml(): String {
        return ProgressiveNative.dcGetConnectivityHtml(nativeHandle) ?: ""
    }

    fun getNativeHandle(): Long = nativeHandle
}

data class DeltaContact(
    val id: Long,
    val name: String,
    val address: String,
    val color: String
)
