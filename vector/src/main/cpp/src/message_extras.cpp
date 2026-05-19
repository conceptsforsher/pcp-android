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

} // namespace progressive
