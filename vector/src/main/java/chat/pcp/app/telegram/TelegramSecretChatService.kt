package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramSecretChatService(private val client: TdLibClient) {

    fun createNewSecretChat(userId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "createNewSecretChat")
            put("user_id", userId)
        })
    }

    fun acceptSecretChat(secretChatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "openSecretChat")
            put("secret_chat_id", secretChatId)
        })
    }

    fun closeSecretChat(secretChatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "closeSecretChat")
            put("secret_chat_id", secretChatId)
        })
    }

    fun getSecretChat(secretChatId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getSecretChat")
            put("secret_chat_id", secretChatId)
        })
    }

    fun setSecretChatTtl(secretChatId: Long, ttl: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendChatSetTtlMessage")
            put("chat_id", secretChatId)
            put("ttl", ttl)
        })
    }

    fun reportSecretChatFingerprint(secretChatId: Long, fingerprint: String) {
        val keyHash = client.execute(JSONObject().apply {
            put("@type", "getSecretChatKeyHash")
            put("secret_chat_id", secretChatId)
        })
        // The fingerprint comparison is done by the UI
    }

    fun getEmailsAddressContact(secretChatId: Long, emailAddresses: List<String>) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getEmailsAddressContact")
            put("email_addresses", org.json.JSONArray(emailAddresses))
        })
    }
}
