package chat.pcp.app.delta

import android.content.Context
import chat.pcp.app.native.PcpNative
import timber.log.Timber

class DeltaSessionHolder(private val context: Context) {

    private var session: DeltaSession? = null

    fun getOrCreateSession(): DeltaSession? {
        if (session != null) return session

        try {
            val handle = PcpNative.dcInit()
            if (handle == 0L) {
                Timber.w("DeltaChat native init failed, handle is 0")
                return null
            }
            session = DeltaSession(context, handle)
            return session
        } catch (e: UnsatisfiedLinkError) {
            Timber.w(e, "DeltaChat native library not available")
            return null
        }
    }

    fun isAvailable(): Boolean {
        return try {
            getOrCreateSession() != null
        } catch (e: Exception) {
            false
        }
    }

    fun getSession(): DeltaSession? = session
}
