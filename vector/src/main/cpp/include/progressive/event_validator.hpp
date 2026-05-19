#ifndef PROGRESSIVE_EVENT_VALIDATOR_HPP
#define PROGRESSIVE_EVENT_VALIDATOR_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Matrix Event Validation ----

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

} // namespace progressive

#endif // PROGRESSIVE_EVENT_VALIDATOR_HPP
