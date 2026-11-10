#include "pcp/message_content_utils.hpp"
#include <sstream>
#include <regex>

namespace pcp {

// Helper: extract string value from JSON by key
static std::string jsonGet(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    // Skip whitespace
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return "";
    if (json[pos] == '"') {
        pos++;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    }
    if (json[pos] == '-' || std::isdigit(json[pos])) {
        auto end = pos;
        while (end < json.size() && (std::isdigit(json[end]) || json[end] == '-' || json[end] == '.')) end++;
        return json.substr(pos, end - pos);
    }
    return "";
}

// Helper: extract int value from JSON by key
static int jsonGetInt(const std::string& json, const std::string& key) {
    std::string val = jsonGet(json, key);
    if (val.empty()) return 0;
    try { return std::stoi(val); } catch (...) { return 0; }
}

// Helper: extract int64 value from JSON by key
static int64_t jsonGetInt64(const std::string& json, const std::string& key) {
    std::string val = jsonGet(json, key);
    if (val.empty()) return 0;
    try { return std::stoll(val); } catch (...) { return 0; }
}

// Parse message content from event JSON
// Original Kotlin: Content.toModel<MessageContent>() — extracts msgtype, body, formatted body, URL, etc.
MessageContentInfo parseMessageContent(const std::string& json) {
    MessageContentInfo info;

    // Check if we have a content sub-object
    std::string contentJson = json;
    auto contentPos = json.find("\"content\"");
    if (contentPos != std::string::npos) {
        auto brace = json.find('{', contentPos);
        if (brace != std::string::npos) {
            // Find matching closing brace
            int depth = 0;
            auto end = brace;
            while (end < json.size()) {
                if (json[end] == '{') depth++;
                else if (json[end] == '}') { depth--; if (depth == 0) break; }
                end++;
            }
            if (depth == 0) contentJson = json.substr(brace, end - brace + 1);
        }
    }

    info.msgType = jsonGet(contentJson, "msgtype");
    info.body = jsonGet(contentJson, "body");
    info.formattedBody = jsonGet(contentJson, "formatted_body");
    info.format = jsonGet(contentJson, "format");
    info.url = jsonGet(contentJson, "url");
    info.filename = jsonGet(contentJson, "filename");
    info.geoUri = jsonGet(contentJson, "geo_uri");

    // Extract info sub-object
    auto infoPos = contentJson.find("\"info\"");
    if (infoPos != std::string::npos) {
        auto brace = contentJson.find('{', infoPos);
        if (brace != std::string::npos) {
            int depth = 0;
            auto end = brace;
            while (end < contentJson.size()) {
                if (contentJson[end] == '{') depth++;
                else if (contentJson[end] == '}') { depth--; if (depth == 0) break; }
                end++;
            }
            if (depth == 0) {
                std::string infoBlock = contentJson.substr(brace, end - brace + 1);
                info.infoMimeType = jsonGet(infoBlock, "mimetype");
                info.infoWidth = jsonGetInt(infoBlock, "w");
                info.infoHeight = jsonGetInt(infoBlock, "h");
                info.infoSize = jsonGetInt(infoBlock, "size");
                info.infoDuration = jsonGetInt(infoBlock, "duration");
                info.thumbnailUrl = jsonGet(infoBlock, "thumbnail_url");
            }
        }
    }

    // Extract relation info
    auto relPos = contentJson.find("\"m.relates_to\"");
    if (relPos != std::string::npos) {
        info.relatesToEventId = jsonGet(contentJson, "event_id");
        info.relatesToType = jsonGet(contentJson, "rel_type");
    }

    info.valid = !info.body.empty() || !info.msgType.empty();
    return info;
}

std::string extractMsgType(const std::string& json) {
    return jsonGet(json, "msgtype");
}

bool isMediaMessage(const std::string& msgType) {
    // Original Kotlin: when(msgType) { MSGTYPE_IMAGE, MSGTYPE_AUDIO, MSGTYPE_VIDEO, MSGTYPE_FILE -> true }
    return msgType == MessageType::IMAGE || msgType == MessageType::AUDIO ||
           msgType == MessageType::VIDEO || msgType == MessageType::FILE ||
           msgType == MessageType::LOCATION || msgType == MessageType::STICKER;
}

bool isDownloadableMessage(const std::string& msgType) {
    // Messages with MXC URL that can be downloaded
    return msgType == MessageType::IMAGE || msgType == MessageType::AUDIO ||
           msgType == MessageType::VIDEO || msgType == MessageType::FILE;
}

std::string getMessageTypeLabel(const std::string& msgType) {
    if (msgType == MessageType::TEXT) return "text";
    if (msgType == MessageType::EMOTE) return "emote";
    if (msgType == MessageType::NOTICE) return "notice";
    if (msgType == MessageType::IMAGE) return "image";
    if (msgType == MessageType::AUDIO) return "audio";
    if (msgType == MessageType::VIDEO) return "video";
    if (msgType == MessageType::FILE) return "file";
    if (msgType == MessageType::LOCATION) return "location";
    if (msgType == MessageType::STICKER) return "sticker";
    if (msgType == MessageType::POLL_START) return "poll";
    if (msgType == MessageType::VERIFICATION_REQUEST) return "verification";
    if (msgType == MessageType::BEACON_INFO) return "location";
    if (msgType == MessageType::VOICE_BROADCAST) return "voice_broadcast";
    return "unknown";
}

bool isDisplayableEvent(const std::string& eventType) {
    // Original Kotlin: Events that can be displayed in the timeline
    if (eventType == EventType::ROOM_MESSAGE) return true;
    if (eventType == EventType::ROOM_MEMBER) return true;
    if (eventType == EventType::ROOM_NAME) return true;
    if (eventType == EventType::ROOM_TOPIC) return true;
    if (eventType == EventType::ROOM_AVATAR) return true;
    if (eventType == EventType::ROOM_CREATE) return true;
    if (eventType == EventType::ROOM_TOMBSTONE) return true;
    if (eventType == EventType::ROOM_JOIN_RULES) return true;
    if (eventType == EventType::ROOM_POWER_LEVELS) return true;
    if (eventType == EventType::ROOM_ENCRYPTION) return true;
    if (eventType == EventType::ROOM_CANONICAL_ALIAS) return true;
    if (eventType == EventType::STICKER) return true;
    if (eventType == EventType::REACTION) return true;
    if (eventType == EventType::CALL_INVITE) return true;
    if (eventType == EventType::CALL_ANSWER) return true;
    if (eventType == EventType::CALL_HANGUP) return true;
    if (eventType == EventType::POLL_START) return true;
    if (eventType == EventType::POLL_END) return true;
    if (eventType == EventType::VOICE_BROADCAST_START) return true;
    if (eventType == EventType::BEACON_INFO) return true;
    if (eventType == EventType::BEACON_LOCATION) return true;
    return false;
}

std::string extractEventId(const std::string& json) {
    return jsonGet(json, "event_id");
}

std::string extractRoomId(const std::string& json) {
    return jsonGet(json, "room_id");
}

std::string extractSenderId(const std::string& json) {
    return jsonGet(json, "sender");
}

int64_t extractTimestamp(const std::string& json) {
    return jsonGetInt64(json, "origin_server_ts");
}

std::string buildMessageEvent(const std::string& roomId, const std::string& body, const std::string& msgType) {
    std::ostringstream oss;
    oss << R"({"type":"m.room.message","room_id":")" << roomId
        << R"(","content":{"msgtype":")" << msgType
        << R"(","body":")";
    // Basic JSON escape for body
    for (char c : body) {
        if (c == '"') oss << "\\\"";
        else if (c == '\\') oss << "\\\\";
        else if (c == '\n') oss << "\\n";
        else oss << c;
    }
    oss << R"("}})";
    return oss.str();
}

} // namespace pcp
