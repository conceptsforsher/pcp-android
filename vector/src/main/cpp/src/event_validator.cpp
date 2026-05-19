#include "progressive/event_validator.hpp"
#include "progressive/json_parser.hpp"
#include "progressive/matrix_patterns.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <unordered_set>

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

// ============================================================================
// NEW: Enhanced Event Validation
// ============================================================================

// ----- Local JSON helpers -----

static std::string localExtractString(const std::string& json, const std::string& key) {
    return parseJsonStringValue(json, key);
}

static int64_t localExtractInt64(const std::string& json, const std::string& key) {
    return parseJsonInt64Value(json, key, 0);
}

static const char* ruleToString(EventValidationRule rule) {
    switch (rule) {
        case EventValidationRule::ROOM_ID_REQUIRED:       return "ROOM_ID_REQUIRED";
        case EventValidationRule::SENDER_REQUIRED:        return "SENDER_REQUIRED";
        case EventValidationRule::TYPE_REQUIRED:          return "TYPE_REQUIRED";
        case EventValidationRule::EVENT_ID_REQUIRED:       return "EVENT_ID_REQUIRED";
        case EventValidationRule::CONTENT_REQUIRED:        return "CONTENT_REQUIRED";
        case EventValidationRule::STATE_KEY_FOR_STATE:     return "STATE_KEY_FOR_STATE";
        case EventValidationRule::TIMESTAMP_NON_NEGATIVE: return "TIMESTAMP_NON_NEGATIVE";
        case EventValidationRule::EVENT_ID_FORMAT:         return "EVENT_ID_FORMAT";
        case EventValidationRule::ROOM_ID_FORMAT:          return "ROOM_ID_FORMAT";
        case EventValidationRule::USER_ID_FORMAT:          return "USER_ID_FORMAT";
        case EventValidationRule::REDACTION_HAS_REASON:    return "REDACTION_HAS_REASON";
    }
    return "UNKNOWN";
}

// ----- Comprehensive Validate Event -----
// Original Kotlin: validateEvent(eventJson, rules)

EventValidationResult validateEvent(
    const std::string& eventJson,
    const std::vector<EventValidationRule>& rules)
{
    EventValidationResult result;

    // Extract fields from JSON
    std::string eventId = localExtractString(eventJson, "event_id");
    std::string roomId = localExtractString(eventJson, "room_id");
    std::string sender = localExtractString(eventJson, "sender");
    std::string type = localExtractString(eventJson, "type");
    int64_t ts = localExtractInt64(eventJson, "origin_server_ts");
    std::string stateKey = localExtractString(eventJson, "state_key");
    std::string redacts = localExtractString(eventJson, "redacts");

    // Check if content exists
    bool hasContent = (eventJson.find("\"content\"") != std::string::npos);

    for (auto rule : rules) {
        switch (rule) {
            case EventValidationRule::ROOM_ID_REQUIRED:
                if (roomId.empty()) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::SENDER_REQUIRED:
                if (sender.empty()) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::TYPE_REQUIRED:
                if (type.empty()) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::EVENT_ID_REQUIRED:
                if (eventId.empty()) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::CONTENT_REQUIRED:
                if (!hasContent) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::STATE_KEY_FOR_STATE:
                if (type.find("m.room.") == 0 && type != "m.room.message" &&
                    type != "m.room.encrypted" && type != "m.room.redaction") {
                    if (stateKey.empty()) {
                        result.ruleFailures.push_back(rule);
                        result.isValid = false;
                    }
                }
                break;
            case EventValidationRule::TIMESTAMP_NON_NEGATIVE:
                if (ts < 0) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::EVENT_ID_FORMAT:
                if (!eventId.empty() && !isWellFormedEventId(eventId)) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::ROOM_ID_FORMAT:
                if (!roomId.empty() && !isWellFormedRoomId(roomId)) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::USER_ID_FORMAT:
                if (!sender.empty() && !isWellFormedUserId(sender)) {
                    result.ruleFailures.push_back(rule);
                    result.isValid = false;
                }
                break;
            case EventValidationRule::REDACTION_HAS_REASON:
                if (!redacts.empty()) {
                    // Look for a reason in the content
                    auto cp = eventJson.find("\"content\"");
                    if (cp != std::string::npos) {
                        auto reason = localExtractString(eventJson.substr(cp), "reason");
                        if (reason.empty()) {
                            result.warnings.push_back("Redaction event has no reason");
                        }
                    }
                }
                break;
        }
    }

    return result;
}

// ----- Basic Validation (required fields only) -----
// Original Kotlin: validateEventBasic(eventJson)

EventValidationResult validateEventBasic(const std::string& eventJson) {
    std::vector<EventValidationRule> rules = {
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::TYPE_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::CONTENT_REQUIRED,
        EventValidationRule::TIMESTAMP_NON_NEGATIVE
    };
    return validateEvent(eventJson, rules);
}

// ----- Full Validation (all rules) -----
// Original Kotlin: validateEventFull(eventJson)

EventValidationResult validateEventFull(const std::string& eventJson) {
    std::vector<EventValidationRule> rules = {
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::TYPE_REQUIRED,
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::CONTENT_REQUIRED,
        EventValidationRule::STATE_KEY_FOR_STATE,
        EventValidationRule::TIMESTAMP_NON_NEGATIVE,
        EventValidationRule::EVENT_ID_FORMAT,
        EventValidationRule::ROOM_ID_FORMAT,
        EventValidationRule::USER_ID_FORMAT,
        EventValidationRule::REDACTION_HAS_REASON
    };
    return validateEvent(eventJson, rules);
}

// ----- Get Validation Errors (human-readable) -----
// Original Kotlin: getValidationErrors(result)

std::vector<std::string> getValidationErrors(const EventValidationResult& result) {
    std::vector<std::string> errors;

    for (auto rule : result.ruleFailures) {
        switch (rule) {
            case EventValidationRule::ROOM_ID_REQUIRED:
                errors.push_back("Room ID is required");
                break;
            case EventValidationRule::SENDER_REQUIRED:
                errors.push_back("Sender is required");
                break;
            case EventValidationRule::TYPE_REQUIRED:
                errors.push_back("Event type is required");
                break;
            case EventValidationRule::EVENT_ID_REQUIRED:
                errors.push_back("Event ID is required");
                break;
            case EventValidationRule::CONTENT_REQUIRED:
                errors.push_back("Event content is required");
                break;
            case EventValidationRule::STATE_KEY_FOR_STATE:
                errors.push_back("State key is required for state events");
                break;
            case EventValidationRule::TIMESTAMP_NON_NEGATIVE:
                errors.push_back("Timestamp must be non-negative");
                break;
            case EventValidationRule::EVENT_ID_FORMAT:
                errors.push_back("Event ID format is invalid (expected $...)");
                break;
            case EventValidationRule::ROOM_ID_FORMAT:
                errors.push_back("Room ID format is invalid (expected !...)");
                break;
            case EventValidationRule::USER_ID_FORMAT:
                errors.push_back("User ID format is invalid (expected @...)");
                break;
            case EventValidationRule::REDACTION_HAS_REASON:
                errors.push_back("Redaction should include a reason");
                break;
        }
    }

    for (const auto& w : result.warnings) {
        errors.push_back("Warning: " + w);
    }

    return errors;
}

// ----- Well-Formed Identifier Checks (regex-free) -----
// Original Kotlin: MatrixPatterns.isWellFormed*

bool isWellFormedEventId(const std::string& eventId) {
    if (eventId.empty()) return false;
    if (eventId[0] != '$') return false;
    if (eventId.size() < 2) return false;
    // No spaces, no control chars, only printable ASCII
    for (char c : eventId) {
        if (c <= 31 || c > 126) return false;
    }
    return true;
}

bool isWellFormedRoomId(const std::string& roomId) {
    if (roomId.empty()) return false;
    if (roomId[0] != '!') return false;
    // Must have at least one colon separating localpart and server
    size_t colon = roomId.find(':', 1);
    if (colon == std::string::npos || colon == roomId.size() - 1) return false;
    for (char c : roomId) {
        if (c <= 31 || c > 126) return false;
    }
    return true;
}

bool isWellFormedUserId(const std::string& userId) {
    if (userId.empty()) return false;
    if (userId[0] != '@') return false;
    size_t colon = userId.find(':', 1);
    if (colon == std::string::npos || colon == userId.size() - 1) return false;
    for (char c : userId) {
        if (c <= 31 || c > 126) return false;
    }
    return true;
}

bool isWellFormedEventType(const std::string& eventType) {
    if (eventType.empty()) return false;
    // Event types look like: m.room.message, org.matrix.msc3381.poll.start
    // Must be at least m.x
    if (eventType.size() < 3) return false;
    // Must contain dots but not start or end with them
    if (eventType[0] == '.' || eventType.back() == '.') return false;
    if (eventType.find('.') == std::string::npos) return false;
    for (char c : eventType) {
        if (c <= 31 || c > 126) return false;
        if (c == ' ') return false;
    }
    return true;
}

// ----- Check Event Integrity (stub) -----
// Original Kotlin: EventIntegrityChecker

EventIntegrityCheck checkEventIntegrity(const std::string& eventJson) {
    EventIntegrityCheck result;

    // Check for hashes
    bool hasHashes = (eventJson.find("\"hashes\"") != std::string::npos);
    bool hasSignature = (eventJson.find("\"signatures\"") != std::string::npos);

    result.passesHashCheck = hasHashes;
    result.passesSignatureCheck = hasSignature;

    if (hasHashes) result.passingCount++;
    if (hasSignature) result.passingCount++;

    if (!hasHashes) result.violationCount++;
    if (!hasSignature) result.violationCount++;

    return result;
}

// ----- Validate Redaction Event -----
// Original Kotlin: validateRedactionEvent(eventJson)

EventValidationResult validateRedactionEvent(const std::string& eventJson) {
    std::vector<EventValidationRule> rules = {
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::EVENT_ID_FORMAT,
        EventValidationRule::USER_ID_FORMAT,
        EventValidationRule::REDACTION_HAS_REASON
    };

    EventValidationResult result = validateEvent(eventJson, rules);

    // Redactions must have a "redacts" field pointing to the target event
    std::string redacts = localExtractString(eventJson, "redacts");
    if (redacts.empty()) {
        result.isValid = false;
        result.ruleFailures.push_back(EventValidationRule::CONTENT_REQUIRED);
        result.warnings.push_back("Redaction must specify event to redact via 'redacts' field");
    }

    return result;
}

// ----- Validate State Event -----
// Original Kotlin: validateStateEvent(eventJson)

EventValidationResult validateStateEvent(const std::string& eventJson) {
    std::vector<EventValidationRule> rules = {
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::TYPE_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::STATE_KEY_FOR_STATE,
        EventValidationRule::CONTENT_REQUIRED,
        EventValidationRule::EVENT_ID_FORMAT,
        EventValidationRule::ROOM_ID_FORMAT,
        EventValidationRule::USER_ID_FORMAT,
        EventValidationRule::TIMESTAMP_NON_NEGATIVE
    };

    EventValidationResult result = validateEvent(eventJson, rules);

    // Ensure the type is a state event type (m.room.* except messages)
    std::string type = localExtractString(eventJson, "type");
    if (type.find("m.room.") != 0 || type == "m.room.message" || type == "m.room.encrypted") {
        result.warnings.push_back("Event type '" + type + "' does not appear to be a state event");
    }

    return result;
}

// ----- Validate Encrypted Event -----
// Original Kotlin: validateEncryptedEvent(eventJson)

EventValidationResult validateEncryptedEvent(const std::string& eventJson) {
    std::vector<EventValidationRule> rules = {
        EventValidationRule::EVENT_ID_REQUIRED,
        EventValidationRule::SENDER_REQUIRED,
        EventValidationRule::ROOM_ID_REQUIRED,
        EventValidationRule::CONTENT_REQUIRED,
        EventValidationRule::EVENT_ID_FORMAT,
        EventValidationRule::ROOM_ID_FORMAT,
        EventValidationRule::USER_ID_FORMAT,
        EventValidationRule::TIMESTAMP_NON_NEGATIVE
    };

    EventValidationResult result = validateEvent(eventJson, rules);

    // Check encryption-related fields
    std::string algorithm = localExtractString(eventJson, "algorithm");
    std::string ciphertext = localExtractString(eventJson, "ciphertext");
    std::string senderKey = localExtractString(eventJson, "sender_key");

    if (algorithm.empty()) {
        result.warnings.push_back("Encrypted event missing algorithm field");
    }
    if (ciphertext.empty()) {
        result.warnings.push_back("Encrypted event missing ciphertext");
    }
    if (senderKey.empty()) {
        result.warnings.push_back("Encrypted event missing sender_key");
    }

    if (algorithm.empty() || ciphertext.empty()) {
        result.isValid = false;
    }

    return result;
}

} // namespace progressive
