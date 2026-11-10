package chat.pcp.app.telegram

import chat.pcp.app.protocol.ProtocolContentType
import org.json.JSONObject

data class TelegramPhoto(
    val hasStickers: Boolean,
    val minithumbnail: ByteArray?,
    val sizes: List<PhotoSize>,
    val isSpoiler: Boolean
) {
    data class PhotoSize(
        val type: String,
        val photoId: Long,
        val width: Int,
        val height: Int,
        val fileId: Int,
        val fileUniqueId: Int,
        val fileSize: Long
    )
}

data class TelegramVoice(
    val duration: Int,
    val waveform: ByteArray?,
    val mimeType: String,
    val voiceId: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long,
    val isListened: Boolean
)

data class TelegramVideo(
    val duration: Int,
    val width: Int,
    val height: Int,
    val mimeType: String,
    val thumbnail: TelegramThumbnail?,
    val videoId: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long,
    val supportsStreaming: Boolean,
    val isSpoiler: Boolean
)

data class TelegramVideoNote(
    val duration: Int,
    val length: Int,
    val minithumbnail: ByteArray?,
    val thumbnail: TelegramThumbnail?,
    val videoId: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long,
    val isViewed: Boolean
)

data class TelegramAnimation(
    val duration: Int,
    val width: Int,
    val height: Int,
    val mimeType: String,
    val thumbnail: TelegramThumbnail?,
    val animationId: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long,
    val isSpoiler: Boolean
)

data class TelegramSticker(
    val setId: Long,
    val width: Int,
    val height: Int,
    val emoji: String?,
    val format: StickerFormat,
    val fullType: StickerFullType,
    val outline: ByteArray?,
    val thumbnail: TelegramThumbnail?,
    val stickerId: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long,
    val isAnimated: Boolean,
    val isVideo: Boolean
) {
    enum class StickerFormat { STATIC, ANIMATED, VIDEO }
    sealed class StickerFullType {
        data class Regular(val premiumAnimation: ByteArray?) : StickerFullType()
        data class Mask(val maskPosition: MaskPosition?) : StickerFullType()
        data class CustomEmoji(val customEmojiId: Long, val needsRepainting: Boolean) : StickerFullType()
    }
    data class MaskPosition(val point: String, val xShift: Double, val yShift: Double, val scale: Double)
}

data class TelegramStickerSet(
    val id: Long,
    val title: String,
    val name: String,
    val thumbnail: TelegramThumbnail?,
    val thumbnailOutline: ByteArray?,
    val isInstalled: Boolean,
    val isArchived: Boolean,
    val isOfficial: Boolean,
    val format: String,
    val stickerType: String,
    val isViewed: Boolean,
    val stickers: List<TelegramSticker>,
    val covers: List<TelegramSticker>
)

data class TelegramThumbnail(
    val format: String,
    val width: Int,
    val height: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long
)

data class TelegramDocument(
    val fileName: String,
    val mimeType: String,
    val minithumbnail: ByteArray?,
    val thumbnail: TelegramThumbnail?,
    val documentId: Int,
    val fileId: Int,
    val fileUniqueId: Int,
    val fileSize: Long
)

data class TelegramPoll(
    val id: Long,
    val question: String,
    val options: List<PollOption>,
    val totalVoterCount: Int,
    val recentVoterIds: List<Long>,
    val isAnonymous: Boolean,
    val pollType: PollKind,
    val openPeriod: Int,
    val closeDate: Long,
    val isClosed: Boolean,
    val allowMultipleAnswers: Boolean
) {
    data class PollOption(
        val text: String,
        val voterCount: Int,
        val votePercentage: Int,
        val isChosen: Boolean,
        val isBeingChosen: Boolean
    )
    enum class PollKind { REGULAR, QUIZ }
}

data class TelegramLocation(
    val latitude: Double,
    val longitude: Double,
    val horizontalAccuracy: Double,
    val livePeriod: Int,
    val heading: Int,
    val proximityAlertRadius: Int
)

data class TelegramContact(
    val firstName: String,
    val lastName: String,
    val vCard: String?,
    val phoneNumber: String,
    val userId: Long
)

data class TelegramDice(
    val emoji: String,
    val value: Int
)

data class TelegramGame(
    val id: Long,
    val shortName: String,
    val title: String,
    val description: String,
    val photo: TelegramPhoto?,
    val animation: TelegramAnimation?
)

data class TelegramWebPage(
    val url: String,
    val displayUrl: String,
    val type: String,
    val siteName: String?,
    val title: String?,
    val description: String?,
    val photo: TelegramPhoto?,
    val embedUrl: String?,
    val embedType: String?,
    val embedWidth: Int,
    val embedHeight: Int,
    val duration: Int,
    val author: String?,
    val animation: TelegramAnimation?,
    val audio: TelegramVoice?,
    val document: TelegramDocument?,
    val sticker: TelegramSticker?,
    val video: TelegramVideo?,
    val videoNote: TelegramVideoNote?,
    val voiceNote: TelegramVoice?,
    val hasInstantView: Boolean
)

data class TelegramForwardInfo(
    val origin: ForwardOrigin,
    val date: Long,
    val source: String?,
    val fromChatId: Long,
    val fromMessageId: Long,
    val isImported: Boolean
) {
    sealed class ForwardOrigin {
        data class User(val senderUserId: Long) : ForwardOrigin()
        data class Chat(val senderChatId: Long, val authorSignature: String? = null) : ForwardOrigin()
        data class Channel(val chatId: Long, val messageId: Long, val authorSignature: String? = null) : ForwardOrigin()
        data class HiddenUser(val senderName: String) : ForwardOrigin()
    }
}

data class TelegramCallInfo(
    val id: Int,
    val userId: Long,
    val isOutgoing: Boolean,
    val isVideo: Boolean,
    val duration: Int,
    val discardReason: CallDiscardReason?
) {
    enum class CallDiscardReason { EMPTY, MISSED, DECLINED, DISCONNECTED, HUNG_UP }
}

data class TelegramBlockQuote(
    val text: String,
    val credit: String?
)

data class TelegramBotCommand(
    val command: String,
    val description: String
)

data class TelegramInlineKeyboardButton(
    val text: String,
    val type: String,
    val data: String?,
    val url: String?
)

data class TelegramInlineKeyboardRow(
    val buttons: List<TelegramInlineKeyboardButton>
)

data class TelegramReplyMarkup(
    val rows: List<TelegramInlineKeyboardRow>
)

fun JSONObject.optStringOrNull(key: String): String? {
    return if (has(key) && !isNull(key)) optString(key) else null
}

fun JSONObject.getFloat(key: String): Float {
    return optDouble(key, 0.0).toFloat()
}
