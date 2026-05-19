#include "progressive/message_content.hpp"
#include <cstring>

namespace progressive {

// ==== JSON Extraction Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
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

static int extractJsonInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int val = 0;
    bool negative = false;
    if (json[pos] == '-') { negative = true; pos++; }
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return negative ? -val : val;
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

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// ==== Parse Info Types ====

// Original Kotlin (ThumbnailInfo.kt:24-40)
static ThumbnailInfo parseThumbnailInfo(const std::string& json) {
    ThumbnailInfo ti;
    ti.width = extractJsonInt(json, "w");
    ti.height = extractJsonInt(json, "h");
    ti.size = extractJsonInt64(json, "size");
    ti.mimeType = extractJsonString(json, "mimetype");
    return ti;
}

// Original Kotlin (ImageInfo.kt:25-55)
static ImageInfo parseImageInfo(const std::string& json) {
    ImageInfo info;
    info.mimeType = extractJsonString(json, "mimetype");
    info.width = extractJsonInt(json, "w");
    info.height = extractJsonInt(json, "h");
    info.size = extractJsonInt64(json, "size");
    auto tnJson = extractJsonObject(json, "thumbnail_info");
    if (!tnJson.empty()) info.thumbnailInfo = parseThumbnailInfo(tnJson);
    info.thumbnailUrl = extractJsonString(json, "thumbnail_url");
    // thumbnail_file handled separately if needed
    return info;
}

// Original Kotlin (VideoInfo.kt:25-56)
static VideoInfo parseVideoInfo(const std::string& json) {
    VideoInfo info;
    info.mimeType = extractJsonString(json, "mimetype");
    info.width = extractJsonInt(json, "w");
    info.height = extractJsonInt(json, "h");
    info.size = extractJsonInt64(json, "size");
    info.duration = extractJsonInt(json, "duration");
    auto tnJson = extractJsonObject(json, "thumbnail_info");
    if (!tnJson.empty()) info.thumbnailInfo = parseThumbnailInfo(tnJson);
    info.thumbnailUrl = extractJsonString(json, "thumbnail_url");
    return info;
}

// Original Kotlin (AudioInfo.kt:23-36)
static AudioInfo parseAudioInfo(const std::string& json) {
    AudioInfo info;
    info.mimeType = extractJsonString(json, "mimetype");
    info.size = extractJsonInt64(json, "size");
    info.duration = extractJsonInt(json, "duration");
    return info;
}

// Original Kotlin (FileInfo.kt:26-46)
static FileInfo parseFileInfo(const std::string& json) {
    FileInfo info;
    info.mimeType = extractJsonString(json, "mimetype");
    info.size = extractJsonInt64(json, "size");
    auto tnJson = extractJsonObject(json, "thumbnail_info");
    if (!tnJson.empty()) info.thumbnailInfo = parseThumbnailInfo(tnJson);
    info.thumbnailUrl = extractJsonString(json, "thumbnail_url");
    return info;
}

// ==== Parse Relation ====

static RelationDefaultContent parseRelation(const std::string& contentJson) {
    RelationDefaultContent rel;
    auto relJson = extractJsonObject(contentJson, "m.relates_to");
    if (relJson.empty()) return rel;
    rel.eventId = extractJsonString(relJson, "event_id");
    rel.relationType = extractJsonString(relJson, "rel_type");
    rel.key = extractJsonString(relJson, "key");
    return rel;
}

// ==== Message Type Detection ====
//
// Original Kotlin (MessageType.kt:22-58):
//   object MessageType {
//       const val MSGTYPE_TEXT = "m.text", MSGTYPE_EMOTE = "m.emote", etc.
//   }

ParsedMessageType msgTypeFromString(const std::string& msgtype) {
    if (msgtype == "m.text") return ParsedMessageType::TEXT;
    if (msgtype == "m.notice") return ParsedMessageType::NOTICE;
    if (msgtype == "m.emote") return ParsedMessageType::EMOTE;
    if (msgtype == "m.image") return ParsedMessageType::IMAGE;
    if (msgtype == "m.video") return ParsedMessageType::VIDEO;
    if (msgtype == "m.audio") return ParsedMessageType::AUDIO;
    if (msgtype == "m.file") return ParsedMessageType::FILE;
    if (msgtype == "m.location") return ParsedMessageType::LOCATION;
    // Original Kotlin: local fake types
    if (msgtype == "org.matrix.android.sdk.sticker") return ParsedMessageType::STICKER;
    if (msgtype == "org.matrix.android.sdk.poll.start") return ParsedMessageType::POLL_START;
    if (msgtype == "org.matrix.android.sdk.poll.response") return ParsedMessageType::POLL_RESPONSE;
    if (msgtype == "org.matrix.android.sdk.poll.end") return ParsedMessageType::POLL_END;
    return ParsedMessageType::UNKNOWN;
}

// ==== Main Message Parser ====
//
// Original Kotlin: parse the "content" JSON blob from an event.
// Each message type is parsed by Moshi from the same JSON structure,
// differentiated by the "msgtype" field.

void fillMessageContent(EventMessageContent& mc, const std::string& json) {
    mc.msgType = extractJsonString(json, "msgtype");
    mc.body = extractJsonString(json, "body");
    mc.relatesTo = parseRelation(json);
    mc.hasRelation = !mc.relatesTo.eventId.empty();
    mc.newContent = extractJsonObject(json, "m.new_content");
    mc.isEdit = !mc.newContent.empty();
}

void fillFormattedBody(MessageContentWithFormattedBody& fmt, const std::string& json) {
    fillMessageContent(fmt, json);
    fmt.format = extractJsonString(json, "format");
    fmt.formattedBody = extractJsonString(json, "formatted_body");
}

ParsedMessage parseMessageContent(const std::string& contentJson) {
    ParsedMessage result;
    result.rawJson = contentJson;

    auto msgtype = extractJsonString(contentJson, "msgtype");
    result.type = msgTypeFromString(msgtype);

    switch (result.type) {
        // Original Kotlin (MessageTextContent.kt:26-43)
        case ParsedMessageType::TEXT:
            fillFormattedBody(result.text, contentJson);
            break;
        // Original Kotlin (MessageNoticeContent.kt:26-43)
        case ParsedMessageType::NOTICE:
            fillFormattedBody(result.notice, contentJson);
            break;
        // Original Kotlin (MessageEmoteContent.kt:26-43)
        case ParsedMessageType::EMOTE:
            fillFormattedBody(result.emote, contentJson);
            break;
        // Original Kotlin (MessageImageContent.kt:28-55)
        case ParsedMessageType::IMAGE:
            fillMessageContent(result.image, contentJson);
            result.image.url = extractJsonString(contentJson, "url");
            {
                auto infoJson = extractJsonObject(contentJson, "info");
                if (!infoJson.empty()) result.image.info = parseImageInfo(infoJson);
                result.image.mimeType = result.image.info.mimeType;
            }
            break;
        // Original Kotlin (MessageVideoContent.kt:28-56)
        case ParsedMessageType::VIDEO:
            fillMessageContent(result.video, contentJson);
            result.video.url = extractJsonString(contentJson, "url");
            {
                auto infoJson = extractJsonObject(contentJson, "info");
                if (!infoJson.empty()) result.video.videoInfo = parseVideoInfo(infoJson);
                result.video.mimeType = result.video.videoInfo.mimeType;
            }
            break;
        // Original Kotlin (MessageAudioContent.kt:28-63)
        case ParsedMessageType::AUDIO:
            fillMessageContent(result.audio, contentJson);
            result.audio.url = extractJsonString(contentJson, "url");
            {
                auto infoJson = extractJsonObject(contentJson, "info");
                if (!infoJson.empty()) result.audio.audioInfo = parseAudioInfo(infoJson);
                result.audio.mimeType = result.audio.audioInfo.mimeType;
            }
            result.audio.isVoiceMessage = contentJson.find("\"org.matrix.msc3245.voice\"") != std::string::npos;
            break;
        // Original Kotlin (MessageFileContent.kt:28-57)
        case ParsedMessageType::FILE:
            fillMessageContent(result.file, contentJson);
            result.file.filename = extractJsonString(contentJson, "filename");
            result.file.url = extractJsonString(contentJson, "url");
            {
                auto infoJson = extractJsonObject(contentJson, "info");
                if (!infoJson.empty()) result.file.info = parseFileInfo(infoJson);
                result.file.mimeType = result.file.info.mimeType;
            }
            break;
        // Original Kotlin (MessageLocationContent)
        case ParsedMessageType::LOCATION:
            fillMessageContent(result.location, contentJson);
            result.location.geoUri = extractJsonString(contentJson, "geo_uri");
            result.location.mxcUrl = extractJsonString(contentJson, "url");
            break;
        default:
            break;
    }

    return result;
}

// ==== JSON Serialization ====

static std::string escapeJson(const std::string& s) {
    std::string r = "\"";
    for (char c : s) {
        if (c == '"') r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c == '\t') r += "\\t";
        else r += c;
    }
    r += "\"";
    return r;
}

// Original Kotlin (MessageTextContent.kt:26-43), Moshi-generated JSON
std::string messageTextToJson(const MessageTextContent& msg) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJson(msg.msgType) + ",";
    json += "\"body\":" + escapeJson(msg.body);
    // Original Kotlin: format / formatted_body
    if (!msg.format.empty()) {
        json += ",\"format\":" + escapeJson(msg.format);
        if (!msg.formattedBody.empty()) {
            json += ",\"formatted_body\":" + escapeJson(msg.formattedBody);
        }
    }
    // Original Kotlin: relates_to
    if (msg.hasRelation) {
        json += ",\"m.relates_to\":{\"event_id\":" + escapeJson(msg.relatesTo.eventId);
        if (!msg.relatesTo.relationType.empty())
            json += ",\"rel_type\":" + escapeJson(msg.relatesTo.relationType);
        json += "}";
    }
    if (!msg.newContent.empty()) {
        json += ",\"m.new_content\":" + msg.newContent;
    }
    json += "}";
    return json;
}

std::string messageNoticeToJson(const MessageNoticeContent& msg) {
    auto base = reinterpret_cast<const MessageContentWithFormattedBody*>(&msg);
    return messageTextToJson(reinterpret_cast<const MessageTextContent&>(*base));
}

std::string messageEmoteToJson(const MessageEmoteContent& msg) {
    auto base = reinterpret_cast<const MessageContentWithFormattedBody*>(&msg);
    return messageTextToJson(reinterpret_cast<const MessageTextContent&>(*base));
}

std::string messageImageToJson(const MessageImageContent& msg) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJson(msg.msgType) + ",";
    json += "\"body\":" + escapeJson(msg.body);
    if (!msg.url.empty()) json += ",\"url\":" + escapeJson(msg.url);
    // info
    if (msg.info.width > 0 || msg.info.height > 0 || msg.info.size > 0) {
        json += ",\"info\":{";
        if (!msg.info.mimeType.empty()) json += "\"mimetype\":" + escapeJson(msg.info.mimeType) + ",";
        json += "\"w\":" + std::to_string(msg.info.width) + ",";
        json += "\"h\":" + std::to_string(msg.info.height) + ",";
        json += "\"size\":" + std::to_string(msg.info.size);
        if (msg.info.thumbnailInfo.hasData()) {
            json += ",\"thumbnail_info\":{\"w\":" + std::to_string(msg.info.thumbnailInfo.width) +
                    ",\"h\":" + std::to_string(msg.info.thumbnailInfo.height) +
                    ",\"size\":" + std::to_string(msg.info.thumbnailInfo.size) + "}";
        }
        json += "}";
    }
    if (msg.hasRelation) {
        json += ",\"m.relates_to\":{\"event_id\":" + escapeJson(msg.relatesTo.eventId) + "}";
    }
    json += "}";
    return json;
}

std::string messageVideoToJson(const MessageVideoContent& msg) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJson(msg.msgType) + ",";
    json += "\"body\":" + escapeJson(msg.body);
    if (!msg.url.empty()) json += ",\"url\":" + escapeJson(msg.url);
    // info (videoInfo)
    json += ",\"info\":{";
    if (!msg.videoInfo.mimeType.empty()) json += "\"mimetype\":" + escapeJson(msg.videoInfo.mimeType) + ",";
    json += "\"w\":" + std::to_string(msg.videoInfo.width) + ",";
    json += "\"h\":" + std::to_string(msg.videoInfo.height) + ",";
    json += "\"size\":" + std::to_string(msg.videoInfo.size) + ",";
    json += "\"duration\":" + std::to_string(msg.videoInfo.duration);
    json += "}";
    if (msg.hasRelation) {
        json += ",\"m.relates_to\":{\"event_id\":" + escapeJson(msg.relatesTo.eventId) + "}";
    }
    json += "}";
    return json;
}

std::string messageAudioToJson(const MessageAudioContent& msg) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJson(msg.msgType) + ",";
    json += "\"body\":" + escapeJson(msg.body);
    if (!msg.url.empty()) json += ",\"url\":" + escapeJson(msg.url);
    // Original Kotlin: voice message indicator
    if (msg.isVoiceMessage) {
        json += ",\"org.matrix.msc3245.voice\":{}";
    }
    json += "}";
    return json;
}

std::string messageFileToJson(const MessageFileContent& msg) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJson(msg.msgType) + ",";
    json += "\"body\":" + escapeJson(msg.body);
    if (!msg.filename.empty()) json += ",\"filename\":" + escapeJson(msg.filename);
    if (!msg.url.empty()) json += ",\"url\":" + escapeJson(msg.url);
    if (msg.info.size > 0 || !msg.info.mimeType.empty()) {
        json += ",\"info\":{";
        if (!msg.info.mimeType.empty()) json += "\"mimetype\":" + escapeJson(msg.info.mimeType) + ",";
        json += "\"size\":" + std::to_string(msg.info.size);
        json += "}";
    }
    json += "}";
    return json;
}

// ================================================================
// MessageContentTypeDetector Implementation
// ================================================================

// Original Kotlin: detectContentType()
ContentType MessageContentTypeDetector::detectContentType(const std::string& contentJson) {
    auto msgtype = extractJsonString(contentJson, "msgtype");
    if (!msgtype.empty()) {
        if (msgtype == "m.text") return ContentType::TEXT;
        if (msgtype == "m.notice") return ContentType::NOTICE;
        if (msgtype == "m.emote") return ContentType::EMOTE;
        if (msgtype == "m.image") return ContentType::IMAGE;
        if (msgtype == "m.video") return ContentType::VIDEO;
        if (msgtype == "m.audio") return ContentType::AUDIO;
        if (msgtype == "m.file") return ContentType::FILE;
        if (msgtype == "m.location") return ContentType::LOCATION;
        if (msgtype == "org.matrix.android.sdk.sticker") return ContentType::STICKER;
        if (msgtype == "org.matrix.android.sdk.poll.start") return ContentType::POLL_START;
        if (msgtype == "org.matrix.android.sdk.poll.response") return ContentType::POLL_RESPONSE;
        if (msgtype == "org.matrix.android.sdk.poll.end") return ContentType::POLL_END;
    }

    // Check for beacon events
    if (contentJson.find("org.matrix.msc3488.beacon_info") != std::string::npos)
        return ContentType::BEACON_INFO;
    if (contentJson.find("org.matrix.msc3488.beacon_location") != std::string::npos ||
        contentJson.find("org.matrix.msc3672.beacon") != std::string::npos)
        return ContentType::BEACON_LOCATION_DATA;

    // Check for key verification events
    if (contentJson.find("m.key.verification.request") != std::string::npos)
        return ContentType::VERIFICATION_REQUEST;
    if (contentJson.find("m.key.verification.start") != std::string::npos)
        return ContentType::VERIFICATION_START;
    if (contentJson.find("m.key.verification.done") != std::string::npos)
        return ContentType::VERIFICATION_DONE;
    if (contentJson.find("m.key.verification.cancel") != std::string::npos)
        return ContentType::VERIFICATION_CANCEL;
    if (contentJson.find("m.key.verification.ready") != std::string::npos)
        return ContentType::VERIFICATION_READY;
    if (contentJson.find("m.key.verification.key") != std::string::npos)
        return ContentType::VERIFICATION_KEY;
    if (contentJson.find("m.key.verification.mac") != std::string::npos)
        return ContentType::VERIFICATION_MAC;

    // Check for call events
    if (contentJson.find("m.call.invite") != std::string::npos)
        return ContentType::CALL_INVITE;
    if (contentJson.find("m.call.answer") != std::string::npos)
        return ContentType::CALL_ANSWER;
    if (contentJson.find("m.call.hangup") != std::string::npos)
        return ContentType::CALL_HANGUP;
    if (contentJson.find("m.call.reject") != std::string::npos)
        return ContentType::CALL_REJECT;
    if (contentJson.find("m.call.candidates") != std::string::npos)
        return ContentType::CALL_CANDIDATES;
    if (contentJson.find("m.call.negotiate") != std::string::npos)
        return ContentType::CALL_NEGOTIATE;
    if (contentJson.find("m.call.select_answer") != std::string::npos)
        return ContentType::CALL_SELECT_ANSWER;

    // Voice broadcast
    if (contentJson.find("org.matrix.msc3890.voice_broadcast") != std::string::npos)
        return ContentType::VOICE_BROADCAST_INFO;

    // State events (have state_key)
    if (contentJson.find("\"state_key\"") != std::string::npos)
        return ContentType::STATE_EVENT;

    return ContentType::UNKNOWN;
}

// Original Kotlin: getContentTypeDescription()
const char* MessageContentTypeDetector::getContentTypeDescription(ContentType type) {
    switch (type) {
        case ContentType::TEXT:                  return "Text message";
        case ContentType::NOTICE:                return "Notice";
        case ContentType::EMOTE:                 return "Emote";
        case ContentType::IMAGE:                 return "Image";
        case ContentType::VIDEO:                 return "Video";
        case ContentType::AUDIO:                 return "Audio";
        case ContentType::FILE:                  return "File";
        case ContentType::LOCATION:              return "Location";
        case ContentType::POLL_START:            return "Poll started";
        case ContentType::POLL_RESPONSE:         return "Poll response";
        case ContentType::POLL_END:              return "Poll ended";
        case ContentType::STICKER:               return "Sticker";
        case ContentType::BEACON_INFO:           return "Live location beacon";
        case ContentType::BEACON_LOCATION_DATA:  return "Beacon location data";
        case ContentType::VERIFICATION_REQUEST:  return "Verification request";
        case ContentType::VERIFICATION_START:    return "Verification started";
        case ContentType::VERIFICATION_DONE:     return "Verification done";
        case ContentType::VERIFICATION_CANCEL:   return "Verification cancelled";
        case ContentType::VERIFICATION_READY:    return "Verification ready";
        case ContentType::VERIFICATION_KEY:      return "Verification key";
        case ContentType::VERIFICATION_MAC:      return "Verification MAC";
        case ContentType::CALL_INVITE:           return "Call invite";
        case ContentType::CALL_ANSWER:           return "Call answer";
        case ContentType::CALL_HANGUP:           return "Call hangup";
        case ContentType::CALL_REJECT:           return "Call reject";
        case ContentType::CALL_CANDIDATES:       return "Call candidates";
        case ContentType::CALL_NEGOTIATE:        return "Call negotiate";
        case ContentType::CALL_SELECT_ANSWER:    return "Call select answer";
        case ContentType::VOICE_BROADCAST_INFO:  return "Voice broadcast";
        case ContentType::STATE_EVENT:           return "State event";
        case ContentType::UNKNOWN:               return "Unknown";
    }
    return "Unknown";
}

// Original Kotlin: isContentTypeDisplayable()
bool MessageContentTypeDetector::isContentTypeDisplayable(ContentType type) {
    switch (type) {
        case ContentType::TEXT:
        case ContentType::NOTICE:
        case ContentType::EMOTE:
        case ContentType::IMAGE:
        case ContentType::VIDEO:
        case ContentType::AUDIO:
        case ContentType::FILE:
        case ContentType::LOCATION:
        case ContentType::POLL_START:
        case ContentType::POLL_RESPONSE:
        case ContentType::POLL_END:
        case ContentType::STICKER:
        case ContentType::BEACON_INFO:
        case ContentType::BEACON_LOCATION_DATA:
            return true;
        default:
            return false;
    }
}

// Original Kotlin: getContentTypeIcon()
const char* MessageContentTypeDetector::getContentTypeIcon(ContentType type) {
    switch (type) {
        case ContentType::TEXT:                  return "ic_message_text";
        case ContentType::NOTICE:                return "ic_message_notice";
        case ContentType::EMOTE:                 return "ic_message_emote";
        case ContentType::IMAGE:                 return "ic_message_image";
        case ContentType::VIDEO:                 return "ic_message_video";
        case ContentType::AUDIO:                 return "ic_message_audio";
        case ContentType::FILE:                  return "ic_message_file";
        case ContentType::LOCATION:              return "ic_message_location";
        case ContentType::POLL_START:
        case ContentType::POLL_RESPONSE:
        case ContentType::POLL_END:              return "ic_poll";
        case ContentType::STICKER:               return "ic_sticker";
        case ContentType::BEACON_INFO:
        case ContentType::BEACON_LOCATION_DATA:  return "ic_beacon";
        case ContentType::VERIFICATION_REQUEST:
        case ContentType::VERIFICATION_START:
        case ContentType::VERIFICATION_DONE:
        case ContentType::VERIFICATION_CANCEL:
        case ContentType::VERIFICATION_READY:
        case ContentType::VERIFICATION_KEY:
        case ContentType::VERIFICATION_MAC:      return "ic_verification";
        case ContentType::CALL_INVITE:
        case ContentType::CALL_ANSWER:
        case ContentType::CALL_HANGUP:
        case ContentType::CALL_REJECT:
        case ContentType::CALL_CANDIDATES:
        case ContentType::CALL_NEGOTIATE:
        case ContentType::CALL_SELECT_ANSWER:    return "ic_call";
        case ContentType::VOICE_BROADCAST_INFO:  return "ic_voice_broadcast";
        case ContentType::STATE_EVENT:           return "ic_state_event";
        case ContentType::UNKNOWN:               return "ic_message_unknown";
    }
    return "ic_message_unknown";
}

// Original Kotlin: computeMessageContentStats()
MessageContentStats computeMessageContentStats(const std::vector<std::string>& contentJsons) {
    MessageContentStats stats;
    for (const auto& json : contentJsons) {
        auto ct = MessageContentTypeDetector::detectContentType(json);
        stats.totalCount++;
        switch (ct) {
            case ContentType::TEXT:
            case ContentType::NOTICE:
            case ContentType::EMOTE:
                stats.textCount++; break;
            case ContentType::IMAGE:  stats.imageCount++; break;
            case ContentType::VIDEO:  stats.videoCount++; break;
            case ContentType::AUDIO:  stats.audioCount++; break;
            case ContentType::FILE:   stats.fileCount++; break;
            case ContentType::LOCATION: stats.locationCount++; break;
            case ContentType::POLL_START:
            case ContentType::POLL_RESPONSE:
            case ContentType::POLL_END:
                stats.pollCount++; break;
            case ContentType::STICKER: stats.stickerCount++; break;
            case ContentType::CALL_INVITE:
            case ContentType::CALL_ANSWER:
            case ContentType::CALL_HANGUP:
            case ContentType::CALL_REJECT:
            case ContentType::CALL_CANDIDATES:
            case ContentType::CALL_NEGOTIATE:
            case ContentType::CALL_SELECT_ANSWER:
                stats.callCount++; break;
            case ContentType::VERIFICATION_REQUEST:
            case ContentType::VERIFICATION_START:
            case ContentType::VERIFICATION_DONE:
            case ContentType::VERIFICATION_CANCEL:
            case ContentType::VERIFICATION_READY:
            case ContentType::VERIFICATION_KEY:
            case ContentType::VERIFICATION_MAC:
                stats.verificationCount++; break;
            default: stats.otherCount++; break;
        }
    }
    return stats;
}

} // namespace progressive
