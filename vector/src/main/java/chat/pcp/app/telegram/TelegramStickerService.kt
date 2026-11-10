package chat.pcp.app.telegram

import org.json.JSONArray
import org.json.JSONObject

class TelegramStickerService(private val client: TdLibClient) {

    fun getStickerSet(setId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getStickerSet")
            put("set_id", setId)
        })
    }

    fun searchStickerSet(name: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchStickerSet")
            put("name", name)
        })
    }

    fun getInstalledStickerSets(isMasks: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getInstalledStickerSets")
            put("sticker_type",
                if (isMasks) JSONObject().apply { put("@type", "stickerTypeMask") }
                else JSONObject().apply { put("@type", "stickerTypeRegular") }
            )
        })
    }

    fun getTrendingStickerSets() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getTrendingStickerSets")
            put("sticker_type", JSONObject().apply { put("@type", "stickerTypeRegular") })
            put("offset", 0)
            put("limit", 20)
        })
    }

    fun changeStickerSet(setId: Long, isInstalled: Boolean, isArchived: Boolean) {
        client.sendRequest(JSONObject().apply {
            put("@type", "changeStickerSet")
            put("set_id", setId)
            put("is_installed", isInstalled)
            put("is_archived", isArchived)
        })
    }

    fun viewTrendingStickerSets(stickerSetIds: LongArray) {
        client.sendRequest(JSONObject().apply {
            put("@type", "viewTrendingStickerSets")
            put("sticker_set_ids", JSONArray(stickerSetIds.toList()))
        })
    }

    fun getRecentStickers(isAttached: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getRecentStickers")
            put("is_attached", isAttached)
        })
    }

    fun getFavoriteStickers() {
        client.sendRequest(JSONObject().apply {
            put("@type", "getFavoriteStickers")
        })
    }

    fun addFavoriteSticker(stickerInputFileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "addFavoriteSticker")
            put("sticker", JSONObject().apply {
                put("@type", "inputFileId")
                put("id", stickerInputFileId)
            })
        })
    }

    fun removeFavoriteSticker(stickerInputFileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "removeFavoriteSticker")
            put("sticker", JSONObject().apply {
                put("@type", "inputFileId")
                put("id", stickerInputFileId)
            })
        })
    }

    fun searchStickers(emoji: String, limit: Int = 20) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchStickers")
            put("emoji", emoji)
            put("limit", limit)
        })
    }

    fun searchInstalledStickerSets(query: String, limit: Int = 10) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchInstalledStickerSets")
            put("sticker_type", JSONObject().apply { put("@type", "stickerTypeRegular") })
            put("query", query)
            put("limit", limit)
        })
    }

    fun getStickers(emoji: String, limit: Int = 20): String {
        val response = client.execute(JSONObject().apply {
            put("@type", "getStickers")
            put("emoji", emoji)
            put("limit", limit)
        })
        return response
    }

    fun removeRecentSticker(isAttached: Boolean, stickerInputFileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "removeRecentSticker")
            put("is_attached", isAttached)
            put("sticker", JSONObject().apply {
                put("@type", "inputFileId")
                put("id", stickerInputFileId)
            })
        })
    }

    fun clearRecentStickers(isAttached: Boolean = false) {
        client.sendRequest(JSONObject().apply {
            put("@type", "clearRecentStickers")
            put("is_attached", isAttached)
        })
    }
}
