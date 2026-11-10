package chat.pcp.app.native
import org.json.JSONObject
import org.json.JSONArray
import timber.log.Timber
/**
 * JNI bridge to the pcp_native C++ library.
 * Handles date validation, MSC3030 URL construction, and response parsing.
 */
object PcpNative {
    private var isLoaded = false
    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("pcp_native")
                isLoaded = true
                Timber.d("pcp_native loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load pcp_native")
                // Fallback: use pure Kotlin implementation
            }
        }
    }

    @JvmStatic
    external fun nativeParseRelation(
        eventJson: String,
        allowedTypes: String
    ): String

    @JvmStatic
    external fun nativeBuildTranslateRequest(
        text: String,
        sourceLang: String,
        targetLang: String,
        apiEndpoint: String,
        apiToken: String,
        model: String
    ): String

    @JvmStatic
    external fun nativeParseTranslateResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    @JvmStatic external fun nativeMarkdownToHtml(markdown: String, enableTables: Boolean): String
    @JvmStatic external fun nativeBuildLlmRequest(prompt: String, provider: Int, endpoint: String, token: String, model: String, systemPrompt: String, temp: Float, maxTokens: Int): String
    @JvmStatic external fun nativeBuildLlmHeaders(provider: Int, token: String): String
    @JvmStatic external fun nativeParseLlmResponse(body: String, statusCode: Int, provider: Int): String
    @JvmStatic external fun nativeNotifSetMode(mode: Int)
    @JvmStatic external fun nativeNotifGetMode(): Int
    @JvmStatic external fun nativeFormatCountToShortDecimal(value: Int): String
    @JvmStatic external fun nativeParseSlashCommand(text: String, formatted: String?, isThread: Boolean, devMode: Boolean): String
    @JvmStatic external fun nativeFormatNoticeEvent(eventJson: String, senderName: String, isDm: Boolean, isSentByMe: Boolean, currentUserId: String): String
    @JvmStatic external fun nativeFormatRedactedEvent(eventJson: String, senderName: String): String
    @JvmStatic external fun nativeFormatDisplayableEvent(eventJson: String, senderName: String, isDm: Boolean, appendAuthor: Boolean): String
    @JvmStatic external fun nativeFormatThreadSummary(eventJson: String, latestEdition: String): String
    @JvmStatic external fun nativeFormatEventDetails(eventJson: String): String
    @JvmStatic external fun nativeFormatRoomHistoryVis(visibility: String, forNotice: Boolean): String
    @JvmStatic external fun nativeClassifyError(errorCode: String, httpCode: Int, errorMessage: String, isNetwork: Boolean, isUnknownHost: Boolean): String
}
