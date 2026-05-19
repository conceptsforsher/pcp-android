#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ==== Event Redaction Utilities ====
//
// Handles the logic of redacting events and cleaning up redacted content.
// Per Matrix spec, redacted events have content replaced with {}.

// Check if an event has been redacted (unsigned.redacted_because exists).
inline bool isEventRedacted(const std::string& unsignedJson) {
    return unsignedJson.find("\"redacted_because\"") != std::string::npos;
}

// Extract the redaction reason from a redacted event's unsigned data.
// Returns the redaction event ID and reason, or empty.
struct RedactionInfo {
    std::string redactedEventId;    // The event that was redacted
    std::string redactedBy;         // User who sent the redaction
    std::string reason;             // Optional reason for redaction
};

inline RedactionInfo extractRedactionInfo(const std::string& unsignedJson) {
    RedactionInfo info;
    auto pos = unsignedJson.find("\"redacted_because\"");
    if (pos == std::string::npos) return info;

    // Extract the redacted_because event
    pos = unsignedJson.find('{', pos);
    if (pos == std::string::npos) return info;

    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < unsignedJson.size() && depth > 0) {
        if (unsignedJson[pos] == '{') depth++;
        else if (unsignedJson[pos] == '}') depth--;
        pos++;
    }
    std::string redactEvent = unsignedJson.substr(start, pos - start);

    // Extract fields
    auto extract = [&](const std::string& key) -> std::string {
        auto p = redactEvent.find("\"" + key + "\"");
        if (p == std::string::npos) return "";
        p = redactEvent.find(':', p);
        if (p == std::string::npos) return "";
        p++;
        while (p < redactEvent.size() && (redactEvent[p] == ' ' || redactEvent[p] == '\t' || redactEvent[p] == '"')) p++;
        size_t e = p;
        while (e < redactEvent.size() && redactEvent[e] != '"') { if (redactEvent[e] == '\\') e++; e++; }
        return redactEvent.substr(p, e - p);
    };

    info.redactedEventId = extract("event_id");
    info.redactedBy = extract("sender");
    info.reason = extract("reason");
    return info;
}

// ==== Event Encryption Utilities ====

// Check if an event content is encrypted (m.room.encrypted type).
inline bool isEventEncrypted(const std::string& eventType) {
    return eventType == "m.room.encrypted";
}

// Extract encryption metadata from encrypted event content.
struct EncryptionMeta {
    std::string algorithm;       // "m.megolm.v1.aes-sha2"
    std::string ciphertext;      // Encrypted payload
    std::string senderKey;       // Curve25519 key of sender
    std::string deviceId;        // Device ID of sender
    std::string sessionId;       // Megolm session ID
};

inline EncryptionMeta extractEncryptionMeta(const std::string& contentJson) {
    EncryptionMeta meta;
    auto extract = [&](const std::string& key) -> std::string {
        auto pos = contentJson.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = contentJson.find(':', pos);
        if (pos == std::string::npos) return "";
        pos++;
        while (pos < contentJson.size() && (contentJson[pos] == ' ' || contentJson[pos] == '\t' || contentJson[pos] == '"')) pos++;
        size_t end = pos;
        while (end < contentJson.size() && contentJson[end] != '"') { if (contentJson[end] == '\\') end++; end++; }
        return contentJson.substr(pos, end - pos);
    };
    meta.algorithm = extract("algorithm");
    meta.ciphertext = extract("ciphertext");
    meta.senderKey = extract("sender_key");
    meta.deviceId = extract("device_id");
    meta.sessionId = extract("session_id");
    return meta;
}

// ==== Event Age Calculation ====

// Calculate the age of an event in milliseconds.
// age = currentTime - originServerTs
// If unsigned.age is present, use that instead (more accurate).
inline int64_t calculateEventAge(int64_t originServerTs, int64_t currentTimeMs,
    int64_t unsignedAge = -1)
{
    if (unsignedAge >= 0) return unsignedAge;
    return currentTimeMs - originServerTs;
}

// Format event age as human-readable string.
// Returns: "just now", "5m ago", "2h ago", "3d ago", etc.
inline std::string formatEventAge(int64_t ageMs) {
    if (ageMs < 60000) return "just now";
    if (ageMs < 3600000) return std::to_string(ageMs / 60000) + "m ago";
    if (ageMs < 86400000) return std::to_string(ageMs / 3600000) + "h ago";
    if (ageMs < 604800000) return std::to_string(ageMs / 86400000) + "d ago";
    if (ageMs < 2592000000LL) return std::to_string(ageMs / 604800000) + "w ago";
    return std::to_string(ageMs / 2592000000LL) + "mo ago";
}

// ==== Event Display Index ====

// Compute display indices for a list of events.
// Maintains consistent ordering without renumbering everything.
// Uses fractional indexing: new events get indices halfway between neighbors.
inline std::vector<double> computeFractionalIndices(
    double beforeIndex, double afterIndex, int count)
{
    std::vector<double> result;
    if (count <= 0) return result;

    double step = (afterIndex - beforeIndex) / (count + 1);
    for (int i = 0; i < count; i++) {
        result.push_back(beforeIndex + step * (i + 1));
    }
    return result;
}

// ==== Send State Utilities ====

// Matrix send states for local echo tracking.
enum class SendStateEnum {
    UNKNOWN = 0,
    UNSENT = 1,
    SENDING = 2,
    SENT = 3,
    FAILED = 4,
    UNDELIVERABLE = 5
};

inline const char* sendStateToString(SendStateEnum s) {
    switch (s) {
        case SendStateEnum::UNSENT: return "unsent";
        case SendStateEnum::SENDING: return "sending";
        case SendStateEnum::SENT: return "sent";
        case SendStateEnum::FAILED: return "failed";
        case SendStateEnum::UNDELIVERABLE: return "undeliverable";
        default: return "unknown";
    }
}

inline bool isSendStateTerminal(SendStateEnum s) {
    return s == SendStateEnum::SENT || s == SendStateEnum::FAILED ||
           s == SendStateEnum::UNDELIVERABLE;
}

inline bool isSendStateInProgress(SendStateEnum s) {
    return s == SendStateEnum::UNSENT || s == SendStateEnum::SENDING;
}

} // namespace progressive
