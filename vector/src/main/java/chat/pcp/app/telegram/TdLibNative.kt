package chat.pcp.app.telegram

import timber.log.Timber

object TdLibNative {

    private var isLoaded = false

    fun ensureLoaded(): Boolean {
        if (!isLoaded) {
            try {
                System.loadLibrary("pcp_native")
                isLoaded = true
                Timber.d("pcp_native loaded — TDLib bridge available")
            } catch (e: UnsatisfiedLinkError) {
                Timber.w(e, "Failed to load pcp_native — TDLib unavailable")
            }
        }
        return isLoaded
    }

    val isAvailable: Boolean get() = isLoaded && nativeTdPing()

    @JvmStatic
    external fun nativeTdCreateClient(): Long

    @JvmStatic
    external fun nativeTdSend(clientPtr: Long, request: String)

    @JvmStatic
    external fun nativeTdReceive(clientPtr: Long, timeout: Double): String?

    @JvmStatic
    external fun nativeTdExecute(clientPtr: Long, request: String): String?

    @JvmStatic
    external fun nativeTdDestroyClient(clientPtr: Long)

    fun nativeTdPing(): Boolean {
        return try {
            nativeTdExecute(0L, """{"@type":"getOption","name":"version"}""") != null
        } catch (e: UnsatisfiedLinkError) {
            false
        }
    }
}
