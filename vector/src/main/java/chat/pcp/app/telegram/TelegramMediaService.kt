package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramMediaService(private val client: TdLibClient) {

    val fileService = TelegramFileService(client)

    fun sendPhoto(chatId: Long, filePath: String, caption: String? = null, hasSpoiler: Boolean = false, ttl: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessagePhoto")
                put("photo", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                if (caption != null) {
                    put("caption", JSONObject().apply {
                        put("@type", "formattedText")
                        put("text", caption)
                    })
                }
                if (hasSpoiler) put("has_spoiler", true)
                if (ttl > 0) put("ttl", ttl)
            })
        })
    }

    fun sendVideo(chatId: Long, filePath: String, caption: String? = null, duration: Int = 0,
                  width: Int = 0, height: Int = 0, supportsStreaming: Boolean = true, ttl: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageVideo")
                put("video", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                if (caption != null) {
                    put("caption", JSONObject().apply {
                        put("@type", "formattedText")
                        put("text", caption)
                    })
                }
                if (duration > 0) put("duration", duration)
                if (width > 0) put("width", width)
                if (height > 0) put("height", height)
                put("supports_streaming", supportsStreaming)
                if (ttl > 0) put("ttl", ttl)
            })
        })
    }

    fun sendVoiceNote(chatId: Long, filePath: String, duration: Int, waveform: ByteArray? = null) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageVoiceNote")
                put("voice_note", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                put("duration", duration)
            })
        })
    }

    fun sendVideoNote(chatId: Long, filePath: String, duration: Int, length: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageVideoNote")
                put("video_note", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                put("duration", duration)
                if (length > 0) put("length", length)
            })
        })
    }

    fun sendAnimation(chatId: Long, filePath: String, caption: String? = null, duration: Int = 0,
                      width: Int = 0, height: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageAnimation")
                put("animation", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                if (caption != null) {
                    put("caption", JSONObject().apply {
                        put("@type", "formattedText")
                        put("text", caption)
                    })
                }
                if (duration > 0) put("duration", duration)
                if (width > 0) put("width", width)
                if (height > 0) put("height", height)
            })
        })
    }

    fun sendSticker(chatId: Long, stickerInputFileId: Int) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageSticker")
                put("sticker", JSONObject().apply {
                    put("@type", "inputFileId")
                    put("id", stickerInputFileId)
                })
            })
        })
    }

    fun sendDocument(chatId: Long, filePath: String, fileName: String? = null, caption: String? = null) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageDocument")
                put("document", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", filePath)
                })
                if (caption != null) {
                    put("caption", JSONObject().apply {
                        put("@type", "formattedText")
                        put("text", caption)
                    })
                }
            })
        })
    }

    fun sendDice(chatId: Long, emoji: String = "\uD83C\uDFB2") {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageDice")
                put("emoji", emoji)
            })
        })
    }

    fun sendContact(chatId: Long, contact: JSONObject) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageContact")
                put("contact", contact)
            })
        })
    }

    fun sendLocation(chatId: Long, latitude: Double, longitude: Double,
                     horizontalAccuracy: Double = 0.0, livePeriod: Int = 0,
                     heading: Int = 0, proximityAlertRadius: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageLocation")
                put("location", JSONObject().apply {
                    put("@type", "location")
                    put("latitude", latitude)
                    put("longitude", longitude)
                    if (horizontalAccuracy > 0) put("horizontal_accuracy", horizontalAccuracy)
                })
                put("live_period", livePeriod)
                put("heading", heading)
                put("proximity_alert_radius", proximityAlertRadius)
            })
        })
    }

    fun sendMessageAlbum(chatId: Long, albumItems: List<JSONObject>) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessageAlbum")
            put("chat_id", chatId)
            put("input_message_contents", org.json.JSONArray(albumItems))
        })
    }

    fun generateChatPhoto(chatId: Long, prompt: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "generateChatPhoto")
            put("chat_id", chatId)
            put("prompt", prompt)
        })
    }

    fun setChatPhoto(chatId: Long, photoPath: String) {
        client.sendRequest(JSONObject().apply {
            put("@type", "setChatPhoto")
            put("chat_id", chatId)
            put("photo", JSONObject().apply {
                put("@type", "inputChatPhotoStatic")
                put("photo", JSONObject().apply {
                    put("@type", "inputFileLocal")
                    put("path", photoPath)
                })
            })
        })
    }
}
