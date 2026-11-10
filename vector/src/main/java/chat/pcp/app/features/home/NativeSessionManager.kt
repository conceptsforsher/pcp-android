package chat.pcp.app.features.home

object NativeSessionManager {
    private var handle: Long = 0
    private var userId: String = ""

    fun setActiveSession(h: Long, uid: String) {
        handle = h
        userId = uid
    }

    fun getHandle(): Long = handle
    fun getUserId(): String = userId
    fun isActive(): Boolean = handle != 0L

    fun clear() {
        handle = 0
        userId = ""
    }
}
