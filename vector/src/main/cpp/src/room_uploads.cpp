#include "progressive/room_uploads.hpp"

namespace progressive {

// ==== Sticker Detection ====
//
// Original Kotlin (GetUploadsTask.kt:72):
//   .filter { it.getClearType() != EventType.STICKER }
//   EventType.STICKER = "m.sticker"

bool isStickerEvent(const std::string& eventType) {
    return eventType == "m.sticker";
}

// ==== Attachment URL Detection ====
//
// Original Kotlin (GetUploadsTask.kt:68):
//   .like(EventEntityFields.DECRYPTION_RESULT_JSON,
//         TimelineEventFilter.DecryptedContent.URL)
//
// For encrypted messages: the decrypted content JSON should contain
// a "url" field pointing to the MXC URI.

bool hasAttachmentUrl(const std::string& decryptedContentJson) {
    // Original Kotlin: checks if decryption result contains URL
    // Look for "url": "mxc://..." in the JSON
    auto pos = decryptedContentJson.find("\"url\"");
    if (pos == std::string::npos) return false;
    // Check that the URL is an mxc:// URI
    pos = decryptedContentJson.find("mxc://", pos);
    return pos != std::string::npos;
}

// ==== Event Extraction ====
//
// Original Kotlin (GetUploadsTask.kt:103-116):
//   val eventId = event.eventId
//   val messageContent = event.getClearContent()?.toModel<MessageContent>()
//   val messageWithAttachmentContent = (messageContent as? MessageWithAttachmentContent)
//   val senderInfo = cacheOfSenderInfos.getOrPut(senderId) { ... }

static std::string extractJsonField(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

UploadEvent extractUploadEvent(
    const std::string& eventJson,
    const std::string& senderName,
    const std::string& senderAvatarUrl,
    bool isUniqueDisplayName)
{
    UploadEvent event;

    // Original Kotlin: val eventId = event.eventId
    event.eventId = extractJsonField(eventJson, "event_id");
    event.senderId = extractJsonField(eventJson, "sender");

    // Original Kotlin: senderInfo.displayName, avatarUrl
    event.senderName = senderName;
    event.senderAvatarUrl = senderAvatarUrl;

    // Extract content fields
    auto contentPos = eventJson.find("\"content\"");
    if (contentPos != std::string::npos) {
        contentPos = eventJson.find('{', contentPos);
        if (contentPos != std::string::npos) {
            // Brace-count to extract content object
            int depth = 1;
            size_t start = contentPos;
            contentPos++;
            while (contentPos < eventJson.size() && depth > 0) {
                if (eventJson[contentPos] == '{') depth++;
                else if (eventJson[contentPos] == '}') depth--;
                contentPos++;
            }
            std::string contentJson = eventJson.substr(start, contentPos - start);

            // Check for "info" sub-object
            auto infoPos = contentJson.find("\"info\"");
            std::string infoJson;
            if (infoPos != std::string::npos) {
                infoPos = contentJson.find('{', infoPos);
                if (infoPos != std::string::npos) {
                    int d = 1;
                    size_t is = infoPos;
                    infoPos++;
                    while (infoPos < contentJson.size() && d > 0) {
                        if (contentJson[infoPos] == '{') d++;
                        else if (contentJson[infoPos] == '}') d--;
                        infoPos++;
                    }
                    infoJson = contentJson.substr(is, infoPos - is);
                }
            }

            // Original Kotlin: messageWithAttachmentContent fields
            event.mxcUrl = extractJsonField(contentJson, "url");
            event.fileName = extractJsonField(contentJson, "filename");
            event.mimeType = extractJsonField(contentJson, "mimetype");

            // From info sub-object
            if (!infoJson.empty()) {
                if (event.mimeType.empty()) event.mimeType = extractJsonField(infoJson, "mimetype");
                if (event.fileName.empty()) event.fileName = extractJsonField(infoJson, "filename");
                event.fileSize = extractJsonInt64(infoJson, "size");
            }

            // Size from body if not in info
            if (event.fileSize == 0) {
                event.fileSize = extractJsonInt64(contentJson, "size");
            }
        }
    }

    // Original Kotlin: origin_server_ts
    event.timestamp = extractJsonInt64(eventJson, "origin_server_ts");

    return event;
}

// ==== Uploads Filter ====
//
// Original Kotlin (FilterFactory.createUploadsFilter(numberOfEvents)):
//   Creates a RoomEventFilter with:
//   - types: ["m.room.message"]
//   - not_types: ["m.room.message.feedback"] (exclude reactions)
//   - limit: numberOfEvents
// The filter is used for the /messages API to only return attachment messages.

std::string createUploadsFilterJson(int numberOfEvents) {
    // Original Kotlin: creates a JSON filter for /messages
    // {
    //   "room": {
    //     "timeline": {
    //       "types": ["m.room.message"],
    //       "not_types": ["m.room.member", "m.room.create"]
    //     }
    //   }
    // }
    // Actually, the uploads filter is simpler:
    // {
    //   "room": {
    //     "timeline": {
    //       "limit": N,
    //       "types": ["m.room.message"],
    //       "not_types": ["m.room.member"]
    //     }
    //   }
    // }
    std::string json = "{";
    json += "\"room\":{";
    json += "\"timeline\":{";
    json += "\"limit\":" + std::to_string(numberOfEvents) + ",";
    json += "\"types\":[\"m.room.message\"],";
    json += "\"not_types\":[\"m.room.member\",\"m.sticker\"]";
    json += "}}}";
    return json;
}

// ==== Serialization ====

std::string getUploadsResultToJson(const GetUploadsResult& result) {
    // Original Kotlin: Kotlin serialization of GetUploadsResult
    std::string json = "{";
    json += "\"uploadEvents\":[";
    bool first = true;
    for (const auto& ev : result.uploadEvents) {
        if (!first) json += ",";
        first = false;
        json += "{";
        json += "\"eventId\":\"" + ev.eventId + "\",";
        json += "\"senderName\":\"" + ev.senderName + "\",";
        json += "\"mxcUrl\":\"" + ev.mxcUrl + "\",";
        json += "\"fileName\":\"" + ev.fileName + "\",";
        json += "\"mimeType\":\"" + ev.mimeType + "\",";
        json += "\"fileSize\":" + std::to_string(ev.fileSize) + ",";
        json += "\"timestamp\":" + std::to_string(ev.timestamp);
        json += "}";
    }
    json += "],";
    json += "\"nextToken\":\"" + result.nextToken + "\",";
    json += "\"hasMore\":" + std::string(result.hasMore ? "true" : "false");
    json += "}";
    return json;
}

} // namespace progressive
