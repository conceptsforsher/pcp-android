#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace progressive {

// ---- Event Encryption Utilities ----

struct EncryptionAlgorithm {
    std::string name;              // "m.megolm.v1.aes-sha2", "m.olm.v1.curve25519-aes-sha2"
    bool isMegolm = false;         // group encryption
    bool isOlm = false;            // 1:1 encryption
    bool isDefault = false;
    std::string keySize;           // "256"
    std::string cipher;            // "aes-sha2"
};

struct EncryptedEventHeader {
    std::string algorithm;
    std::string senderKey;
    std::string deviceId;
    std::string sessionId;
    int messageIndex = 0;
    bool valid = false;
};

struct EncryptedContent {
    std::string ciphertext;
    std::string senderKey;
    std::string deviceId;
    std::string sessionId;
    int messageIndex = 0;
};

// Parse encryption algorithm from room state or event.
EncryptionAlgorithm parseEncryptionAlgorithm(const std::string& algorithmStr);

// Parse encrypted event content header.
EncryptedEventHeader parseEncryptedHeader(const std::string& contentJson);

// Extract sender key from encrypted event.
std::string extractSenderKey(const std::string& contentJson);

// Extract session ID from encrypted event.
std::string extractSessionId(const std::string& contentJson);

// Get the list of known encryption algorithms.
std::vector<EncryptionAlgorithm> getKnownAlgorithms();

// Check if an algorithm is considered secure.
bool isSecureAlgorithm(const std::string& algorithm);

// Check if two encrypted events use the same session.
bool isSameSession(const EncryptedEventHeader& a, const EncryptedEventHeader& b);

// Format encryption info for debug display.
std::string formatEncryptionInfo(const EncryptedEventHeader& header);

// ---- Olm/Megolm Session Tracking ----

struct SessionUsage {
    std::string sessionId;
    std::string senderKey;
    int messageCount = 0;
    int firstIndex = 0;
    int lastIndex = 0;
    int missedIndices = 0;         // gaps in message sequence
    int64_t firstSeenMs = 0;
    int64_t lastSeenMs = 0;
    bool isActive = true;
};

// Track session usage across multiple encrypted events.
std::vector<SessionUsage> trackSessionUsage(
    const std::vector<std::string>& sessionIds,
    const std::vector<std::string>& senderKeys,
    const std::vector<int>& messageIndices,
    const std::vector<int64_t>& timestamps
);

// Detect missed message indices in a session.
int detectMissedIndices(const std::vector<int>& indices);

// Check if a session needs key re-request.
bool needsKeyRequest(const SessionUsage& session, int maxMissed = 5);

// Format session usage summary.
std::string formatSessionUsage(const SessionUsage& session);

// ============================================================================
// Event Encryption Models (ported from Kotlin EncryptedEventContent)
// ============================================================================
//
// Original Kotlin (EncryptedEventContent.kt):
//   @JsonClass(generateAdapter = true)
//   data class EncryptedEventContent(
//       @Json(name="algorithm") val algorithm: String?,
//       @Json(name="ciphertext") val ciphertext: String?,
//       @Json(name="sender_key") val senderKey: String?,
//       @Json(name="device_id") val deviceId: String?,
//       @Json(name="session_id") val sessionId: String?
//   )

// ==== EncryptedEventContent ====
//
// Generic encrypted event content structure.
// Used to parse the "content" object of an encrypted Matrix event.
// Both Olm and Megolm encrypted events share this base structure.

struct EncryptedEventContent {
    std::string algorithm;       // "algorithm": "m.megolm.v1.aes-sha2" or "m.olm.v1.curve25519-aes-sha2"
    std::string ciphertext;      // "ciphertext": base64-encoded ciphertext
    std::string senderKey;       // "sender_key": Curve25519 key
    std::string deviceId;        // "device_id": sender device ID
    std::string sessionId;       // "session_id": session identifier
};

// Parse EncryptedEventContent from event content JSON.
// Original Kotlin (EncryptedEventContent deserialized via Moshi):
EncryptedEventContent parseEncryptedEventContent(const std::string& contentJson);

// Build EncryptedEventContent to JSON string.
std::string buildEncryptedEventContent(const EncryptedEventContent& content);

// ==== OlmEncryptedEventContent ====
//
// Original Kotlin (OlmEventContent.kt):
//   Olm-encrypted events have per-device ciphertext maps.
//   {
//     "algorithm": "m.olm.v1.curve25519-aes-sha2",
//     "sender_key": "...",
//     "ciphertext": { "<device_curve25519>": { "type": 0|1, "body": "..." } }
//   }

struct OlmEncryptedEventContent {
    std::string algorithm;
    std::string senderKey;
    std::map<std::string, std::string> ciphertext;  // deviceKey -> ciphertext JSON
};

// Parse OlmEncryptedEventContent from event content JSON.
OlmEncryptedEventContent parseOlmEncryptedContent(const std::string& contentJson);

// Build OlmEncryptedEventContent to JSON string.
std::string buildOlmEncryptedContent(const OlmEncryptedEventContent& content);

// ==== MegolmEncryptedEventContent ====
//
// Original Kotlin (MegolmEventContent):
//   Megolm-encrypted events have a single ciphertext for all room members.
//   {
//     "algorithm": "m.megolm.v1.aes-sha2",
//     "ciphertext": "...",
//     "sender_key": "...",
//     "device_id": "...",
//     "session_id": "..."
//   }

struct MegolmEncryptedEventContent {
    std::string algorithm;       // "m.megolm.v1.aes-sha2"
    std::string ciphertext;      // base64-encoded ciphertext
    std::string senderKey;       // Curve25519 sender key
    std::string deviceId;        // sender device ID
    std::string sessionId;       // Megolm session ID
    int messageIndex = 0;        // "megolm_message_index" or "message_index"
};

// Parse MegolmEncryptedEventContent from event content JSON.
MegolmEncryptedEventContent parseMegolmEncryptedContent(const std::string& contentJson);

// Build MegolmEncryptedEventContent to JSON string.
std::string buildMegolmEncryptedContent(const MegolmEncryptedEventContent& content);

// ==== Encryption Algorithm Detection ====
//
// Original Kotlin (Event.isEncrypted() + MXCRYPTO_ALGORITHM constants):
//   fun isEncrypted() -> checks type or content.algorithm

// Extract the encryption algorithm name from event content JSON.
// Returns empty string if the event is not encrypted.
std::string getEncryptionAlgorithm(const std::string& contentJson);

// Check if event is Olm-encrypted (m.olm.v1.curve25519-aes-sha2).
// Original Kotlin (CryptoConstants.MXCRYPTO_ALGORITHM_OLM):
bool isOlmEncrypted(const std::string& contentJson);

// Check if event is Megolm-encrypted (m.megolm.v1.aes-sha2).
// Original Kotlin (CryptoConstants.MXCRYPTO_ALGORITHM_MEGOLM):
bool isMegolmEncrypted(const std::string& contentJson);

// Check if event content indicates encryption at all.
bool isEncryptedEvent(const std::string& contentJson);

// ============================================================================
// NEW: Encryption Operations & Health
// ============================================================================

// Original Kotlin: EncryptionResult, prepareEncryptionSession, encryptForDevice,
//   decryptEventContent, EncryptionHealth, EncryptionWarning, etc.

// Result of an encryption operation.
// Original Kotlin: EncryptionResult
struct EncryptionResult {
    bool success = false;
    std::string algorithm;
    std::string sessionId;
    std::string senderKey;
    std::string senderDeviceId;
    std::string clearEvent;       // original cleartext event JSON
    std::string errorMessage;     // populated on failure
};

// Result of preparing an encryption session for sending.
// Original Kotlin: EncryptionSessionResult
struct EncryptionSessionResult {
    std::string sessionId;
    std::string deviceId;
    std::string algorithm;
    bool established = false;
};

// Prepare or find an encryption session for a room before sending a message.
// Returns session info or indicates the session is not yet available.
// Original Kotlin: prepareEncryptionSession(roomId)
EncryptionSessionResult prepareEncryptionSession(
    const std::string& roomId,
    const std::string& algorithm
);

// Encrypt a cleartext event for all room members (Megolm group session).
// Returns encrypted event JSON or empty on failure.
// Original Kotlin: encryptEventForRoom(clearEvent, roomId)
EncryptionResult encryptEventForRoom(
    const std::string& clearEventJson,
    const std::string& roomId
);

// Encrypt an event for a single device using Olm (1:1).
// Returns the encrypted event content.
// Original Kotlin: encryptEventForDevice(clearEvent, deviceKey, deviceId)
EncryptionResult encryptEventForDevice(
    const std::string& clearEventJson,
    const std::string& deviceKey,
    const std::string& deviceId
);

// Attempt to decrypt an encrypted event's content.
// Returns the cleartext content JSON, or empty on failure.
// Original Kotlin: decryptEventContent(encryptedContent, sessionId)
std::string decryptEventContent(
    const std::string& encryptedContentJson,
    const std::string& sessionId
);

// Find or create an encryption session required for a room.
// Returns session info including whether a new session was created.
// Original Kotlin: getRequiredEncryptionSession(roomId)
EncryptionSessionResult getRequiredEncryptionSession(
    const std::string& roomId
);

// Health status of room encryption.
// Original Kotlin: EncryptionHealth
enum class EncryptionHealth {
    GOOD = 0,    // all sessions active, no issues
    WARNING = 1, // some sessions stale, minor issues
    ERROR = 2,   // sessions missing, keys missing
    UNKNOWN = 3  // unable to determine state
};

// Check the encryption health status for a room.
// Original Kotlin: checkEncryptionHealth(roomId)
EncryptionHealth checkEncryptionHealth(
    const std::string& roomId,
    const std::string& algorithm
);

// An encryption warning for display to the user.
// Original Kotlin: EncryptionWarning
struct EncryptionWarning {
    std::string code;        // e.g. "UNVERIFIED_SESSION", "STALE_KEY"
    std::string message;     // human-readable description
    int severity = 0;        // 0=info, 1=warning, 2=error
};

// Get a list of active encryption warnings for a room.
// Original Kotlin: getEncryptionWarnings(roomId)
std::vector<EncryptionWarning> getEncryptionWarnings(const std::string& roomId);

// Check if a room requires encryption.
// Returns true if the room state contains an encryption algorithm like "m.megolm.v1.aes-sha2".
// Original Kotlin: isEncryptionRequired(roomId, roomStateJson)
bool isEncryptionRequired(
    const std::string& roomId,
    const std::string& roomStateJson
);

} // namespace progressive
