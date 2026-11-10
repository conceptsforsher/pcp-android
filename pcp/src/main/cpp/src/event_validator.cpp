#include "pcp/event_validator.hpp"
#include "pcp/json_parser.hpp"
#include "pcp/matrix_patterns.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace pcp {

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



// ============================================================================
// Enhanced Event Validation
// ============================================================================

static std::string extractStrLocal(const std::string& json, const std::string& key) {
    auto search = "\"" + key + "\":\"";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = json.find('"', pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

EventValidationResult validateEvent(
    const std::string& eventJson,
    const std::vector<EventValidationRule>& rules
) {
    EventValidationResult result;
    auto has = [&](const std::string& key) { return eventJson.find("\"" + key + "\"") != std::string::npos; };
    
    for (auto rule : rules) {
        switch (rule) {
            case EventValidationRule::ROOM_ID_REQUIRED:
                if (!has("room_id")) { result.isValid = false; result.ruleFailures.push_back(rule); }
                break;
            case EventValidationRule::SENDER_REQUIRED:
                if (!has("sender")) { result.isValid = false; result.ruleFailures.push_back(rule); }
                break;
            case EventValidationRule::TYPE_REQUIRED:
                if (!has("type")) { result.isValid = false; result.ruleFailures.push_back(rule); }
                break;
            case EventValidationRule::EVENT_ID_REQUIRED:
                if (!has("event_id")) { result.isValid = false; result.ruleFailures.push_back(rule); }
                break;
            case EventValidationRule::CONTENT_REQUIRED:
                if (!has("content")) { result.isValid = false; result.ruleFailures.push_back(rule); }
                break;
            case EventValidationRule::STATE_KEY_FOR_STATE:
                if (eventJson.find("\"type\":\"m.room.") != std::string::npos &&
                    !has("state_key")) {
                    result.isValid = false; result.ruleFailures.push_back(rule);
                }
                break;
            case EventValidationRule::TIMESTAMP_NON_NEGATIVE:
                if (eventJson.find("\"origin_server_ts\":-") != std::string::npos) {
                    result.isValid = false; result.ruleFailures.push_back(rule);
                }
                break;
            case EventValidationRule::EVENT_ID_FORMAT: {
                auto eid = extractStrLocal(eventJson, "event_id");
                if (!eid.empty() && eid[0] != '$') {
                    result.isValid = false; result.ruleFailures.push_back(rule);
                }
                break;
            }
            case EventValidationRule::ROOM_ID_FORMAT: {
                auto rid = extractStrLocal(eventJson, "room_id");
                if (!rid.empty() && rid[0] != '!') {
                    result.isValid = false; result.ruleFailures.push_back(rule);
                }
                break;
            }
            case EventValidationRule::USER_ID_FORMAT: {
                auto uid = extractStrLocal(eventJson, "sender");
                if (!uid.empty() && uid[0] != '@') {
                    result.isValid = false; result.ruleFailures.push_back(rule);
                }
                break;
            }
            case EventValidationRule::REDACTION_HAS_REASON:
                if (has("type") && eventJson.find("\"type\":\"m.room.redaction\"") != std::string::npos && !has("reason")) {
                    result.warnings.push_back("Redaction without reason");
                }
                break;
        }
    }
    return result;
}

EventValidationResult validateEventBasic(const std::string& eventJson) {
    return validateEvent(eventJson, {
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::TYPE_REQUIRED,
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::CONTENT_REQUIRED
    });
}

EventValidationResult validateEventFull(const std::string& eventJson) {
    return validateEvent(eventJson, {
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::TYPE_REQUIRED,
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::CONTENT_REQUIRED,
        EventValidationRule::STATE_KEY_FOR_STATE,
        EventValidationRule::TIMESTAMP_NON_NEGATIVE,
        EventValidationRule::EVENT_ID_FORMAT,
        EventValidationRule::ROOM_ID_FORMAT,
        EventValidationRule::USER_ID_FORMAT
    });
}

std::vector<std::string> getValidationErrors(const EventValidationResult& result) {
    std::vector<std::string> errors;
    for (auto rule : result.ruleFailures) {
        switch (rule) {
            case EventValidationRule::ROOM_ID_REQUIRED: errors.push_back("Missing room_id"); break;
            case EventValidationRule::SENDER_REQUIRED: errors.push_back("Missing sender"); break;
            case EventValidationRule::TYPE_REQUIRED: errors.push_back("Missing type"); break;
            case EventValidationRule::EVENT_ID_REQUIRED: errors.push_back("Missing event_id"); break;
            case EventValidationRule::CONTENT_REQUIRED: errors.push_back("Missing content"); break;
            case EventValidationRule::STATE_KEY_FOR_STATE: errors.push_back("State event missing state_key"); break;
            case EventValidationRule::TIMESTAMP_NON_NEGATIVE: errors.push_back("Negative timestamp"); break;
            case EventValidationRule::EVENT_ID_FORMAT: errors.push_back("Invalid event_id format"); break;
            case EventValidationRule::ROOM_ID_FORMAT: errors.push_back("Invalid room_id format"); break;
            case EventValidationRule::USER_ID_FORMAT: errors.push_back("Invalid user_id format"); break;
            case EventValidationRule::REDACTION_HAS_REASON: break;
        }
    }
    for (const auto& w : result.warnings) errors.push_back(w);
    return errors;
}

bool isWellFormedEventId(const std::string& eventId) {
    return !eventId.empty() && eventId[0] == '$' && eventId.size() > 1;
}

bool isWellFormedRoomId(const std::string& roomId) {
    return !roomId.empty() && roomId[0] == '!' && roomId.find(':') != std::string::npos;
}

bool isWellFormedUserId(const std::string& userId) {
    return !userId.empty() && userId[0] == '@' && userId.find(':') != std::string::npos;
}

bool isWellFormedEventType(const std::string& eventType) {
    if (eventType.empty()) return false;
    size_t dots = 0;
    for (char c : eventType) {
        if (c == '.') dots++;
        if (!isalnum(c) && c != '.' && c != '_' && c != '-') return false;
    }
    return dots >= 1;
}

EventIntegrityCheck checkEventIntegrity(const std::string& eventJson) {
    EventIntegrityCheck check;
    // Basic presence check — actual signature verification requires crypto
    check.passesHashCheck = eventJson.find("\"hashes\"") != std::string::npos;
    check.passesSignatureCheck = eventJson.find("\"signatures\"") != std::string::npos;
    check.passingCount = (check.passesHashCheck ? 1 : 0) + (check.passesSignatureCheck ? 1 : 0);
    check.violationCount = 2 - check.passingCount;
    return check;
}

EventValidationResult validateRedactionEvent(const std::string& eventJson) {
    auto result = validateEventBasic(eventJson);
    if (!eventJson.empty() && eventJson.find("\"type\":\"m.room.redaction\"") != std::string::npos) {
        if (!eventJson.empty() && eventJson.find("\"redacts\"") == std::string::npos) {
            result.isValid = false;
            result.ruleFailures.push_back(EventValidationRule::REDACTION_HAS_REASON);
        }
    }
    return result;
}

EventValidationResult validateStateEvent(const std::string& eventJson) {
    auto result = validateEventBasic(eventJson);
    if (!eventJson.empty() && !eventJson.empty() &&
        eventJson.find("\"state_key\"") == std::string::npos &&
        eventJson.find("\"type\":\"m.room.") != std::string::npos) {
        result.isValid = false;
        result.ruleFailures.push_back(EventValidationRule::STATE_KEY_FOR_STATE);
    }
    return result;
}

EventValidationResult validateEncryptedEvent(const std::string& eventJson) {
    auto result = validateEventBasic(eventJson);
    if (!eventJson.empty() && !eventJson.empty() &&
        eventJson.find("\"type\":\"m.room.encrypted\"") != std::string::npos) {
        // Encrypted events don't need content validation
        result.isValid = true;
        result.ruleFailures.clear();
    }
    return result;
}

} // namespace pcp
