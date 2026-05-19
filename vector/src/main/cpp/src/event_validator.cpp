#include "progressive/event_validator.hpp"
#include "progressive/json_parser.hpp"
#include "progressive/matrix_patterns.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

bool isValidEventId(const std::string& eventId) {
    return isEventId(eventId);
}

bool isValidSenderId(const std::string& senderId) {
    return isUserId(senderId);
}

bool isReasonableTimestamp(const std::string& originServerTs, int64_t maxFutureMs) {
    if (originServerTs.empty()) return true; // no timestamp = accept

    int64_t ts = std::stoll(originServerTs);
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    // Not more than 5 minutes in the future
    if (ts > now + maxFutureMs) return false;

    // Not more than 10 years in the past
    if (ts < now - 10LL * 365 * 24 * 3600 * 1000) return false;

    return true;
}

bool isBodyWithinLimits(const std::string& body, int maxLength) {
    return static_cast<int>(body.size()) <= maxLength;
}

bool isFileSizeWithinLimits(int64_t fileSize, int64_t maxSizeBytes) {
    return fileSize <= maxSizeBytes;
}

EventContent parseEventContent(const std::string& eventType, const std::string& contentJson) {
    EventContent content;
    content.eventType = eventType;

    content.msgType = parseJsonStringValue(contentJson, "msgtype");
    content.body   = parseJsonStringValue(contentJson, "body");
    content.formattedBody = parseJsonStringValue(contentJson, "formatted_body");
    content.url    = parseJsonStringValue(contentJson, "url");
    content.filename = parseJsonStringValue(contentJson, "filename");

    auto size = parseJsonStringValue(contentJson, "size");
    if (!size.empty()) content.fileSize = std::stoll(size);

    // Parse info block for media
    auto info = parseJsonStringValue(contentJson, "info");
    if (!info.empty()) {
        auto w = parseJsonStringValue("{" + info + "}", "w");
        auto h = parseJsonStringValue("{" + info + "}", "h");
        auto dur = parseJsonStringValue("{" + info + "}", "duration");
        auto fSize = parseJsonStringValue("{" + info + "}", "size");
        if (!w.empty()) content.imgWidth = std::stoi(w);
        if (!h.empty()) content.imgHeight = std::stoi(h);
        if (!dur.empty()) content.durationMs = std::stoll(dur);
        if (!fSize.empty()) content.fileSize = std::stoll(fSize);
    }

    return content;
}

EventValidation validateEvent(
    const std::string& eventId,
    const std::string& eventType,
    const std::string& senderId,
    const std::string& contentJson,
    const std::string& originServerTs,
    const std::vector<std::string>& blockedUsers
) {
    EventValidation result;
    result.eventId = eventId;

    // Structural checks
    if (eventId.empty()) {
        result.errorMessage = "Missing event ID.";
        return result;
    }

    if (!isValidEventId(eventId)) {
        result.errorMessage = "Invalid event ID format. Expected $base64.";
        return result;
    }

    if (senderId.empty()) {
        result.errorMessage = "Missing sender ID.";
        return result;
    }

    if (!isValidSenderId(senderId)) {
        result.errorMessage = "Invalid sender ID format.";
        return result;
    }

    result.hasRequiredFields = true;

    // Timestamp check
    if (!isReasonableTimestamp(originServerTs)) {
        result.isExpired = true;
        result.warningMessage = "Event timestamp is outside acceptable range.";
    }

    // Blocked users
    for (const auto& blocked : blockedUsers) {
        if (senderId == blocked) {
            result.isFromBlockedUser = true;
            result.warningMessage = "Event from user in your block list.";
            break;
        }
    }

    // Content validation
    auto content = parseEventContent(eventType, contentJson);

    if (!isBodyWithinLimits(content.body)) {
        result.messageTooLong = true;
        result.errorMessage = "Message body exceeds maximum length.";
        return result;
    }

    if (content.fileSize > 0 && !isFileSizeWithinLimits(content.fileSize)) {
        result.mediaTooLarge = true;
        result.errorMessage = "Media file exceeds maximum upload size.";
        return result;
    }

    result.valid = true;
    return result;
}

std::string eventValidationToJson(const EventValidation& validation) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"valid": )" << (validation.valid ? "true" : "false");
    json << R"(,"eventId": ")" << esc(validation.eventId) << R"(")";
    if (!validation.errorMessage.empty())
        json << R"(,"errorMessage": ")" << esc(validation.errorMessage) << R"(")";
    if (!validation.warningMessage.empty())
        json << R"(,"warningMessage": ")" << esc(validation.warningMessage) << R"(")";
    json << R"(,"messageTooLong": )" << (validation.messageTooLong ? "true" : "false");
    json << R"(,"isFromBlockedUser": )" << (validation.isFromBlockedUser ? "true" : "false");
    json << "}";
    return json.str();
}

} // namespace progressive
