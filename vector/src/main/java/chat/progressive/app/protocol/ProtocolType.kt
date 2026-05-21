package chat.progressive.app.protocol

enum class ProtocolType(val displayName: String, val id: String) {
    MATRIX("Matrix", "matrix"),
    XMPP("XMPP", "xmpp"),
    TELEGRAM("Telegram", "telegram");

    companion object {
        fun fromId(id: String): ProtocolType = entries.first { it.id == id }
    }
}
