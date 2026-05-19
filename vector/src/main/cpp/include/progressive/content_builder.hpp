#pragma once

#include <string>
#include <vector>

namespace progressive {

// ==== Event Content Builder ====
//
// Builds JSON content bodies for various Matrix event types.
// Used for sending messages, state events, and room configuration.
// Original Kotlin: EventFactory, LocalEchoEventFactory, various content builders

// ==== Message Content ====

// Build m.room.message content for text.
// msgType: "m.text", "m.notice", "m.emote"
inline std::string buildTextContent(const std::string& msgType, const std::string& body,
    const std::string& formattedBody = "", const std::string& format = "org.matrix.custom.html")
{
    std::string json = R"({"msgtype":")" + msgType + R"(","body":")" + body + R"(")";
    if (!formattedBody.empty()) {
        json += R"(,"format":")" + format + R"(","formatted_body":")" + formattedBody + R"(")";
    }
    json += "}";
    return json;
}

// Build image content.
inline std::string buildImageContent(const std::string& body, const std::string& mxcUrl,
    int width = 0, int height = 0, int64_t size = 0, const std::string& mimeType = "")
{
    std::string json = R"({"msgtype":"m.image","body":")" + body + R"(","url":")" + mxcUrl + R"(")";
    if (width > 0 || height > 0 || size > 0 || !mimeType.empty()) {
        json += R"(,"info":{)";
        if (!mimeType.empty()) json += R"("mimetype":")" + mimeType + R"(",)";
        if (width > 0) json += R"("w":)" + std::to_string(width) + ",";
        if (height > 0) json += R"("h":)" + std::to_string(height) + ",";
        if (size > 0) json += R"("size":)" + std::to_string(size);
        if (json.back() == ',') json.pop_back();
        json += "}";
    }
    json += "}";
    return json;
}

// Build file content.
inline std::string buildFileContent(const std::string& body, const std::string& mxcUrl,
    const std::string& fileName = "", int64_t size = 0, const std::string& mimeType = "")
{
    std::string json = R"({"msgtype":"m.file","body":")" + body + R"(","url":")" + mxcUrl + R"(")";
    if (!fileName.empty()) json += R"(,"filename":")" + fileName + R"(")";
    if (size > 0 || !mimeType.empty()) {
        json += R"(,"info":{)";
        if (!mimeType.empty()) json += R"("mimetype":")" + mimeType + R"(",)";
        if (size > 0) json += R"("size":)" + std::to_string(size);
        if (json.back() == ',') json.pop_back();
        json += "}";
    }
    json += "}";
    return json;
}

// Build location content.
inline std::string buildLocationContent(const std::string& body, const std::string& geoUri)
{
    return R"({"msgtype":"m.location","body":")" + body + R"(","geo_uri":")" + geoUri + R"("})";
}

// ==== State Event Content ====

// Build m.room.name content.
inline std::string buildRoomNameContent(const std::string& name) {
    return R"({"name":")" + name + R"("})";
}

// Build m.room.topic content.
inline std::string buildRoomTopicContent(const std::string& topic) {
    return R"({"topic":")" + topic + R"("})";
}

// Build m.room.avatar content.
inline std::string buildRoomAvatarContent(const std::string& mxcUrl) {
    return R"({"url":")" + mxcUrl + R"("})";
}

// Build m.room.member content.
inline std::string buildRoomMemberContent(const std::string& membership,
    const std::string& reason = "", const std::string& displayName = "")
{
    std::string json = R"({"membership":")" + membership + R"(")";
    if (!reason.empty()) json += R"(,"reason":")" + reason + R"(")";
    if (!displayName.empty()) json += R"(,"displayname":")" + displayName + R"(")";
    json += "}";
    return json;
}

// Build m.room.join_rules content.
inline std::string buildJoinRulesContent(const std::string& rule,
    const std::vector<std::string>& allowRoomIds = {})
{
    std::string json = R"({"join_rule":")" + rule + R"(")";
    if (!allowRoomIds.empty()) {
        json += R"(,"allow":[)";
        for (size_t i = 0; i < allowRoomIds.size(); i++) {
            if (i > 0) json += ",";
            json += R"({"type":"m.room_membership","room_id":")" + allowRoomIds[i] + R"("})";
        }
        json += "]";
    }
    json += "}";
    return json;
}

// Build m.room.guest_access content.
inline std::string buildGuestAccessContent(bool canJoin) {
    return R"({"guest_access":")" + std::string(canJoin ? "can_join" : "forbidden") + R"("})";
}

// Build m.room.history_visibility content.
inline std::string buildHistoryVisibilityContent(const std::string& visibility) {
    return R"({"history_visibility":")" + visibility + R"("})";
}

// Build m.room.power_levels content (simplified — user power level only).
inline std::string buildPowerLevelsContent(int usersDefault, int eventsDefault, int stateDefault,
    const std::vector<std::pair<std::string, int>>& userLevels = {})
{
    std::string json = R"({"users_default":)" + std::to_string(usersDefault) +
                       R"(,"events_default":)" + std::to_string(eventsDefault) +
                       R"(,"state_default":)" + std::to_string(stateDefault);
    if (!userLevels.empty()) {
        json += R"(,"users":{)";
        for (size_t i = 0; i < userLevels.size(); i++) {
            if (i > 0) json += ",";
            json += R"(")" + userLevels[i].first + R"(":)" + std::to_string(userLevels[i].second);
        }
        json += "}";
    }
    json += "}";
    return json;
}

// Build m.room.encryption content.
inline std::string buildEncryptionContent(const std::string& algorithm = "m.megolm.v1.aes-sha2",
    int64_t rotationPeriodMs = 604800000, int rotationPeriodMsgs = 100)
{
    return R"({"algorithm":")" + algorithm + R"(","rotation_period_ms":)" +
           std::to_string(rotationPeriodMs) + R"(,"rotation_period_msgs":)" +
           std::to_string(rotationPeriodMsgs) + "}";
}

// Build m.room.tombstone content.
inline std::string buildTombstoneContent(const std::string& body, const std::string& replacementRoomId) {
    return R"({"body":")" + body + R"(","replacement_room":")" + replacementRoomId + R"("})";
}

// ==== Transaction ID Generator ====

// Generate a unique transaction ID for event sending.
// Format: "m" + timestamp + random suffix
// Used to deduplicate sent events.
inline std::string generateTransactionId() {
    static int counter = 0;
    return "m" + std::to_string(time(nullptr)) + std::to_string(++counter);
}

// ==== Poll Content ====

// Build poll start content (m.poll.start).
inline std::string buildPollStartContent(const std::string& question,
    const std::vector<std::string>& options, int maxSelections = 1)
{
    std::string json = R"({"org.matrix.msc3381.poll.start":{"kind":"org.matrix.msc3381.poll.disclosed")";
    json += R"(,"max_selections":)" + std::to_string(maxSelections);
    json += R"(,"question":{"org.matrix.msc1767.text":")" + question + R"("})";
    json += R"(,"answers":[)";
    for (size_t i = 0; i < options.size(); i++) {
        if (i > 0) json += ",";
        json += R"({"id":")" + std::to_string(i + 1) + R"(","org.matrix.msc1767.text":")" + options[i] + R"("})";
    }
    json += "]}";
    json += R"(,"msgtype":"m.text","body":")" + question + R"("})";
    return json;
}

// Build poll response content.
inline std::string buildPollResponseContent(const std::vector<std::string>& selectedOptionIds) {
    std::string json = R"({"org.matrix.msc3381.poll.response":{"answers":[)";
    for (size_t i = 0; i < selectedOptionIds.size(); i++) {
        if (i > 0) json += ",";
        json += R"(")" + selectedOptionIds[i] + R"(")";
    }
    json += "]}";
    json += R"(,"msgtype":"m.text","body":"voted"})";
    return json;
}

// Build poll end content.
inline std::string buildPollEndContent(const std::string& text = "Poll ended") {
    return R"({"org.matrix.msc3381.poll.end":{},"m.text":")" + text +
           R"(","msgtype":"m.text","body":")" + text + R"("})";
}

} // namespace progressive
