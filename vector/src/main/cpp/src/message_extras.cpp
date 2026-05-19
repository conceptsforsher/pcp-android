#include "progressive/message_extras.hpp"

namespace progressive {

// ==== JSON Helpers ====

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

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
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

static std::vector<std::string> extractJsonArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return result;
    pos = json.find('[', pos);
    if (pos == std::string::npos) return result;
    pos++; // skip '['
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == ',')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '"') {
            pos++;
            size_t end = pos;
            while (end < json.size() && json[end] != '"') { if (json[end] == '\\') end++; end++; }
            result.push_back(json.substr(pos, end - pos));
            pos = end + 1;
        } else if (json[pos] == '{') {
            int d = 1;
            size_t start = pos;
            pos++;
            while (pos < json.size() && d > 0) {
                if (json[pos] == '{') d++;
                else if (json[pos] == '}') d--;
                pos++;
            }
            result.push_back(json.substr(start, pos - start));
        } else {
            pos++;
        }
    }
    return result;
}

// ==== Poll Type Conversions ====
//
// Original Kotlin (PollType.kt):
//   enum class PollType {
//       @Json(name="org.matrix.msc3381.poll.disclosed") DISCLOSED_UNSTABLE,
//       @Json(name="m.poll.disclosed") DISCLOSED,
//       @Json(name="org.matrix.msc3381.poll.undisclosed") UNDISCLOSED_UNSTABLE,
//       @Json(name="m.poll.undisclosed") UNDISCLOSED
//   }

std::string pollTypeToString(PollType type) {
    switch (type) {
        case PollType::DISCLOSED_UNSTABLE: return "org.matrix.msc3381.poll.disclosed";
        case PollType::DISCLOSED: return "m.poll.disclosed";
        case PollType::UNDISCLOSED_UNSTABLE: return "org.matrix.msc3381.poll.undisclosed";
        case PollType::UNDISCLOSED: return "m.poll.undisclosed";
    }
    return "org.matrix.msc3381.poll.disclosed";
}

PollType pollTypeFromString(const std::string& s) {
    if (s == "m.poll.disclosed") return PollType::DISCLOSED;
    if (s == "m.poll.undisclosed") return PollType::UNDISCLOSED;
    if (s == "org.matrix.msc3381.poll.disclosed") return PollType::DISCLOSED_UNSTABLE;
    return PollType::UNDISCLOSED_UNSTABLE;
}

// ==== Parse Poll Question ====
//
// Original Kotlin (PollQuestion.kt:24-31):
//   data class PollQuestion(
//       @Json(name="org.matrix.msc1767.text") unstableQuestion,
//       @Json(name="m.text") question
//   )

static PollQuestion parsePollQuestion(const std::string& json) {
    PollQuestion q;
    q.question = extractJsonString(json, "m.text");
    q.unstableQuestion = extractJsonString(json, "org.matrix.msc1767.text");
    return q;
}

// ==== Parse Poll Answer ====
//
// Original Kotlin (PollAnswer.kt:23-33):
//   data class PollAnswer(
//       @Json(name="id") id,
//       @Json(name="org.matrix.msc1767.text") unstableAnswer,
//       @Json(name="m.text") answer
//   )

static PollAnswer parsePollAnswer(const std::string& json) {
    PollAnswer a;
    a.id = extractJsonString(json, "id");
    a.answer = extractJsonString(json, "m.text");
    a.unstableAnswer = extractJsonString(json, "org.matrix.msc1767.text");
    return a;
}

// ==== Parse Poll Creation Info ====
//
// Original Kotlin (PollCreationInfo.kt:25-36):
//   data class PollCreationInfo(
//       @Json(name="question") question,
//       @Json(name="kind") kind = DISCLOSED_UNSTABLE,
//       @Json(name="max_selections") maxSelections = 1,
//       @Json(name="answers") answers
//   )

static PollCreationInfo parsePollCreationInfo(const std::string& json) {
    PollCreationInfo info;
    auto qJson = extractJsonObject(json, "question");
    if (!qJson.empty()) info.question = parsePollQuestion(qJson);

    auto kindStr = extractJsonString(json, "kind");
    if (!kindStr.empty()) info.kind = pollTypeFromString(kindStr);

    info.maxSelections = static_cast<int>(extractJsonInt64(json, "max_selections"));
    if (info.maxSelections < 1) info.maxSelections = 1;

    // Parse answers array
    auto answersArr = extractJsonArray(json, "answers");
    for (const auto& ansJson : answersArr) {
        if (!ansJson.empty() && ansJson[0] == '{') {
            info.answers.push_back(parsePollAnswer(ansJson));
        }
    }

    return info;
}

// ==== Parse Poll Content ====
//
// Original Kotlin (MessagePollContent.kt:25-47):
//   @Json(name="org.matrix.msc3381.poll.start") unstablePollCreationInfo,
//   @Json(name="m.poll.start") pollCreationInfo

MessagePollContent parsePollContent(const std::string& contentJson) {
    MessagePollContent c;
    c.msgType = "org.matrix.android.sdk.poll.start";
    c.body = extractJsonString(contentJson, "body");

    auto stableJson = extractJsonObject(contentJson, "m.poll.start");
    if (!stableJson.empty()) c.pollCreationInfo = parsePollCreationInfo(stableJson);

    auto unstableJson = extractJsonObject(contentJson, "org.matrix.msc3381.poll.start");
    if (!unstableJson.empty()) c.unstablePollCreationInfo = parsePollCreationInfo(unstableJson);

    return c;
}

// ==== Parse Poll Response ====
//
// Original Kotlin (MessagePollResponseContent.kt:27-49):
//   @Json(name="org.matrix.msc3381.poll.response") unstableResponse,
//   @Json(name="m.response") response

MessagePollResponseContent parsePollResponseContent(const std::string& contentJson) {
    MessagePollResponseContent c;
    c.msgType = "org.matrix.android.sdk.poll.response";
    c.body = extractJsonString(contentJson, "body");

    auto stableResp = extractJsonObject(contentJson, "m.response");
    if (!stableResp.empty()) {
        c.response.answerIds = extractJsonArray(stableResp, "answers");
    }

    auto unstableResp = extractJsonObject(contentJson, "org.matrix.msc3381.poll.response");
    if (!unstableResp.empty()) {
        c.unstableResponse.answerIds = extractJsonArray(unstableResp, "answers");
    }

    return c;
}

// ==== Parse End Poll Content ====
//
// Original Kotlin (MessageEndPollContent.kt:27-40):
//   @Json(name="org.matrix.msc1767.text") unstableText,
//   @Json(name="m.text") text

MessageEndPollContent parseEndPollContent(const std::string& contentJson) {
    MessageEndPollContent c;
    c.msgType = "org.matrix.android.sdk.poll.end";
    c.body = extractJsonString(contentJson, "body");
    c.text = extractJsonString(contentJson, "m.text");
    c.unstableText = extractJsonString(contentJson, "org.matrix.msc1767.text");
    return c;
}

// ==== Parse Beacon Content ====
//
// Original Kotlin (MessageBeaconInfoContent.kt:28-67)

MessageBeaconInfoContent parseBeaconInfoContent(const std::string& contentJson) {
    MessageBeaconInfoContent c;
    c.msgType = "org.matrix.android.sdk.beacon.info";
    c.body = extractJsonString(contentJson, "body");
    c.description = extractJsonString(contentJson, "description");
    c.timeout = extractJsonInt64(contentJson, "timeout");
    c.isLive = extractJsonBool(contentJson, "live");

    // Timestamp: prefer stable key
    c.timestampMillis = extractJsonInt64(contentJson, "m.ts");
    if (c.timestampMillis == 0) c.timestampMillis = extractJsonInt64(contentJson, "org.matrix.msc3488.ts");

    // Location asset
    auto assetJson = extractJsonObject(contentJson, "m.asset");
    if (assetJson.empty()) assetJson = extractJsonObject(contentJson, "org.matrix.msc3488.asset");
    if (!assetJson.empty()) {
        c.locationAsset.type = extractJsonString(assetJson, "type");
    }

    return c;
}

MessageBeaconLocationDataContent parseBeaconLocationDataContent(const std::string& contentJson) {
    MessageBeaconLocationDataContent c;
    c.msgType = "org.matrix.android.sdk.beacon.location.data";
    c.body = extractJsonString(contentJson, "body");

    auto locJson = extractJsonObject(contentJson, "m.location");
    if (locJson.empty()) locJson = extractJsonObject(contentJson, "org.matrix.msc3488.location");
    if (!locJson.empty()) {
        c.locationInfo.geoUri = extractJsonString(locJson, "uri");
        c.locationInfo.description = extractJsonString(locJson, "description");
    }

    c.timestampMillis = extractJsonInt64(contentJson, "m.ts");
    if (c.timestampMillis == 0) c.timestampMillis = extractJsonInt64(contentJson, "org.matrix.msc3488.ts");

    return c;
}

// ==== Parse Location Content (MSC3488 enhanced) ====
//
// Original Kotlin (MessageLocationContent.kt:25-90)

MessageEnhancedLocationContent parseEnhancedLocationContent(const std::string& contentJson) {
    MessageEnhancedLocationContent c;
    c.msgType = "m.location";
    c.body = extractJsonString(contentJson, "body");
    c.geoUri = extractJsonString(contentJson, "geo_uri");

    auto locJson = extractJsonObject(contentJson, "m.location");
    if (locJson.empty()) locJson = extractJsonObject(contentJson, "org.matrix.msc3488.location");
    if (!locJson.empty()) {
        c.locationInfo.geoUri = extractJsonString(locJson, "uri");
        c.locationInfo.description = extractJsonString(locJson, "description");
    }

    // Original Kotlin: getBestTimestampMillis()
    c.timestampMillis = extractJsonInt64(contentJson, "m.ts");
    if (c.timestampMillis == 0) c.timestampMillis = extractJsonInt64(contentJson, "org.matrix.msc3488.ts");

    // Original Kotlin: getBestText()
    c.text = extractJsonString(contentJson, "m.text");
    if (c.text.empty()) c.text = extractJsonString(contentJson, "org.matrix.msc1767.text");

    // Original Kotlin: getBestLocationAsset()
    auto assetJson = extractJsonObject(contentJson, "m.asset");
    if (assetJson.empty()) assetJson = extractJsonObject(contentJson, "org.matrix.msc3488.asset");
    if (!assetJson.empty()) {
        c.locationAsset.type = extractJsonString(assetJson, "type");
    }

    return c;
}

// ==== Parse Sticker Content ====
//
// Original Kotlin (MessageStickerContent.kt:28-54):
//   Same structure as image but with STICKER_LOCAL msgtype

MessageStickerContent parseStickerContent(const std::string& contentJson) {
    MessageStickerContent c;
    c.msgType = "org.matrix.android.sdk.sticker";
    c.body = extractJsonString(contentJson, "body");
    c.url = extractJsonString(contentJson, "url");
    // info handled by base MessageImageContent parsing
    return c;
}

// ==== Poll Serialization ====

static std::string escapeJsonStr(const std::string& s) {
    std::string r = "\"";
    for (char c : s) {
        if (c == '"') r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else if (c == '\n') r += "\\n";
        else r += c;
    }
    r += "\"";
    return r;
}

std::string pollContentToJson(const MessagePollContent& poll) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJsonStr(poll.msgType) + ",";
    json += "\"body\":" + escapeJsonStr(poll.body) + ",";
    json += "\"m.poll.start\":{";
    auto& info = poll.pollCreationInfo;
    json += "\"question\":{\"m.text\":" + escapeJsonStr(info.question.getBestQuestion()) + "},";
    json += "\"kind\":" + escapeJsonStr(pollTypeToString(info.kind)) + ",";
    json += "\"max_selections\":" + std::to_string(info.maxSelections) + ",";
    json += "\"answers\":[";
    bool first = true;
    for (const auto& ans : info.answers) {
        if (!first) json += ",";
        first = false;
        json += "{\"id\":" + escapeJsonStr(ans.id) + ",\"m.text\":" + escapeJsonStr(ans.getBestAnswer()) + "}";
    }
    json += "]}";
    json += "}";
    return json;
}

std::string pollResponseToJson(const MessagePollResponseContent& response) {
    auto best = response.getBestResponse();
    std::string json = "{";
    json += "\"msgtype\":" + escapeJsonStr(response.msgType) + ",";
    json += "\"body\":" + escapeJsonStr(response.body) + ",";
    json += "\"m.response\":{\"answers\":[";
    bool first = true;
    for (const auto& id : best.answerIds) {
        if (!first) json += ",";
        first = false;
        json += escapeJsonStr(id);
    }
    json += "]}}";
    return json;
}

std::string endPollContentToJson(const MessageEndPollContent& endPoll) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJsonStr(endPoll.msgType) + ",";
    json += "\"body\":" + escapeJsonStr(endPoll.body) + ",";
    json += "\"m.text\":" + escapeJsonStr(endPoll.getBestText());
    json += "}";
    return json;
}

// ==== Verification Parsing ====
//
// Original Kotlin (MessageVerificationRequestContent.kt:28-43)

MessageVerificationRequestContent parseVerificationRequest(const std::string& json) {
    MessageVerificationRequestContent c;
    c.msgType = "m.key.verification.request";
    c.body = extractJsonString(json, "body");
    c.fromDevice = extractJsonString(json, "from_device");
    c.toUserId = extractJsonString(json, "to");
    c.timestamp = extractJsonInt64(json, "timestamp");
    c.format = extractJsonString(json, "format");
    c.formattedBody = extractJsonString(json, "formatted_body");

    // Parse methods array
    auto methodsArr = extractJsonArray(json, "methods");
    for (const auto& m : methodsArr) {
        if (!m.empty()) c.methods.push_back(m);
    }

    return c;
}

// Original Kotlin (MessageVerificationStartContent.kt:26-40)
MessageVerificationStartContent parseVerificationStart(const std::string& json) {
    MessageVerificationStartContent c;
    c.fromDevice = extractJsonString(json, "from_device");
    c.method = extractJsonString(json, "method");
    c.sharedSecret = extractJsonString(json, "secret");

    // Array fields
    auto hashesArr = extractJsonArray(json, "hashes");
    for (const auto& h : hashesArr) c.hashes.push_back(h);
    auto protocols = extractJsonArray(json, "key_agreement_protocols");
    for (const auto& p : protocols) c.keyAgreementProtocols.push_back(p);
    auto macs = extractJsonArray(json, "message_authentication_codes");
    for (const auto& m : macs) c.messageAuthenticationCodes.push_back(m);
    auto sas = extractJsonArray(json, "short_authentication_string");
    for (const auto& s : sas) c.shortAuthenticationStrings.push_back(s);

    // Relation
    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.transactionId = c.relatesTo.eventId;
    }

    return c;
}

// Original Kotlin (MessageVerificationReadyContent.kt:26-33)
MessageVerificationReadyContent parseVerificationReady(const std::string& json) {
    MessageVerificationReadyContent c;
    c.fromDevice = extractJsonString(json, "from_device");
    auto methodsArr = extractJsonArray(json, "methods");
    for (const auto& m : methodsArr) c.methods.push_back(m);

    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.transactionId = c.relatesTo.eventId;
    }

    return c;
}

// Original Kotlin (MessageVerificationDoneContent.kt:26-32)
MessageVerificationDoneContent parseVerificationDone(const std::string& json) {
    MessageVerificationDoneContent c;
    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.transactionId = c.relatesTo.eventId;
    }
    return c;
}

// Original Kotlin (MessageVerificationCancelContent.kt:28-36)
MessageVerificationCancelContent parseVerificationCancel(const std::string& json) {
    MessageVerificationCancelContent c;
    c.code = extractJsonString(json, "code");
    c.reason = extractJsonString(json, "reason");
    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.transactionId = c.relatesTo.eventId;
    }
    return c;
}

// Original Kotlin (MessageVerificationMacContent.kt:27-35)
MessageVerificationMacContent parseVerificationMac(const std::string& json) {
    MessageVerificationMacContent c;
    c.keys = extractJsonString(json, "keys");

    // Parse mac object as key-value string pairs
    auto macJson = extractJsonObject(json, "mac");
    if (!macJson.empty()) {
        size_t pos = 1;
        while (pos < macJson.size()) {
            while (pos < macJson.size() && (macJson[pos] == ' ' || macJson[pos] == ',')) pos++;
            if (pos >= macJson.size() || macJson[pos] == '}') break;
            if (macJson[pos] == '"') {
                pos++;
                size_t keyEnd = pos;
                while (keyEnd < macJson.size() && macJson[keyEnd] != '"') keyEnd++;
                std::string key = macJson.substr(pos, keyEnd - pos);
                pos = keyEnd + 1;
                while (pos < macJson.size() && macJson[pos] != ':') pos++;
                pos++;
                while (pos < macJson.size() && (macJson[pos] == ' ' || macJson[pos] == '\t')) pos++;
                if (pos < macJson.size() && macJson[pos] == '"') {
                    pos++;
                    size_t valEnd = pos;
                    while (valEnd < macJson.size() && macJson[valEnd] != '"') {
                        if (macJson[valEnd] == '\\') valEnd++;
                        valEnd++;
                    }
                    c.mac[key] = macJson.substr(pos, valEnd - pos);
                    pos = valEnd + 1;
                }
            }
        }
    }

    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.transactionId = c.relatesTo.eventId;
    }

    return c;
}

// Original Kotlin (MessageVerificationKeyContent.kt:28-36)
MessageVerificationKeyContent parseVerificationKey(const std::string& json) {
    MessageVerificationKeyContent c;
    c.key = extractJsonString(json, "key");
    auto relJson = extractJsonObject(json, "m.relates_to");
    if (!relJson.empty()) {
        c.relatesTo.eventId = extractJsonString(relJson, "event_id");
        c.transactionId = c.relatesTo.eventId;
    }
    return c;
}

// ==== Element Call Notification Parsing ====
//
// Original Kotlin (ElementCallNotifyContent.kt:24-33)

ElementCallNotifyContent parseCallNotifyContent(const std::string& json) {
    ElementCallNotifyContent c;
    c.application = extractJsonString(json, "application");
    c.callId = extractJsonString(json, "call_id");
    c.notifyType = extractJsonString(json, "notify_type");

    // Original Kotlin: mentions object
    auto mentionsJson = extractJsonObject(json, "m.mentions");
    if (!mentionsJson.empty()) {
        c.mentions.room = extractJsonBool(mentionsJson, "room");
        auto userIdsArr = extractJsonArray(mentionsJson, "user_ids");
        for (const auto& uid : userIdsArr) {
            if (!uid.empty()) c.mentions.userIds.push_back(uid);
        }
    }

    return c;
}

// ==== Message Type Helpers ====
//
// Original Kotlin: Determine msgtype from content JSON

std::string getMessageType(const std::string& contentJson) {
    return extractJsonString(contentJson, "msgtype");
}

// Original Kotlin (MessageVoiceContent.kt helpers):
//   Voice messages have msgtype=m.audio and a voice indicator key
bool isVoiceMessage(const std::string& contentJson) {
    std::string msgType = extractJsonString(contentJson, "msgtype");
    if (msgType != "m.audio") return false;
    // Check for voice indicator: "org.matrix.msc3245.voice" or "org.matrix.msc2516.voice"
    if (contentJson.find("\"org.matrix.msc3245.voice\"") != std::string::npos) return true;
    if (contentJson.find("\"org.matrix.msc2516.voice\"") != std::string::npos) return true;
    return false;
}

bool isVideoMessage(const std::string& contentJson) {
    return extractJsonString(contentJson, "msgtype") == "m.video";
}

bool isFileMessage(const std::string& contentJson) {
    return extractJsonString(contentJson, "msgtype") == "m.file";
}

bool isImageMessage(const std::string& contentJson) {
    return extractJsonString(contentJson, "msgtype") == "m.image";
}

bool isAudioMessage(const std::string& contentJson) {
    return extractJsonString(contentJson, "msgtype") == "m.audio";
}

// ==== Parse ThumbnailInfo from JSON ====
//
// Original Kotlin (ThumbnailInfo.kt:24-40):
//   data class ThumbnailInfo(
//       @Json(name="w") width, @Json(name="h") height,
//       @Json(name="size") size, @Json(name="mimetype") mimeType
//   )

static ThumbnailInfo parseThumbnailInfo(const std::string& json) {
    ThumbnailInfo info;
    info.width = static_cast<int>(extractJsonInt64(json, "w"));
    info.height = static_cast<int>(extractJsonInt64(json, "h"));
    info.size = extractJsonInt64(json, "size");
    info.mimeType = extractJsonString(json, "mimetype");
    return info;
}

// ==== Parse VideoInfo from JSON ====
//
// Original Kotlin (VideoInfo.kt:25-56):
//   data class VideoInfo(
//       @Json(name="w") width, @Json(name="h") height,
//       @Json(name="duration") duration, @Json(name="mimetype") mimeType,
//       @Json(name="size") size, @Json(name="thumbnail_url") thumbnailUrl,
//       @Json(name="thumbnail_info") thumbnailInfo
//   )

static VideoInfo parseVideoInfo(const std::string& json) {
    VideoInfo info;
    info.width = static_cast<int>(extractJsonInt64(json, "w"));
    info.height = static_cast<int>(extractJsonInt64(json, "h"));
    info.duration = static_cast<int>(extractJsonInt64(json, "duration"));
    info.mimeType = extractJsonString(json, "mimetype");
    info.size = extractJsonInt64(json, "size");
    info.thumbnailUrl = extractJsonString(json, "thumbnail_url");

    auto thumbJson = extractJsonObject(json, "thumbnail_info");
    if (!thumbJson.empty()) {
        info.thumbnailInfo = parseThumbnailInfo(thumbJson);
    }

    return info;
}

// ==== Parse AudioInfo from JSON ====
//
// Original Kotlin (AudioInfo.kt:23-36):
//   data class AudioInfo(
//       @Json(name="duration") duration, @Json(name="mimetype") mimeType,
//       @Json(name="size") size
//   )

static AudioInfo parseAudioInfo(const std::string& json) {
    AudioInfo info;
    info.duration = static_cast<int>(extractJsonInt64(json, "duration"));
    info.mimeType = extractJsonString(json, "mimetype");
    info.size = extractJsonInt64(json, "size");
    return info;
}

// ==== Parse FileInfo from JSON ====
//
// Original Kotlin (FileInfo.kt:26-46):
//   data class FileInfo(
//       @Json(name="mimetype") mimeType, @Json(name="size") size,
//       @Json(name="thumbnail_url") thumbnailUrl,
//       @Json(name="thumbnail_info") thumbnailInfo
//   )

static FileInfo parseFileInfo(const std::string& json) {
    FileInfo info;
    info.mimeType = extractJsonString(json, "mimetype");
    info.size = extractJsonInt64(json, "size");
    info.thumbnailUrl = extractJsonString(json, "thumbnail_url");

    auto thumbJson = extractJsonObject(json, "thumbnail_info");
    if (!thumbJson.empty()) {
        info.thumbnailInfo = parseThumbnailInfo(thumbJson);
    }

    return info;
}

// ==== Parse ImageInfo from JSON ====
//
// Original Kotlin (ImageInfo.kt:25-55):
//   data class ImageInfo(
//       @Json(name="w") width, @Json(name="h") height,
//       @Json(name="size") size, @Json(name="mimetype") mimeType,
//       @Json(name="thumbnail_url") thumbnailUrl,
//       @Json(name="thumbnail_info") thumbnailInfo
//   )

static ImageInfo parseImageInfo(const std::string& json) {
    ImageInfo info;
    info.width = static_cast<int>(extractJsonInt64(json, "w"));
    info.height = static_cast<int>(extractJsonInt64(json, "h"));
    info.size = extractJsonInt64(json, "size");
    info.mimeType = extractJsonString(json, "mimetype");
    info.thumbnailUrl = extractJsonString(json, "thumbnail_url");

    auto thumbJson = extractJsonObject(json, "thumbnail_info");
    if (!thumbJson.empty()) {
        info.thumbnailInfo = parseThumbnailInfo(thumbJson);
    }

    return info;
}

// ==== Parse Video Content ====
//
// Original Kotlin (MessageVideoContent.kt:28-56):
//   data class MessageVideoContent(
//       @Transient override msgType = MSGTYPE_VIDEO,
//       body, relatesTo, newContent,
//       videoInfo, url, file
//   ) : MessageContentWithAttachmentContent

MessageVideoContent parseVideoContent(const std::string& contentJson) {
    MessageVideoContent c;
    c.msgType = "m.video";
    c.body = extractJsonString(contentJson, "body");
    c.url = extractJsonString(contentJson, "url");

    auto infoJson = extractJsonObject(contentJson, "info");
    if (!infoJson.empty()) {
        c.videoInfo = parseVideoInfo(infoJson);
        c.mimeType = c.videoInfo.mimeType;
        c.duration = c.videoInfo.duration;
        c.width = c.videoInfo.width;
        c.height = c.videoInfo.height;
        c.size = c.videoInfo.size;
        c.thumbnailUrl = c.videoInfo.getThumbnailUrl();
    }

    // Parse encrypted file info
    auto encJson = extractJsonObject(contentJson, "file");
    if (!encJson.empty()) {
        c.encryptedFile.url = extractJsonString(encJson, "url");
        c.encryptedFile.iv = extractJsonString(encJson, "iv");
        c.encryptedFile.keyJson = extractJsonString(encJson, "key");
        c.encryptedFile.hashes = extractJsonObject(encJson, "hashes");
    }

    return c;
}

// ==== Parse Audio Content ====
//
// Original Kotlin (MessageAudioContent.kt:28-63):
//   data class MessageAudioContent(
//       @Transient override msgType = MSGTYPE_AUDIO,
//       body, relatesTo, newContent,
//       url, info, audioWaveFormInfo, file
//   )

MessageAudioContent parseAudioContent(const std::string& contentJson) {
    MessageAudioContent c;
    c.msgType = "m.audio";
    c.body = extractJsonString(contentJson, "body");
    c.url = extractJsonString(contentJson, "url");

    auto infoJson = extractJsonObject(contentJson, "info");
    if (!infoJson.empty()) {
        c.audioInfo = parseAudioInfo(infoJson);
        c.mimeType = c.audioInfo.mimeType;
        c.duration = c.audioInfo.duration;
        c.size = c.audioInfo.size;
    }

    // Parse voice message indicator
    // Original Kotlin: check for "org.matrix.msc3245.voice" or "org.matrix.msc2516.voice"
    if (contentJson.find("\"org.matrix.msc3245.voice\"") != std::string::npos ||
        contentJson.find("\"org.matrix.msc2516.voice\"") != std::string::npos) {
        c.isVoiceMessage = true;
    }

    // Parse audio waveform (MSC1767)
    // {"org.matrix.msc1767.audio":{"duration":5000,"waveform":[1,2,3,...]}}
    auto waveJson = extractJsonObject(contentJson, "org.matrix.msc1767.audio");
    if (!waveJson.empty()) {
        c.audioWaveform.duration = static_cast<int>(extractJsonInt64(waveJson, "duration"));
        auto waveArr = extractJsonArray(waveJson, "waveform");
        for (const auto& w : waveArr) {
            int val = 0;
            for (char ch : w) {
                if (ch >= '0' && ch <= '9') val = val * 10 + (ch - '0');
                else if (ch == '-') val = 0;
            }
            c.audioWaveform.waveform.push_back(val);
        }
    }

    // Parse encrypted file info
    auto encJson = extractJsonObject(contentJson, "file");
    if (!encJson.empty()) {
        c.encryptedFile.url = extractJsonString(encJson, "url");
        c.encryptedFile.iv = extractJsonString(encJson, "iv");
        c.encryptedFile.keyJson = extractJsonString(encJson, "key");
        c.encryptedFile.hashes = extractJsonObject(encJson, "hashes");
    }

    return c;
}

// ==== Parse File Content ====
//
// Original Kotlin (MessageFileContent.kt:28-57):
//   data class MessageFileContent(
//       @Transient override msgType = MSGTYPE_FILE,
//       body, relatesTo, newContent,
//       url, filename, info, file
//   )

MessageFileContent parseFileContent(const std::string& contentJson) {
    MessageFileContent c;
    c.msgType = "m.file";
    c.body = extractJsonString(contentJson, "body");
    c.url = extractJsonString(contentJson, "url");
    c.filename = extractJsonString(contentJson, "filename");

    auto infoJson = extractJsonObject(contentJson, "info");
    if (!infoJson.empty()) {
        c.info = parseFileInfo(infoJson);
        c.mimeType = c.info.mimeType;
        c.size = c.info.size;
    }

    // Parse encrypted file info
    auto encJson = extractJsonObject(contentJson, "file");
    if (!encJson.empty()) {
        c.encryptedFile.url = extractJsonString(encJson, "url");
        c.encryptedFile.iv = extractJsonString(encJson, "iv");
        c.encryptedFile.keyJson = extractJsonString(encJson, "key");
        c.encryptedFile.hashes = extractJsonObject(encJson, "hashes");
    }

    return c;
}

// ==== JSON Builders ====

// Build video info as JSON
static std::string videoInfoToJson(const VideoInfo& info) {
    std::string json = "{";
    if (info.width > 0) json += "\"w\":" + std::to_string(info.width) + ",";
    else json += "\"w\":0,";
    if (info.height > 0) json += "\"h\":" + std::to_string(info.height) + ",";
    else json += "\"h\":0,";
    if (info.duration > 0) json += "\"duration\":" + std::to_string(info.duration) + ",";
    if (!info.mimeType.empty()) json += "\"mimetype\":" + escapeJsonStr(info.mimeType) + ",";
    if (info.size > 0) json += "\"size\":" + std::to_string(info.size) + ",";
    if (!info.thumbnailUrl.empty()) json += "\"thumbnail_url\":" + escapeJsonStr(info.thumbnailUrl) + ",";
    if (info.thumbnailInfo.hasData()) {
        json += "\"thumbnail_info\":{\"w\":" + std::to_string(info.thumbnailInfo.width) +
                ",\"h\":" + std::to_string(info.thumbnailInfo.height) + ",";
        if (!info.thumbnailInfo.mimeType.empty())
            json += "\"mimetype\":" + escapeJsonStr(info.thumbnailInfo.mimeType) + ",";
        json += "\"size\":" + std::to_string(info.thumbnailInfo.size) + "},";
    }
    // Remove trailing comma
    if (json.back() == ',') json.pop_back();
    json += "}";
    return json;
}

// Build audio info as JSON
static std::string audioInfoToJson(const AudioInfo& info) {
    std::string json = "{";
    if (info.duration > 0) json += "\"duration\":" + std::to_string(info.duration) + ",";
    if (!info.mimeType.empty()) json += "\"mimetype\":" + escapeJsonStr(info.mimeType) + ",";
    if (info.size > 0) json += "\"size\":" + std::to_string(info.size) + ",";
    if (json.back() == ',') json.pop_back();
    json += "}";
    return json;
}

// Build file info as JSON
static std::string fileInfoToJson(const FileInfo& info) {
    std::string json = "{";
    if (!info.mimeType.empty()) json += "\"mimetype\":" + escapeJsonStr(info.mimeType) + ",";
    if (info.size > 0) json += "\"size\":" + std::to_string(info.size) + ",";
    if (!info.thumbnailUrl.empty()) json += "\"thumbnail_url\":" + escapeJsonStr(info.thumbnailUrl) + ",";
    if (info.thumbnailInfo.hasData()) {
        json += "\"thumbnail_info\":{\"w\":" + std::to_string(info.thumbnailInfo.width) +
                ",\"h\":" + std::to_string(info.thumbnailInfo.height) + ",";
        if (!info.thumbnailInfo.mimeType.empty())
            json += "\"mimetype\":" + escapeJsonStr(info.thumbnailInfo.mimeType) + ",";
        json += "\"size\":" + std::to_string(info.thumbnailInfo.size) + "},";
    }
    if (json.back() == ',') json.pop_back();
    json += "}";
    return json;
}

// Build image info as JSON
static std::string imageInfoToJson(const ImageInfo& info) {
    std::string json = "{";
    if (info.width > 0) json += "\"w\":" + std::to_string(info.width) + ",";
    if (info.height > 0) json += "\"h\":" + std::to_string(info.height) + ",";
    if (!info.mimeType.empty()) json += "\"mimetype\":" + escapeJsonStr(info.mimeType) + ",";
    if (info.size > 0) json += "\"size\":" + std::to_string(info.size) + ",";
    if (!info.thumbnailUrl.empty()) json += "\"thumbnail_url\":" + escapeJsonStr(info.thumbnailUrl) + ",";
    if (info.thumbnailInfo.hasData()) {
        json += "\"thumbnail_info\":{\"w\":" + std::to_string(info.thumbnailInfo.width) +
                ",\"h\":" + std::to_string(info.thumbnailInfo.height) + ",";
        if (!info.thumbnailInfo.mimeType.empty())
            json += "\"mimetype\":" + escapeJsonStr(info.thumbnailInfo.mimeType) + ",";
        json += "\"size\":" + std::to_string(info.thumbnailInfo.size) + "},";
    }
    if (json.back() == ',') json.pop_back();
    json += "}";
    return json;
}

std::string buildVideoContent(const MessageVideoContent& video) {
    std::string json = "{";
    json += "\"msgtype\":\"m.video\",";
    json += "\"body\":" + escapeJsonStr(video.body) + ",";
    if (!video.url.empty()) json += "\"url\":" + escapeJsonStr(video.url) + ",";
    // Build info from videoInfo
    json += "\"info\":" + videoInfoToJson(video.videoInfo);

    // If there's an encrypted file, add it
    if (!video.encryptedFile.url.empty()) {
        json += ",\"file\":{";
        json += "\"url\":" + escapeJsonStr(video.encryptedFile.url) + ",";
        json += "\"key\":" + escapeJsonStr(video.encryptedFile.keyJson) + ",";
        json += "\"iv\":" + escapeJsonStr(video.encryptedFile.iv) + ",";
        json += "\"hashes\":" + video.encryptedFile.hashes;
        json += "}";
    }

    json += "}";
    return json;
}

std::string buildAudioContent(const MessageAudioContent& audio) {
    std::string json = "{";
    json += "\"msgtype\":\"m.audio\",";
    json += "\"body\":" + escapeJsonStr(audio.body) + ",";
    if (!audio.url.empty()) json += "\"url\":" + escapeJsonStr(audio.url) + ",";

    // Build info from audioInfo
    json += "\"info\":" + audioInfoToJson(audio.audioInfo);

    // Voice message indicator (MSC3245)
    if (audio.isVoiceMessage) {
        json += ",\"org.matrix.msc3245.voice\":{}";
    }

    // Build audio waveform if present (MSC1767)
    if (!audio.audioWaveform.waveform.empty()) {
        json += ",\"org.matrix.msc1767.audio\":{";
        json += "\"duration\":" + std::to_string(audio.audioWaveform.duration) + ",";
        json += "\"waveform\":[";
        bool first = true;
        for (int val : audio.audioWaveform.waveform) {
            if (!first) json += ",";
            first = false;
            json += std::to_string(val);
        }
        json += "]}";
    }

    // Encrypted file info
    if (!audio.encryptedFile.url.empty()) {
        json += ",\"file\":{";
        json += "\"url\":" + escapeJsonStr(audio.encryptedFile.url) + ",";
        json += "\"key\":" + escapeJsonStr(audio.encryptedFile.keyJson) + ",";
        json += "\"iv\":" + escapeJsonStr(audio.encryptedFile.iv) + ",";
        json += "\"hashes\":" + audio.encryptedFile.hashes;
        json += "}";
    }

    json += "}";
    return json;
}

std::string buildFileContent(const MessageFileContent& file) {
    std::string json = "{";
    json += "\"msgtype\":\"m.file\",";
    json += "\"body\":" + escapeJsonStr(file.body) + ",";
    if (!file.filename.empty()) json += "\"filename\":" + escapeJsonStr(file.filename) + ",";
    if (!file.url.empty()) json += "\"url\":" + escapeJsonStr(file.url) + ",";

    // Build info from FileInfo
    json += "\"info\":" + fileInfoToJson(file.info);

    // Encrypted file info
    if (!file.encryptedFile.url.empty()) {
        json += ",\"file\":{";
        json += "\"url\":" + escapeJsonStr(file.encryptedFile.url) + ",";
        json += "\"key\":" + escapeJsonStr(file.encryptedFile.keyJson) + ",";
        json += "\"iv\":" + escapeJsonStr(file.encryptedFile.iv) + ",";
        json += "\"hashes\":" + file.encryptedFile.hashes;
        json += "}";
    }

    json += "}";
    return json;
}

std::string buildStickerContent(const MessageStickerContent& sticker) {
    std::string json = "{";
    json += "\"body\":" + escapeJsonStr(sticker.body) + ",";
    if (!sticker.url.empty()) json += "\"url\":" + escapeJsonStr(sticker.url) + ",";

    // Build info from ImageInfo
    if (sticker.info.hasData() || !sticker.url.empty()) {
        json += "\"info\":" + imageInfoToJson(sticker.info);
    }

    // Encrypted file info
    if (!sticker.encryptedFile.url.empty()) {
        json += ",\"file\":{";
        json += "\"url\":" + escapeJsonStr(sticker.encryptedFile.url) + ",";
        json += "\"key\":" + escapeJsonStr(sticker.encryptedFile.keyJson) + ",";
        json += "\"iv\":" + escapeJsonStr(sticker.encryptedFile.iv) + ",";
        json += "\"hashes\":" + sticker.encryptedFile.hashes;
        json += "}";
    }

    json += "}";
    return json;
}

std::string buildDefaultContent(const MessageDefaultContent& def) {
    std::string json = "{";
    json += "\"msgtype\":" + escapeJsonStr(def.msgType) + ",";
    json += "\"body\":" + escapeJsonStr(def.body);
    if (!def.newContent.empty()) json += ",\"m.new_content\":" + def.newContent;
    if (def.hasRelation) {
        json += ",\"m.relates_to\":{";
        json += "\"event_id\":" + escapeJsonStr(def.relatesTo.eventId) + ",";
        json += "\"rel_type\":" + escapeJsonStr(def.relatesTo.relationType);
        json += "}";
    }
    json += "}";
    return json;
}

// ==== Audio Waveform Utilities ====
//
// Original Kotlin (VoiceMessageHelper.kt formatWaveform()):
//   Normalize raw waveform data:
//   1. Resample to fit within [minValues, maxValues]
//   2. Normalize amplitudes to [0, maxAmplitude]
//   3. Scale relative to the maximum value in the sample

std::vector<int> formatWaveform(const std::vector<int>& rawWaveform) {
    std::vector<int> result;
    if (rawWaveform.empty()) return result;

    // Determine target sample count
    int targetCount = static_cast<int>(rawWaveform.size());
    // Original Kotlin: clamp to [kAudioWaveformMinValues, kAudioWaveformMaxValues]
    if (targetCount < AudioWaveformConstants::kAudioWaveformMinValues)
        targetCount = AudioWaveformConstants::kAudioWaveformMinValues;
    if (targetCount > AudioWaveformConstants::kAudioWaveformMaxValues)
        targetCount = AudioWaveformConstants::kAudioWaveformMaxValues;

    // Resample to target count using linear interpolation
    result.resize(targetCount, 0);
    double ratio = static_cast<double>(rawWaveform.size()) / targetCount;

    for (int i = 0; i < targetCount; ++i) {
        double srcIdx = i * ratio;
        int idx0 = static_cast<int>(srcIdx);
        int idx1 = (idx0 + 1 < static_cast<int>(rawWaveform.size())) ? idx0 + 1 : idx0;
        double frac = srcIdx - idx0;
        double val = rawWaveform[idx0] * (1.0 - frac) + rawWaveform[idx1] * frac;
        result[i] = static_cast<int>(val);
    }

    // Find max amplitude in result
    int maxAmp = 0;
    for (int val : result) {
        if (val > maxAmp) maxAmp = val;
    }

    // Normalize to [0, kAudioWaveformMaxAmplitude]
    if (maxAmp > 0) {
        double scale = static_cast<double>(AudioWaveformConstants::kAudioWaveformMaxAmplitude) / maxAmp;
        for (int& val : result) {
            val = static_cast<int>(val * scale);
        }
    }

    return result;
}

} // namespace progressive
