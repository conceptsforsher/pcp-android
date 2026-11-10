package chat.pcp.app.telegram

import org.json.JSONObject

class TelegramLocationService(private val client: TdLibClient) {

    fun sendLiveLocation(
        chatId: Long,
        latitude: Double,
        longitude: Double,
        livePeriod: Int = 3600,
        heading: Int = 0,
        proximityAlertRadius: Int = 0
    ) {
        client.sendRequest(JSONObject().apply {
            put("@type", "sendMessage")
            put("chat_id", chatId)
            put("input_message_content", JSONObject().apply {
                put("@type", "inputMessageLocation")
                put("location", JSONObject().apply {
                    put("@type", "location")
                    put("latitude", latitude)
                    put("longitude", longitude)
                })
                put("live_period", livePeriod)
                put("heading", heading)
                put("proximity_alert_radius", proximityAlertRadius)
            })
        })
    }

    fun editLiveLocation(chatId: Long, messageId: Long, latitude: Double, longitude: Double,
                         heading: Int = 0, proximityAlertRadius: Int = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "editMessageLiveLocation")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("location", JSONObject().apply {
                put("@type", "location")
                put("latitude", latitude)
                put("longitude", longitude)
            })
            put("heading", heading)
            put("proximity_alert_radius", proximityAlertRadius)
        })
    }

    fun stopLiveLocation(chatId: Long, messageId: Long) {
        client.sendRequest(JSONObject().apply {
            put("@type", "editMessageLiveLocation")
            put("chat_id", chatId)
            put("message_id", messageId)
            put("location", JSONObject().apply {
                put("@type", "location")
                put("latitude", 0.0)
                put("longitude", 0.0)
            })
        })
    }

    fun getMapThumbnailFile(latitude: Double, longitude: Double, width: Int = 400, height: Int = 300,
                            zoom: Int = 15, scale: Int = 2, chatId: Long = 0) {
        client.sendRequest(JSONObject().apply {
            put("@type", "getMapThumbnailFile")
            put("location", JSONObject().apply {
                put("@type", "location")
                put("latitude", latitude)
                put("longitude", longitude)
            })
            put("zoom", zoom)
            put("width", width)
            put("height", height)
            put("scale", scale)
            put("chat_id", chatId)
        })
    }

    fun searchNearbyVenues(latitude: Double, longitude: Double, query: String = "",
                           limit: Int = 20, radius: Int = 1000) {
        client.sendRequest(JSONObject().apply {
            put("@type", "searchVenuesNearby")
            put("location", JSONObject().apply {
                put("@type", "location")
                put("latitude", latitude)
                put("longitude", longitude)
            })
            put("query", query)
            put("limit", limit)
            put("radius", radius)
        })
    }
}
