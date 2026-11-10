package chat.pcp.app.telegram

class TelegramSessionHolder(context: android.content.Context) {

    private val dbDir: String
    private val filesDir: String

    private val apiId: Int
    private val apiHash: String

    private var session: TelegramSession? = null
    private var tdClient: TdLibClient? = null

    init {
        val appDir = java.io.File(context.filesDir, "telegram")
        appDir.mkdirs()
        dbDir = java.io.File(appDir, "tdlib").absolutePath
        filesDir = java.io.File(appDir, "files").absolutePath

        apiId = try {
            context.packageManager
                .getApplicationInfo(context.packageName, android.content.pm.PackageManager.GET_META_DATA)
                .metaData?.getInt("tg_api_id", 0) ?: 0
        } catch (e: Exception) {
            0
        }

        apiHash = try {
            context.packageManager
                .getApplicationInfo(context.packageName, android.content.pm.PackageManager.GET_META_DATA)
                .metaData?.getString("tg_api_hash") ?: ""
        } catch (e: Exception) {
            ""
        }
    }

    fun getOrCreateSession(): TelegramSession? {
        if (!TdLibNative.ensureLoaded()) {
            android.util.Log.w("TelegramSessionHolder", "TDLib not available")
            return null
        }

        if (session == null && apiId > 0 && apiHash.isNotEmpty()) {
            tdClient = TdLibClient(dbDir, filesDir, apiId, apiHash)
            session = TelegramSession(tdClient!!)
        }

        return session
    }

    fun getExistingSession(): TelegramSession? = session
    fun getTdClient(): TdLibClient? = tdClient

    fun close() {
        tdClient?.close()
        session = null
        tdClient = null
    }
}
