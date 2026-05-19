#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Matrix Event Validation ----

// Original Kotlin: EventValidation, EventContent, MatrixPatterns

struct EventValidation {
    bool valid = false;
    std::string eventId;
    std::string errorMessage;
    std::string warningMessage;  // non-fatal issues

    // Structural checks
    bool hasRequiredFields = false;
    bool hasValidSignature = false;

    // Content checks
    bool messageTooLong = false;
    bool containsInvalidChars = false;
    bool mediaTooLarge = false;

    // Security checks
    bool isSpoofed = false;      // sender doesn't match signature
    bool isExpired = false;      // timestamp too old
    bool isFromBlockedUser = false;
};

struct EventContent {
    std::string eventType;
    std::string msgType;
    std::string body;
    std::string formattedBody;
    std::string url;          // for m.room.avatar etc.
    std::string filename;     // for file uploads
    int64_t fileSize = 0;
    int64_t durationMs = 0;   // for audio/video
    int imgWidth = 0;
    int imgHeight = 0;
};

// Validate a Matrix event for correctness.
EventValidation validateEvent(
    const std::string& eventId,
    const std::string& eventType,
    const std::string& senderId,
    const std::string& contentJson,
    const std::string& originServerTs,
    const std::vector<std::string>& blockedUsers
);

// Validate event content for each message type.
EventContent parseEventContent(const std::string& eventType, const std::string& contentJson);

// Check if a message body is within length limits.
bool isBodyWithinLimits(const std::string& body, int maxLength = 65536);

// Check if a file size is within upload limits.
bool isFileSizeWithinLimits(int64_t fileSize, int64_t maxSizeBytes = 100 * 1024 * 1024);

// Check event id format: $ + base64
bool isValidEventId(const std::string& eventId);

// Validate sender MXID format.
bool isValidSenderId(const std::string& senderId);

// Check if timestamp is reasonable (not too far in past or future).
bool isReasonableTimestamp(const std::string& originServerTs, int64_t maxFutureMs = 300000);

// Format validation result as JSON.
std::string eventValidationToJson(const EventValidation& validation);

// ============================================================================
// NEW: Enhanced Event Validation
// ============================================================================

// Original Kotlin: EventValidationRule, EventValidationResult

// Enumeration of possible validation rules.
// Each rule checks a specific aspect of an event.
enum class EventValidationRule {
    ROOM_ID_REQUIRED = 0,
    SENDER_REQUIRED = 1,
    TYPE_REQUIRED = 2,
    EVENT_ID_REQUIRED = 3,
    CONTENT_REQUIRED = 4,
    STATE_KEY_FOR_STATE = 5,
    TIMESTAMP_NON_NEGATIVE = 6,
    EVENT_ID_FORMAT = 7,
    ROOM_ID_FORMAT = 8,
    USER_ID_FORMAT = 9,
    REDACTION_HAS_REASON = 10
};

// Result of comprehensive validation with per-rule failures.
// Original Kotlin: EventValidationResult
struct EventValidationResult {
    bool isValid = true;
    std::vector<EventValidationRule> ruleFailures;
    std::vector<std::string> warnings;
};

// Comprehensive validation with multiple configurable rules.
// Original Kotlin: validateEvent(eventJson, rules)
EventValidationResult validateEvent(
    const std::string& eventJson,
    const std::vector<EventValidationRule>& rules
);

// Just required fields (eventId, type, sender, roomId, originServerTs, content).
// Original Kotlin: validateEventBasic(eventJson)
EventValidationResult validateEventBasic(const std::string& eventJson);

// All validation rules.
// Original Kotlin: validateEventFull(eventJson)
EventValidationResult validateEventFull(const std::string& eventJson);

// Format all rule failures as human-readable error strings.
// Original Kotlin: getValidationErrors(result)
std::vector<std::string> getValidationErrors(const EventValidationResult& result);

// Regex-free pattern checks for well-formed Matrix identifiers.
// Original Kotlin: MatrixPatterns.isWellFormed*
bool isWellFormedEventId(const std::string& eventId);
bool isWellFormedRoomId(const std::string& roomId);
bool isWellFormedUserId(const std::string& userId);
bool isWellFormedEventType(const std::string& eventType);

// Event integrity checks (signature+hash, stub for NDK).
// Original Kotlin: EventIntegrityChecker
struct EventIntegrityCheck {
    bool passesSignatureCheck = false;
    bool passesHashCheck = false;
    int passingCount = 0;
    int violationCount = 0;
};

EventIntegrityCheck checkEventIntegrity(const std::string& eventJson);

// Specialized validation for specific event types.
// Original Kotlin: validateRedactionEvent, validateStateEvent, validateEncryptedEvent
EventValidationResult validateRedactionEvent(const std::string& eventJson);
EventValidationResult validateStateEvent(const std::string& eventJson);
EventValidationResult validateEncryptedEvent(const std::string& eventJson);

} // namespace progressive
