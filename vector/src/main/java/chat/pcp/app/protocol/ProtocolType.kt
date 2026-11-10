package chat.pcp.app.protocol

enum class ProtocolType(val displayName: String, val id: String) {
    MATRIX("Matrix", "matrix"),
    TELEGRAM("Telegram", "telegram");

    companion object {
        fun fromId(id: String): ProtocolType = entries.first { it.id == id }
    }
}
