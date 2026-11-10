package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject
import timber.log.Timber

object TelegramContentParser {

    fun parseMessage(msgJson: JSONObject): TelegramRichMessage {
        val id = msgJson.optLong("id", 0)
        val chatId = msgJson.optLong("chat_id", 0)
        val date = msgJson.optLong("date", 0)
        val senderIdObj = msgJson.optJSONObject("sender_id")
        val senderId = senderIdObj?.let {
            when {
                it.has("user_id") -> "user_${it.getLong("user_id")}"
                it.has("chat_id") -> "chat_${it.getLong("chat_id")}"
                else -> ""
            }
        } ?: ""
        val isOutgoing = msgJson.optBoolean("is_outgoing", false)
        val content = msgJson.optJSONObject("content") ?: return TelegramRichMessage.Empty

        val contentType = content.optString("@type", "")
        val replyToMsgId = msgJson.optLong("reply_to_message_id", 0).takeIf { it > 0 }
        val forwardInfo = msgJson.optJSONObject("forward_info")?.let { parseForwardInfo(it) }
        val editDate = msgJson.optLong("edit_date", 0).takeIf { it > 0 }
        val threadId = msgJson.optLong("message_thread_id", 0).takeIf { it > 0 }
        val viaBotUserId = msgJson.optLong("via_bot_user_id", 0).takeIf { it > 0 }
        val mediaAlbumId = msgJson.optLong("media_album_id", 0).takeIf { it > 0 }
        val ttl = msgJson.optInt("ttl", 0).takeIf { it > 0 }

        val parsed = parseContent(content)
        val replyMarkup = content.optJSONObject("reply_markup")?.let { parseReplyMarkup(it) }

        return TelegramRichMessage(
            id = id,
            chatId = chatId,
            senderId = senderId,
            isOutgoing = isOutgoing,
            date = date,
            contentType = parsed.type,
            text = parsed.text,
            caption = parsed.caption,
            photo = parsed.photo,
            video = parsed.video,
            voice = parsed.voice,
            videoNote = parsed.videoNote,
            animation = parsed.animation,
            sticker = parsed.sticker,
            document = parsed.document,
            poll = parsed.poll,
            location = parsed.location,
            contact = parsed.contact,
            dice = parsed.dice,
            game = parsed.game,
            webPage = parsed.webPage,
            callInfo = parsed.callInfo,
            blockQuote = parsed.blockQuote,
            replyMarkup = replyMarkup,
            forwardInfo = forwardInfo,
            replyToMessageId = replyToMsgId,
            editDate = editDate,
            threadId = threadId,
            viaBotUserId = viaBotUserId,
            mediaAlbumId = mediaAlbumId,
            ttl = ttl
        )
    }

    data class ParsedContent(
        val type: chat.pcp.app.protocol.ProtocolContentType,
        val text: String,
        val caption: String? = null,
        val photo: TelegramPhoto? = null,
        val video: TelegramVideo? = null,
        val voice: TelegramVoice? = null,
        val videoNote: TelegramVideoNote? = null,
        val animation: TelegramAnimation? = null,
        val sticker: TelegramSticker? = null,
        val document: TelegramDocument? = null,
        val poll: TelegramPoll? = null,
        val location: TelegramLocation? = null,
        val contact: TelegramContact? = null,
        val dice: TelegramDice? = null,
        val game: TelegramGame? = null,
        val webPage: TelegramWebPage? = null,
        val callInfo: TelegramCallInfo? = null,
        val blockQuote: TelegramBlockQuote? = null
    )

    @Suppress("CyclomaticComplexMethod")
    private fun parseContent(content: JSONObject): ParsedContent {
        val type = content.optString("@type", "")
        val caption = content.optJSONObject("caption")?.optString("text", null)
        val textContent = content.optJSONObject("text")?.optString("text", "")

        return when {
            type == "messageText" -> {
                val text = textContent ?: content.optString("text", "")
                val webPage = content.optJSONObject("web_page")?.let { parseWebPage(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.TEXT, text, webPage = webPage)
            }
            type == "messagePhoto" -> {
                val photo = content.optJSONObject("photo")?.let { parsePhoto(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.PHOTO, "[Photo]", caption = caption, photo = photo)
            }
            type == "messageVideo" -> {
                val video = content.optJSONObject("video")?.let { parseVideo(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.VIDEO, "[Video]", caption = caption, video = video)
            }
            type == "messageVoiceNote" -> {
                val voice = content.optJSONObject("voice_note")?.let { parseVoice(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.VOICE, "[Voice: ${voice?.duration ?: 0}s]", voice = voice)
            }
            type == "messageVideoNote" -> {
                val vn = content.optJSONObject("video_note")?.let { parseVideoNote(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.VIDEO, "[Video note]", videoNote = vn)
            }
            type == "messageAnimation" -> {
                val anim = content.optJSONObject("animation")?.let { parseAnimation(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.VIDEO, "[GIF]", caption = caption, animation = anim)
            }
            type == "messageSticker" -> {
                val sticker = content.optJSONObject("sticker")?.let { parseSticker(it) }
                val emoji = sticker?.emoji ?: "\uD83D\uDE42"
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.STICKER, emoji, sticker = sticker)
            }
            type == "messageDocument" -> {
                val doc = content.optJSONObject("document")?.let { parseDocument(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.DOCUMENT, "[File: ${doc?.fileName ?: ""}]", caption = caption, document = doc)
            }
            type == "messagePoll" -> {
                val poll = content.optJSONObject("poll")?.let { parsePoll(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.POLL, poll?.question ?: "[Poll]", poll = poll)
            }
            type == "messageLocation" || type == "messageVenue" -> {
                val loc = content.optJSONObject("location")?.let { parseLocation(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.LOCATION, "[Location]", location = loc)
            }
            type == "messageContact" -> {
                val contact = content.optJSONObject("contact")?.let { parseContact(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.CONTACT, contact?.firstName ?: "[Contact]", contact = contact)
            }
            type == "messageDice" -> {
                val emoji = content.optString("emoji", "\uD83C\uDFB2")
                val value = content.optInt("value", 0)
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.TEXT, "$emoji $value", dice = TelegramDice(emoji, value))
            }
            type == "messageGame" -> {
                val game = content.optJSONObject("game")?.let { parseGame(it) }
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.TEXT, game?.title ?: "[Game]", game = game)
            }
            type == "messageCall" -> {
                val call = parseCall(content)
                ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Call]", callInfo = call)
            }
            type == "messageInvoice" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.UNKNOWN, "[Invoice]")
            type == "messageChatSetTheme" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Theme changed]")
            type == "messageChatAddMembers" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Members added]")
            type == "messageChatJoinByLink" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Joined via link]")
            type == "messageChatDeleteMember" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Member removed]")
            type == "messageChatDeletePhoto" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Photo removed]")
            type == "messageChatChangePhoto" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Photo changed]")
            type == "messageChatChangeTitle" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Title changed]")
            type == "messagePinMessage" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Pinned message]")
            type == "messageScreenshotTaken" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Screenshot taken]")
            type == "messageChatSetTtl" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[TTL changed]")
            type == "messageCustomServiceAction" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Service message]")
            type == "messagePaidMedia" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.UNKNOWN, "[Paid media]")
            type == "messagePassportDataSent" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Passport data sent]")
            type == "messagePassportDataReceived" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Passport data received]")
            type == "messageProximityAlertTriggered" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Proximity alert]")
            type == "messageUnsupported" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.UNKNOWN, "[Unsupported]")
            type == "messageChatUpgradeFrom" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Upgraded to supergroup]")
            type == "messageChatUpgradeTo" -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.SYSTEM, "[Upgraded from basic group]")
            else -> ParsedContent(chat.pcp.app.protocol.ProtocolContentType.UNKNOWN, type)
        }
    }

    private fun parsePhoto(json: JSONObject): TelegramPhoto {
        val sizes = json.optJSONArray("sizes")?.let { arr ->
            (0 until arr.length()).map { i ->
                val s = arr.getJSONObject(i)
                TelegramPhoto.PhotoSize(
                    type = s.optString("type", ""),
                    photoId = s.optString("photo", "0").let { it.toLongOrNull() ?: (it.hashCode().toLong()) },
                    width = s.optInt("width", 0),
                    height = s.optInt("height", 0),
                    fileId = s.optJSONObject("photo")?.optInt("id", 0) ?: 0,
                    fileUniqueId = s.optJSONObject("photo")?.optInt("remote", 0)?.let { it xor 0x1701 } ?: 0,
                    fileSize = s.optJSONObject("photo")?.optLong("size", 0L) ?: 0L
                )
            } ?: emptyList()
        }
        return TelegramPhoto(
            hasStickers = json.optBoolean("has_stickers", false),
            minithumbnail = null,
            sizes = sizes ?: emptyList(),
            isSpoiler = false
        )
    }

    private fun parseVideo(json: JSONObject): TelegramVideo {
        val thumbnail = json.optJSONObject("minithumbnail")?.let { parseThumbnail(it) }
        val video = json.optJSONObject("video") ?: json
        return TelegramVideo(
            duration = video.optInt("duration", 0),
            width = video.optInt("width", 0),
            height = video.optInt("height", 0),
            mimeType = video.optString("mime_type", "video/mp4"),
            thumbnail = thumbnail,
            videoId = video.optJSONObject("video")?.optInt("id", 0) ?: video.optInt("id", 0),
            fileId = video.optInt("id", 0),
            fileUniqueId = video.optInt("remote", 0)?.let { it xor 0x1702 } ?: 0,
            fileSize = video.optLong("size", 0L),
            supportsStreaming = video.optBoolean("supports_streaming", false),
            isSpoiler = false
        )
    }

    private fun parseVoice(json: JSONObject): TelegramVoice {
        val voice = json.optJSONObject("voice") ?: json
        return TelegramVoice(
            duration = voice.optInt("duration", 0),
            waveform = null,
            mimeType = voice.optString("mime_type", "audio/ogg"),
            voiceId = voice.optInt("id", 0),
            fileId = voice.optInt("id", 0),
            fileUniqueId = voice.optInt("remote", 0)?.let { it xor 0x1801 } ?: 0,
            fileSize = voice.optLong("size", 0L),
            isListened = json.optBoolean("is_listened", false)
        )
    }

    private fun parseVideoNote(json: JSONObject): TelegramVideoNote {
        val vn = json.optJSONObject("video_note") ?: json
        return TelegramVideoNote(
            duration = vn.optInt("duration", 0),
            length = vn.optInt("length", 0),
            minithumbnail = null,
            thumbnail = vn.optJSONObject("thumbnail")?.let { parseThumbnail(it) },
            videoId = vn.optInt("id", 0),
            fileId = vn.optInt("id", 0),
            fileUniqueId = vn.optInt("remote", 0)?.let { it xor 0x1901 } ?: 0,
            fileSize = vn.optLong("size", 0L),
            isViewed = json.optBoolean("is_viewed", false)
        )
    }

    private fun parseAnimation(json: JSONObject): TelegramAnimation {
        val anim = json.optJSONObject("animation") ?: json
        return TelegramAnimation(
            duration = anim.optInt("duration", 0),
            width = anim.optInt("width", 0),
            height = anim.optInt("height", 0),
            mimeType = anim.optString("mime_type", "video/mp4"),
            thumbnail = anim.optJSONObject("thumbnail")?.let { parseThumbnail(it) },
            animationId = anim.optInt("id", 0),
            fileId = anim.optInt("id", 0),
            fileUniqueId = anim.optInt("remote", 0)?.let { it xor 0x2001 } ?: 0,
            fileSize = anim.optLong("size", 0L),
            isSpoiler = false
        )
    }

    private fun parseSticker(json: JSONObject): TelegramSticker {
        val sticker = json.optJSONObject("sticker") ?: json
        val type = sticker.optString("format", "")
        val format = when {
            type.contains("tgs") || type.contains("animated") -> TelegramSticker.StickerFormat.ANIMATED
            type.contains("webm") || type.contains("video") -> TelegramSticker.StickerFormat.VIDEO
            else -> TelegramSticker.StickerFormat.STATIC
        }
        return TelegramSticker(
            setId = sticker.optLong("set_id", 0),
            width = sticker.optInt("width", 0),
            height = sticker.optInt("height", 0),
            emoji = sticker.optStringOrNull("emoji"),
            format = format,
            fullType = TelegramSticker.StickerFullType.Regular(null),
            outline = null,
            thumbnail = sticker.optJSONObject("thumbnail")?.let { parseThumbnail(it) },
            stickerId = sticker.optInt("id", 0),
            fileId = sticker.optInt("id", 0),
            fileUniqueId = sticker.optInt("remote", 0)?.let { it xor 0x2101 } ?: 0,
            fileSize = sticker.optLong("size", 0L),
            isAnimated = format == TelegramSticker.StickerFormat.ANIMATED,
            isVideo = format == TelegramSticker.StickerFormat.VIDEO
        )
    }

    private fun parseDocument(json: JSONObject): TelegramDocument {
        val doc = json.optJSONObject("document") ?: json
        return TelegramDocument(
            fileName = doc.optString("file_name", ""),
            mimeType = doc.optString("mime_type", "application/octet-stream"),
            minithumbnail = null,
            thumbnail = doc.optJSONObject("thumbnail")?.let { parseThumbnail(it) },
            documentId = doc.optInt("id", 0),
            fileId = doc.optInt("id", 0),
            fileUniqueId = doc.optInt("remote", 0)?.let { it xor 0x2201 } ?: 0,
            fileSize = doc.optLong("size", 0L)
        )
    }

    private fun parsePoll(json: JSONObject): TelegramPoll {
        val poll = json.optJSONObject("poll") ?: json
        val options = poll.optJSONArray("options")?.let { arr ->
            (0 until arr.length()).map { i ->
                val o = arr.getJSONObject(i)
                TelegramPoll.PollOption(
                    text = o.optString("text", ""),
                    voterCount = o.optInt("voter_count", 0),
                    votePercentage = o.optInt("vote_percentage", 0),
                    isChosen = o.optBoolean("is_chosen", false),
                    isBeingChosen = o.optBoolean("is_being_chosen", false)
                )
            }
        } ?: emptyList()
        return TelegramPoll(
            id = poll.optLong("id", 0),
            question = poll.optString("question", ""),
            options = options,
            totalVoterCount = poll.optInt("total_voter_count", 0),
            recentVoterIds = poll.optJSONArray("recent_voter_ids")?.let { arr ->
                (0 until arr.length()).map { arr.getLong(it) }
            } ?: emptyList(),
            isAnonymous = poll.optBoolean("is_anonymous", true),
            pollType = if (poll.optJSONObject("type")?.optString("@type")?.contains("Quiz") == true)
                TelegramPoll.PollKind.QUIZ else TelegramPoll.PollKind.REGULAR,
            openPeriod = poll.optInt("open_period", 0),
            closeDate = poll.optLong("close_date", 0),
            isClosed = poll.optBoolean("is_closed", false),
            allowMultipleAnswers = poll.optBoolean("allow_multiple_answers", false)
        )
    }

    private fun parseLocation(json: JSONObject): TelegramLocation {
        val loc = json.optJSONObject("location") ?: json
        return TelegramLocation(
            latitude = loc.optDouble("latitude", 0.0),
            longitude = loc.optDouble("longitude", 0.0),
            horizontalAccuracy = loc.optDouble("horizontal_accuracy", 0.0),
            livePeriod = loc.optInt("live_period", 0),
            heading = loc.optInt("heading", 0),
            proximityAlertRadius = loc.optInt("proximity_alert_radius", 0)
        )
    }

    private fun parseContact(json: JSONObject): TelegramContact {
        val contact = json.optJSONObject("contact") ?: json
        return TelegramContact(
            firstName = contact.optString("first_name", ""),
            lastName = contact.optString("last_name", ""),
            vCard = contact.optStringOrNull("vcard"),
            phoneNumber = contact.optString("phone_number", ""),
            userId = contact.optLong("user_id", 0)
        )
    }

    private fun parseGame(json: JSONObject): TelegramGame {
        return TelegramGame(
            id = json.optLong("id", 0),
            shortName = json.optString("short_name", ""),
            title = json.optString("title", ""),
            description = json.optString("description", ""),
            photo = json.optJSONObject("photo")?.let { parsePhoto(it) },
            animation = json.optJSONObject("animation")?.let { parseAnimation(it) }
        )
    }

    private fun parseWebPage(json: JSONObject): TelegramWebPage {
        return TelegramWebPage(
            url = json.optString("url", ""),
            displayUrl = json.optString("display_url", ""),
            type = json.optString("type", ""),
            siteName = json.optStringOrNull("site_name"),
            title = json.optStringOrNull("title"),
            description = json.optStringOrNull("description"),
            photo = json.optJSONObject("photo")?.let { parsePhoto(it) },
            embedUrl = json.optStringOrNull("embed_url"),
            embedType = json.optStringOrNull("embed_type"),
            embedWidth = json.optInt("embed_width", 0),
            embedHeight = json.optInt("embed_height", 0),
            duration = json.optInt("duration", 0),
            author = json.optStringOrNull("author"),
            animation = null,
            audio = null,
            document = null,
            sticker = null,
            video = null,
            videoNote = null,
            voiceNote = null,
            hasInstantView = json.optBoolean("has_instant_view", false)
        )
    }

    private fun parseForwardInfo(json: JSONObject): TelegramForwardInfo {
        val origin = json.optJSONObject("origin")?.let { parseForwardOrigin(it) }
            ?: run {
                val senderName = json.optStringOrNull("sender_name")
                if (senderName != null) TelegramForwardInfo.ForwardOrigin.HiddenUser(senderName)
                else TelegramForwardInfo.ForwardOrigin.User(0)
            }
        return TelegramForwardInfo(
            origin = origin,
            date = json.optLong("date", 0),
            source = json.optStringOrNull("source"),
            fromChatId = json.optLong("from_chat_id", 0),
            fromMessageId = json.optLong("from_message_id", 0),
            isImported = json.optBoolean("is_imported", false)
        )
    }

    private fun parseForwardOrigin(json: JSONObject): TelegramForwardInfo.ForwardOrigin {
        return when (json.optString("@type", "")) {
            "messageForwardOriginUser" -> TelegramForwardInfo.ForwardOrigin.User(json.optLong("sender_user_id", 0))
            "messageForwardOriginChat" -> TelegramForwardInfo.ForwardOrigin.Chat(
                json.optLong("sender_chat_id", 0),
                json.optStringOrNull("author_signature")
            )
            "messageForwardOriginChannel" -> TelegramForwardInfo.ForwardOrigin.Channel(
                json.optLong("chat_id", 0),
                json.optLong("message_id", 0),
                json.optStringOrNull("author_signature")
            )
            "messageForwardOriginHiddenUser" -> TelegramForwardInfo.ForwardOrigin.HiddenUser(
                json.optString("sender_name", "")
            )
            else -> TelegramForwardInfo.ForwardOrigin.User(0)
        }
    }

    private fun parseCall(json: JSONObject): TelegramCallInfo {
        val call = json.optJSONObject("call") ?: json
        val reasonStr = call.optJSONObject("discard_reason")?.optString("@type", "")
        val reason = when (reasonStr) {
            "callDiscardReasonMissed" -> TelegramCallInfo.CallDiscardReason.MISSED
            "callDiscardReasonDeclined" -> TelegramCallInfo.CallDiscardReason.DECLINED
            "callDiscardReasonDisconnected" -> TelegramCallInfo.CallDiscardReason.DISCONNECTED
            "callDiscardReasonHungUp" -> TelegramCallInfo.CallDiscardReason.HUNG_UP
            else -> TelegramCallInfo.CallDiscardReason.EMPTY
        }.takeIf { !reasonStr.isNullOrEmpty() }
        return TelegramCallInfo(
            id = call.optInt("id", 0),
            userId = call.optLong("user_id", 0),
            isOutgoing = call.optBoolean("is_outgoing", false),
            isVideo = call.optBoolean("is_video", false),
            duration = call.optInt("duration", 0),
            discardReason = reason
        )
    }

    private fun parseThumbnail(json: JSONObject): TelegramThumbnail {
        val file = json.optJSONObject("photo") ?: json.optJSONObject("thumbnail") ?: json
        return TelegramThumbnail(
            format = file.optString("@type", ""),
            width = file.optInt("width", 0),
            height = file.optInt("height", 0),
            fileId = file.optInt("id", 0),
            fileUniqueId = file.optInt("remote", 0)?.let { it xor 0x2301 } ?: 0,
            fileSize = file.optLong("size", 0L)
        )
    }

    fun parseReplyMarkup(json: JSONObject): TelegramReplyMarkup {
        val rows = json.optJSONArray("rows")?.let { arr ->
            (0 until arr.length()).map { ri ->
                val row = arr.getJSONObject(ri)
                val buttons = row.optJSONArray("buttons")?.let { btnArr ->
                    (0 until btnArr.length()).map { bi ->
                        val btn = btnArr.getJSONObject(bi)
                        TelegramInlineKeyboardButton(
                            text = btn.optString("text", ""),
                            type = btn.optJSONObject("type")?.optString("@type", "") ?: "",
                            data = btn.optJSONObject("type")?.optStringOrNull("data"),
                            url = btn.optJSONObject("type")?.optStringOrNull("url")
                        )
                    }
                } ?: emptyList()
                TelegramInlineKeyboardRow(buttons)
            }
        } ?: emptyList()
        return TelegramReplyMarkup(rows)
    }

    fun parseBotCommands(json: JSONObject): List<TelegramBotCommand> {
        val arr = json.optJSONArray("commands") ?: return emptyList()
        return (0 until arr.length()).map { i ->
            val cmd = arr.getJSONObject(i)
            TelegramBotCommand(
                command = cmd.optString("command", ""),
                description = cmd.optString("description", "")
            )
        }
    }

    fun parseStickerSet(json: JSONObject): TelegramStickerSet {
        val stickers = json.optJSONArray("stickers")?.let { arr ->
            (0 until arr.length()).map { i -> parseSticker(arr.getJSONObject(i)) }
        } ?: emptyList()
        val covers = json.optJSONArray("covers")?.let { arr ->
            (0 until arr.length()).map { i -> parseSticker(arr.getJSONObject(i)) }
        } ?: emptyList()
        return TelegramStickerSet(
            id = json.optLong("id", 0),
            title = json.optString("title", ""),
            name = json.optString("name", ""),
            thumbnail = json.optJSONObject("thumbnail")?.let { parseThumbnail(it) },
            thumbnailOutline = null,
            isInstalled = json.optBoolean("is_installed", false),
            isArchived = json.optBoolean("is_archived", false),
            isOfficial = json.optBoolean("is_official", false),
            format = json.optString("sticker_format", ""),
            stickerType = json.optString("sticker_type", ""),
            isViewed = json.optBoolean("is_viewed", false),
            stickers = stickers,
            covers = covers
        )
    }
}

data class TelegramRichMessage(
    val id: Long,
    val chatId: Long,
    val senderId: String,
    val isOutgoing: Boolean,
    val date: Long,
    val contentType: chat.pcp.app.protocol.ProtocolContentType,
    val text: String,

    // Rich content
    val caption: String? = null,
    val photo: TelegramPhoto? = null,
    val video: TelegramVideo? = null,
    val voice: TelegramVoice? = null,
    val videoNote: TelegramVideoNote? = null,
    val animation: TelegramAnimation? = null,
    val sticker: TelegramSticker? = null,
    val document: TelegramDocument? = null,
    val poll: TelegramPoll? = null,
    val location: TelegramLocation? = null,
    val contact: TelegramContact? = null,
    val dice: TelegramDice? = null,
    val game: TelegramGame? = null,
    val webPage: TelegramWebPage? = null,
    val callInfo: TelegramCallInfo? = null,
    val blockQuote: TelegramBlockQuote? = null,
    val replyMarkup: TelegramReplyMarkup? = null,

    // Metadata
    val forwardInfo: TelegramForwardInfo? = null,
    val replyToMessageId: Long? = null,
    val editDate: Long? = null,
    val threadId: Long? = null,
    val viaBotUserId: Long? = null,
    val mediaAlbumId: Long? = null,
    val ttl: Int? = null
) {
    companion object {
        val Empty = TelegramRichMessage(
            id = 0, chatId = 0, senderId = "", isOutgoing = false,
            date = 0, contentType = chat.pcp.app.protocol.ProtocolContentType.UNKNOWN, text = ""
        )
    }
}
