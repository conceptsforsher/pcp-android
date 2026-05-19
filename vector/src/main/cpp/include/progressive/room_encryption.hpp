#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ==== Encryption Algorithm Constants ====
//
// Original Kotlin (RoomEncryption.kt algorithm constants):
//   The Matrix spec defines these algorithm identifiers for E2E room encryption
namespace EncryptionAlgorithm {
    // Original Kotlin: standard Megolm algorithm
    constexpr const char* kMegolmAlgo = "m.megolm.v1.aes-sha2";

    // Original Kotlin: legacy Olm algorithm (1:1 rooms only)
    constexpr const char* kOlmAlgo = "m.olm.v1.curve25519-aes-sha2";

    // Original Kotlin: Megolm v2 (if supported by server)
    constexpr const char* kMegolmV2Algo = "m.megolm.v2.aes-sha2";
}

// ==== RoomEncryptionEventContent ====
//
// Original Kotlin (RoomEncryption.kt: content model for m.room.encryption):
//   data class RoomEncryptionEventContent(
//       @Json(name="algorithm") algorithm: String?,
//       @Json(name="rotation_period_ms") rotationPeriodMs: Long?,
//       @Json(name="rotation_period_msgs") rotationPeriodMsgs: Long?,
//       @Json(name="session_key_bits") sessionKeyBits: Int?
//   )
struct RoomEncryptionEventContent {
    std::string algorithm;               // e.g. "m.megolm.v1.aes-sha2"
    int64_t rotationPeriodMs = 0;        // milliseconds between key rotations, 0 = never
    int64_t rotationPeriodMsgs = 0;      // message count between key rotations, 0 = never
    int sessionKeyBits = 0;              // key size in bits, e.g. 256

    // Original Kotlin: helper to check if any rotation is configured
    bool hasRotation() const {
        return rotationPeriodMs > 0 || rotationPeriodMsgs > 0;
    }

    // Original Kotlin: check if this content is valid (algorithm is set)
    bool isValid() const {
        return !algorithm.empty();
    }
};

// ==== EncryptionState Enum ====
//
// Original Kotlin (RoomEncryption.kt encryption state machine):
//   sealed class EncryptionState {
//       object NotEncrypted : EncryptionState()
//       object Encrypting : EncryptionState()
//       object Encrypted : EncryptionState()
//       object DecryptionError : EncryptionState()
//   }
enum class EncryptionState {
    NOT_ENCRYPTED,       // Room has no m.room.encryption state event
    ENCRYPTING,          // m.room.encryption event exists, but key setup is in progress
    ENCRYPTED,           // Room is fully encrypted and keys are ready
    DECRYPTION_ERROR     // Room is encrypted but decryption has failed (e.g. missing keys)
};

// ---- Room Encryption ----

struct EncryptionConfig {
    std::string algorithm;           // "m.megolm.v1.aes-sha2"
    int rotationPeriodMs = 0;        // 0 = never rotate
    int rotationPeriodMessages = 0;  // 0 = never rotate
    bool enabled = false;

    // Derived
    bool isDefaultAlgorithm = false; // standard megolm
    int64_t nextRotationMs = 0;
};

struct EncryptionStatus {
    bool isEncrypted = false;
    bool isVerified = false;         // all devices verified
    bool hasUnverifiedDevices = false;
    bool hasBlacklistedDevices = false;
    int verifiedCount = 0;
    int unverifiedCount = 0;
    int blacklistedCount = 0;
    int totalDevices = 0;
    std::string algorithm;
    std::string trustLevel;          // "Verified", "Warning", "Unknown"
};

// Parse encryption config from m.room.encryption state event.
EncryptionConfig parseEncryptionConfig(const std::string& stateContentJson);

// Check if a room has encryption enabled.
bool isRoomEncrypted(const std::string& stateContentJson);

// Get the default encryption algorithm.
std::string getDefaultEncryptionAlgorithm();

// Check if an algorithm requires device verification.
bool requiresDeviceVerification(const std::string& algorithm);

// Compute encryption status from device verification states.
EncryptionStatus computeEncryptionStatus(
    const std::string& algorithm,
    const std::vector<bool>& deviceVerified,
    const std::vector<bool>& deviceBlacklisted
);

// Format encryption status as text.
std::string formatEncryptionStatus(const EncryptionStatus& status);

// Format encryption status as a short label for the room header.
std::string formatEncryptionBadge(const EncryptionStatus& status);

// Build the m.room.encryption state event content JSON.
std::string buildEncryptionContent(const EncryptionConfig& config);

// Check if an encryption rotation is due.
bool isRotationDue(const EncryptionConfig& config, int messageCount, int64_t sessionStartMs);

// ==== New Encryption Utilities ====

// Original Kotlin: Build JSON for a complete m.room.encryption state event
// Adds event type wrapper: {"type":"m.room.encryption","content":{...},"state_key":""}
std::string buildEncryptionEvent(const RoomEncryptionEventContent& content);

// Original Kotlin: Parse a full m.room.encryption state event content from JSON
// Reads algorithm, rotation_period_ms, rotation_period_msgs, session_key_bits
RoomEncryptionEventContent parseEncryptionEvent(const std::string& stateContentJson);

// Original Kotlin: Extract just the algorithm name from encryption content
std::string getEncryptionAlgorithm(const std::string& stateContentJson);

// Original Kotlin: Determine whether a session key rotation is due
//   - Checks rotation_period_msgs: if messageCount exceeds the limit
//   - Checks rotation_period_ms: if enough time has elapsed since sessionStartMs
//   - Returns true if either threshold has been met
bool shouldRotateSession(
    const RoomEncryptionEventContent& content,
    int messageCount,
    int64_t sessionStartMs
);

// Original Kotlin: Convert EncryptionState enum to display string
//   NOT_ENCRYPTED → "Not Encrypted", ENCRYPTING → "Encrypting...",
//   ENCRYPTED → "Encrypted", DECRYPTION_ERROR → "Decryption Error"
std::string encryptionStateToString(EncryptionState state);

// Original Kotlin: Determine current EncryptionState from room state
//   If no encryption event: NOT_ENCRYPTED
//   If encryption event but no session keys: ENCRYPTING
//   If encryption event and keys ready: ENCRYPTED
EncryptionState determineEncryptionState(
    const std::string& stateContentJson,
    bool hasSessionKeys
);

// Original Kotlin: Build a rotation recommendation message for the user
//   Returns human-readable string explaining when rotation is needed
std::string formatRotationRecommendation(
    const RoomEncryptionEventContent& content,
    int currentMessageCount,
    int64_t sessionStartMs
);

} // namespace progressive
