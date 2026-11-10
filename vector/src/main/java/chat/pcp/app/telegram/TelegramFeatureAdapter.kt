package chat.pcp.app.telegram

import chat.pcp.app.protocol.IProtocolMessage
import chat.pcp.app.protocol.ProtocolContentType
import chat.pcp.app.protocol.ProtocolType
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import org.json.JSONObject
import timber.log.Timber

class TelegramFeatureAdapter(
    private val session: TelegramSession
) {

    data class TelegramFeatureState(
        val recentStickers: List<TelegramSticker> = emptyList(),
        val installedStickerSets: List<TelegramStickerSet> = emptyList(),
        val activePolls: Map<Long, TelegramPoll> = emptyMap(),
        val messageReactions: Map<Long, Map<String, Int>> = emptyMap(),
        val inlineQueryResults: List<InlineQueryResult> = emptyList(),
        val chatFolders: List<ChatFolderConfig> = emptyList(),
        val scheduledMessages: Map<Long, List<IProtocolMessage>> = emptyMap(),
        val proxyList: List<TdProxyInfo> = emptyList(),
        val fileDownloadProgress: Map<Int, FileProgress> = emptyMap()
    ) {
        data class InlineQueryResult(
            val id: String,
            val title: String,
            val description: String,
            val thumbnailUrl: String?
        )
        data class TdProxyInfo(
            val id: Int,
            val server: String,
            val port: Int,
            val type: String,
            val isEnabled: Boolean,
            val lastPing: Long
        )
        data class FileProgress(
            val fileId: Int,
            val downloaded: Long,
            val total: Long,
            val isCompleted: Boolean
        )
    }

    private val _featureState = MutableStateFlow(TelegramFeatureState())
    val featureState: StateFlow<TelegramFeatureState> = _featureState

    fun loadRecentStickers() {
        session.stickerService.getRecentStickers()
    }

    fun loadInstalledStickerSets() {
        session.stickerService.getInstalledStickerSets()
    }

    fun searchStickers(emoji: String) {
        val result = session.stickerService.getStickers(emoji, 20)
        try {
            val json = JSONObject(result)
            val stickers = TelegramContentParser.parseStickerSet(json)
            _featureState.value = _featureState.value.copy(
                recentStickers = stickers.stickers
            )
        } catch (e: Exception) {
            Timber.e(e, "Failed to parse sticker search result")
        }
    }

    fun loadChatFolders() {
        session.folderService.getRecommendedChatFolders()
    }

    fun handleTdUpdate(type: String, json: JSONObject) {
        when (type) {
            "updatePoll" -> handlePollUpdate(json)
            "updateMessageReaction" -> handleReactionUpdate(json)
            "updateStickerSet" -> handleStickerSetUpdate(json)
            "updateFile" -> handleFileUpdate(json)
            "updateNewInlineQuery" -> handleInlineQuery(json)
            "updateChatFolders" -> handleFoldersUpdate(json)
            "updateProxy" -> handleProxyUpdate(json)
        }
    }

    private fun handlePollUpdate(json: JSONObject) {
        val poll = json.optJSONObject("poll") ?: return
        val pollId = poll.optLong("id", 0)
        val parsed = TelegramContentParser.parseMessage(
            JSONObject().apply {
                put("@type", "message")
                put("content", JSONObject().apply {
                    put("@type", "messagePoll")
                    put("poll", poll)
                })
            }
        )
        parsed.poll?.let {
            _featureState.value = _featureState.value.copy(
                activePolls = _featureState.value.activePolls + (pollId to it)
            )
        }
    }

    private fun handleReactionUpdate(json: JSONObject) {
        val messageId = json.optLong("message_id", 0)
        val reactions = json.optJSONArray("reactions")?.let { arr ->
            val map = mutableMapOf<String, Int>()
            for (i in 0 until arr.length()) {
                val r = arr.getJSONObject(i)
                val type = r.optJSONObject("type")?.let { typeObj ->
                    when (typeObj.optString("@type", "")) {
                        "reactionTypeEmoji" -> typeObj.optString("emoji", "")
                        "reactionTypeCustomEmoji" -> "custom:${typeObj.optLong("custom_emoji_id", 0)}"
                        else -> ""
                    }
                } ?: ""
                val count = r.optInt("total_count", 0)
                if (type.isNotEmpty()) map[type] = count
            }
            map
        }
        if (reactions != null) {
            _featureState.value = _featureState.value.copy(
                messageReactions = _featureState.value.messageReactions + (messageId to reactions)
            )
        }
    }

    private fun handleStickerSetUpdate(json: JSONObject) {
        val set = json.optJSONObject("sticker_set") ?: return
        val parsed = TelegramContentParser.parseStickerSet(set)
        _featureState.value = _featureState.value.copy(
            installedStickerSets = _featureState.value.installedStickerSets + parsed
        )
    }

    private fun handleFileUpdate(json: JSONObject) {
        val file = json.optJSONObject("file") ?: return
        val fileId = file.optInt("id", 0)
        val size = file.optLong("size", 0)
        val local = file.optJSONObject("local")
        val downloadedSize = local?.optLong("downloaded_size", 0) ?: 0
        val isComplete = local?.optBoolean("is_downloading_completed", false) ?: false

        _featureState.value = _featureState.value.copy(
            fileDownloadProgress = _featureState.value.fileDownloadProgress + (
                fileId to TelegramFeatureState.FileProgress(
                    fileId = fileId,
                    downloaded = downloadedSize,
                    total = size,
                    isCompleted = isComplete
                )
            )
        )
    }

    private fun handleInlineQuery(json: JSONObject) {
        val queryId = json.optLong("id", 0)
        val results = json.optJSONArray("results")?.let { arr ->
            (0 until arr.length()).map { i ->
                val r = arr.getJSONObject(i)
                TelegramFeatureState.InlineQueryResult(
                    id = r.optString("id", ""),
                    title = r.optString("title", ""),
                    description = r.optString("description", ""),
                    thumbnailUrl = r.optJSONObject("thumbnail")?.optJSONObject("file")?.optString("path")
                )
            }
        } ?: emptyList()
        _featureState.value = _featureState.value.copy(inlineQueryResults = results)
    }

    private fun handleFoldersUpdate(json: JSONObject) {
        val foldersJson = json.optJSONArray("chat_folders") ?: return
        val folders = (0 until foldersJson.length()).map { i ->
            val f = foldersJson.getJSONObject(i)
            ChatFolderConfig(
                title = f.optString("title", ""),
                iconName = f.optJSONObject("icon")?.optString("name", "Custom") ?: "Custom",
                pinnedChatIds = f.optJSONArray("pinned_chat_ids")?.let { arr ->
                    (0 until arr.length()).map { arr.getLong(it) }
                } ?: emptyList(),
                includedChatIds = f.optJSONArray("included_chat_ids")?.let { arr ->
                    (0 until arr.length()).map { arr.getLong(it) }
                } ?: emptyList(),
                excludedChatIds = f.optJSONArray("excluded_chat_ids")?.let { arr ->
                    (0 until arr.length()).map { arr.getLong(it) }
                } ?: emptyList(),
                excludeMuted = f.optBoolean("exclude_muted", false),
                excludeRead = f.optBoolean("exclude_read", false),
                excludeArchived = f.optBoolean("exclude_archived", true),
                includeContacts = f.optBoolean("include_contacts", true),
                includeNonContacts = f.optBoolean("include_non_contacts", true),
                includeBots = f.optBoolean("include_bots", true),
                includeGroups = f.optBoolean("include_groups", true),
                includeChannels = f.optBoolean("include_channels", true)
            )
        }
        _featureState.value = _featureState.value.copy(chatFolders = folders)
    }

    private fun handleProxyUpdate(json: JSONObject) {
        val proxies = json.optJSONArray("proxies")?.let { arr ->
            (0 until arr.length()).map { i ->
                val p = arr.getJSONObject(i)
                TelegramFeatureState.TdProxyInfo(
                    id = p.optInt("id", 0),
                    server = p.optString("server", ""),
                    port = p.optInt("port", 0),
                    type = p.optJSONObject("type")?.optString("@type", "") ?: "",
                    isEnabled = p.optBoolean("is_enabled", false),
                    lastPing = p.optLong("last_ping", 0)
                )
            }
        } ?: emptyList()
        _featureState.value = _featureState.value.copy(proxyList = proxies)
    }

    fun sendReaction(chatId: Long, messageId: Long, reaction: String, isBig: Boolean = false) {
        session.reactionService.setMessageReaction(chatId, messageId, reaction, isBig)
    }

    fun removeReaction(chatId: Long, messageId: Long, reaction: String) {
        session.reactionService.removeMessageReaction(chatId, messageId, reaction)
    }

    fun voteInPoll(chatId: Long, messageId: Long, optionIds: IntArray) {
        session.pollService.setPollVote(chatId, messageId, optionIds)
    }

    fun installStickerSet(setId: Long) {
        session.stickerService.changeStickerSet(setId, isInstalled = true, isArchived = false)
    }

    fun removeStickerSet(setId: Long) {
        session.stickerService.changeStickerSet(setId, isInstalled = false, isArchived = true)
    }
}
