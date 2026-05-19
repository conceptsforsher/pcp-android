#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace progressive {

// ---- libolm C++ Wrapper ----
// Wraps the libolm C API (https://gitlab.matrix.org/matrix-org/olm)
// for use by Progressive Chat's Kotlin layer via JNI.
//
// Replaces: org.matrix.android:olm-sdk (removed from Element Android in PR #8901)
//           org.matrix.rustcomponents:crypto-android (current Rust crypto SDK)
//
// libolm provides:
//   - OlmAccount:    identity keypair, one-time key management
//   - OlmSession:    Double Ratchet for 1:1 encrypted messages
//   - OlmInboundGroupSession:  Megolm inbound (receiving group messages)
//   - OlmOutboundGroupSession: Megolm outbound (sending group messages)
//   - OlmSAS:         Short Authentication String (device verification)

// ---- Matrix Crypto Algorithm Constants ----
// Original Kotlin (CryptoConstants.kt:22-42):
//   const val MXCRYPTO_ALGORITHM_OLM = "m.olm.v1.curve25519-aes-sha2"
//   const val MXCRYPTO_ALGORITHM_MEGOLM = "m.megolm.v1.aes-sha2"
//   const val MXCRYPTO_ALGORITHM_MEGOLM_BACKUP = "m.megolm_backup.v1.curve25519-aes-sha2"
//   const val MEGOLM_DEFAULT_ROTATION_MSGS = 100L
//   const val MEGOLM_DEFAULT_ROTATION_PERIOD_MS = 7 * 24 * 3600 * 1000L

namespace CryptoConstants {
    constexpr const char* ALGORITHM_OLM = "m.olm.v1.curve25519-aes-sha2";
    constexpr const char* ALGORITHM_MEGOLM = "m.megolm.v1.aes-sha2";
    constexpr const char* ALGORITHM_MEGOLM_BACKUP = "m.megolm_backup.v1.curve25519-aes-sha2";
    constexpr const char* SSSS_ALGORITHM_AES_HMAC_SHA2 = "m.secret_storage.v1.aes-hmac-sha2";
    constexpr const char* ED25519 = "ed25519";
    constexpr const char* CURVE25519 = "curve25519";
    constexpr int64_t MEGOLM_DEFAULT_ROTATION_MSGS = 100;
    constexpr int64_t MEGOLM_DEFAULT_ROTATION_PERIOD_MS = 7L * 24 * 3600 * 1000;
}

// ---- OLM Error Codes ----
enum class OlmError {
    None = 0,
    NotEnoughRandom,
    OutputBufferTooSmall,
    BadMessageVersion,
    BadMessageFormat,
    BadMessageMac,
    BadMessageKeyId,
    InvalidBase64,
    BadAccountKey,
    UnknownPickleVersion,
    Corruption,
    SessionNotFound,
    UnknownError
};

// ---- OlmAccount (Identity Keypair) ----

struct OlmAccountResult {
    bool success = false;
    OlmError error = OlmError::None;
    std::string data;     // output data (JSON, pickle, etc.)
};

class OlmAccount {
public:
    OlmAccount();
    ~OlmAccount();

    // Create a new account with random keys.
    OlmAccountResult create();

    // Load an account from a pickled (serialized) state.
    OlmAccountResult unpickle(const std::string& key, const std::string& pickle);

    // Save account state to a pickle string.
    OlmAccountResult pickle(const std::string& key);

    // Get the identity keys (Curve25519 + Ed25519) as JSON.
    OlmAccountResult identityKeys();

    // Generate one-time keys (count specifies how many).
    OlmAccountResult generateOneTimeKeys(int count);

    // Get the maximum number of one-time keys.
    int maxOneTimeKeys();

    // Sign a message with the Ed25519 key.
    OlmAccountResult sign(const std::string& message);

    // Get the account's Ed25519 fingerprint key.
    OlmAccountResult ed25519Key();

    // Get the account's Curve25519 identity key.
    OlmAccountResult curve25519Key();

private:
    void* account_;  // OlmAccount*
    friend class OlmSession;
    friend class OlmInboundGroupSession;
    friend class OlmOutboundGroupSession;
};

// ---- OlmSession (1:1 Double Ratchet) ----

struct OlmSessionResult {
    bool success = false;
    OlmError error = OlmError::None;
    std::string data;
    int messageType = 0; // 0 = pre-key, 1 = message
};

class OlmSession {
public:
    OlmSession();
    ~OlmSession();

    // Create an outbound session (Alice side).
    OlmSessionResult createOutbound(OlmAccount& account,
        const std::string& theirIdentityKey, const std::string& theirOneTimeKey);

    // Create an inbound session from a pre-key message (Bob side).
    OlmSessionResult createInbound(OlmAccount& account, const std::string& preKeyMessage);

    // Create an inbound session from a regular message.
    OlmSessionResult createInboundFrom(OlmAccount& account,
        const std::string& theirIdentityKey, const std::string& encryptedMessage);

    // Encrypt a plaintext message.
    OlmSessionResult encrypt(const std::string& plaintext);

    // Decrypt a message.
    OlmSessionResult decrypt(const std::string& encryptedMessage, int messageType);

    // Pickle/unpickle for persistence.
    OlmSessionResult pickle(const std::string& key);
    OlmSessionResult unpickle(const std::string& key, const std::string& pickle);

    // Check if pre-key messages match this session.
    bool matchesInbound(const std::string& preKeyMessage);

private:
    void* session_;  // OlmSession*
};

// ---- Megolm (Group Ratchet) ----

struct MegolmSessionResult {
    bool success = false;
    OlmError error = OlmError::None;
    std::string data;        // encrypted message or decrypted plaintext
    int messageIndex = 0;    // message index in the ratchet
    std::string sessionId;   // unique session identifier
    std::string sessionKey;  // key for sharing
};

struct MegolmInboundResult {
    bool success = false;
    OlmError error = OlmError::None;
    std::string plaintext;
    int messageIndex = 0;
    bool keysProved = false;      // Ed25519 key verified
};

class OlmOutboundGroupSession {
public:
    OlmOutboundGroupSession();
    ~OlmOutboundGroupSession();

    // Create a new outbound Megolm session.
    MegolmSessionResult create();

    // Encrypt a plaintext message (advances ratchet).
    MegolmSessionResult encrypt(const std::string& plaintext);

    // Get the session identifier.
    MegolmSessionResult sessionId();

    // Get the current message index.
    int messageIndex();

    // Get the session key for sharing with new participants.
    MegolmSessionResult sessionKey();

    // Pickle/unpickle.
    MegolmSessionResult pickle(const std::string& key);
    MegolmSessionResult unpickle(const std::string& key, const std::string& pickle);

private:
    void* session_;  // OlmOutboundGroupSession*
};

class OlmInboundGroupSession {
public:
    OlmInboundGroupSession();
    ~OlmInboundGroupSession();

    // Create an inbound session from a shared session key.
    MegolmInboundResult create(const std::string& sessionKey);

    // Decrypt a message.
    MegolmInboundResult decrypt(const std::string& encryptedMessage);

    // Import/export for persistence.
    MegolmSessionResult importSession(const std::string& sessionKey);
    MegolmSessionResult exportSession(const std::string& messageIndex);

    // Pickle/unpickle.
    MegolmSessionResult pickle(const std::string& key);
    MegolmSessionResult unpickle(const std::string& key, const std::string& pickle);

    // Check if this session matches.
    bool isVerified() const;

    // Get the first known index.
    int firstKnownIndex() const;

private:
    void* session_;  // OlmInboundGroupSession*
};

// ---- SAS (Short Authentication String) ----

struct SasResult {
    bool success = false;
    OlmError error = OlmError::None;
    std::string data;         // public key, MAC, or emoji codes
    bool complete = false;
};

class OlmSas {
public:
    OlmSas();
    ~OlmSas();

    // Create a new SAS object.
    SasResult create();

    // Get the public key to send to the other party.
    SasResult getPublicKey();

    // Set the other party's public key.
    SasResult setTheirPublicKey(const std::string& key);

    // Generate bytes for the SAS calculation.
    // extraInfo: "MATRIX_KEY_VERIFICATION_SAS" + sorted identities
    SasResult generateBytes(const std::string& extraInfo, int length = 6);

    // Calculate the MAC for verification.
    SasResult calculateMac(const std::string& input, const std::string& info);

    // Get the emoji codes (7 numbers 0-63) for visual comparison.
    SasResult getEmojiCodes();

    // Check if SAS verification is complete.
    bool isComplete() const;

private:
    void* sas_;  // OlmSAS*
};

// ---- Utility ----

// Generate random bytes using libolm's RNG.
std::string generateRandomBytes(int count);

// Convert OlmError to human-readable string.
std::string olmErrorToString(OlmError error);

// Format a pickled session for storage.
std::string formatPickle(const std::string& type, const std::string& pickle);

// ============================================================================
// OLM Crypto Models (ported from Kotlin Matrix SDK)
// ============================================================================

// ==== OlmDecryptionResult ====
//
// Original Kotlin (OlmDecryptionResult.kt:27-59):
//   @JsonClass(generateAdapter = true)
//   data class OlmDecryptionResult(
//       @Json(name="payload") val payload: JsonDict?,
//       @Json(name="keysClaimed") val keysClaimed: Map<String, String>?,
//       @Json(name="senderKey") val senderKey: String?,
//       @Json(name="forwardingCurve25519KeyChain") val forwardingCurve25519KeyChain: List<String>?,
//       @Json(name="key_safety") val isSafe: Boolean?,
//       @Json(name="verification_state") val verificationState: MessageVerificationState?
//   )
//
// Also ported from MXEventDecryptionResult.kt:33-58:
//   data class MXEventDecryptionResult(
//       clearEvent: JsonDict,
//       senderCurve25519Key: String?,
//       claimedEd25519Key: String?,
//       forwardingCurve25519KeyChain: List<String>,
//       messageVerificationState: MessageVerificationState?
//   )

struct OlmDecryptionResultModel {
    std::string senderKey;                               // "sender_key": curve25519 key
    std::string curve25519Key;                           // explicit curve25519 sender key
    std::string ed25519Key;                              // claimed ed25519 key (aka senderClaimedEd25519Key)
    std::map<std::string, std::string> keysClaimed;      // "keys_claimed": key type → key value
    std::map<std::string, std::string> keysProved;       // "keys_proved": key type → key value
    std::vector<std::string> forwardingCurve25519KeyChain; // "forwarding_curve25519_key_chain"
    std::string payloadJson;                             // "payload": clear event JSON
    bool isSafe = false;                                 // "key_safety"
    int verificationState = 0;                           // "verification_state"
};

// Parse OlmDecryptionResult from JSON.
// Original Kotlin (OlmDecryptionResult deserialized via Moshi):
OlmDecryptionResultModel parseOlmDecryptionResult(const std::string& json);

// Build OlmDecryptionResult to JSON.
std::string buildOlmDecryptionResult(const OlmDecryptionResultModel& result);

// ==== OlmSessionData ====
//
// Original Kotlin: data class representing an active OLM session.
// Contains session identifier, remote key, last activity timestamp, and message count.

struct OlmSessionData {
    std::string sessionId;       // Unique session identifier
    std::string senderKey;       // Remote Curve25519 identity key
    int64_t lastReceivedTs = 0;  // Timestamp (epoch millis) of last received message
    int64_t messageCount = 0;    // Number of messages received in this session
};

// ==== OlmAccountState ====
//
// Original Kotlin: account state values extracted from OlmAccount JSON.
// The olm_account_identity_keys() / olm_account_one_time_keys() JSON produces counts.

struct OlmAccountState {
    int uploadedSignedKeyCount = 0;                          // "uploaded_signed_key_count"
    std::map<std::string, int> oneTimeKeyCounts;             // "one_time_key_counts" : { "curve25519": N, "signed_curve25519": M }
    std::map<std::string, int> fallbackKeyCounts;            // "org.matrix.msc2732.fallback_keys"
};

// Parse OlmAccountState from Matrix /sync device_one_time_keys_count JSON.
OlmAccountState parseOlmAccountState(const std::string& json);

// Build OlmAccountState to JSON.
std::string buildOlmAccountState(const OlmAccountState& state);

// ==== Olm Labs Flags (experimental feature flags) ====
//
// Original Kotlin (various feature toggle objects in RustCryptoService + VectorPreferences):
//   Element Android uses feature flags to control experimental crypto features.
//   These constants mirror the flag names used in the Kotlin layer.

namespace olmLabsFlags {
    // Original Kotlin: "SETTINGS_SECURITY_USE_FLAG_SECURE"
    // Prevents screenshots/screen recording when set.
    constexpr const char* FLAG_SECURE = "SETTINGS_SECURITY_USE_FLAG_SECURE";

    // Original Kotlin: "SETTINGS_CRYPTO_USE_DEVICE_VERIFICATION_V2"
    // Enables new SAS verification flow (emoji/decimal).
    constexpr const char* DEVICE_VERIFICATION_V2 = "SETTINGS_CRYPTO_USE_DEVICE_VERIFICATION_V2";

    // Original Kotlin: "SETTINGS_ENCRYPTION_USE_MEGOLM_BACKUP"
    // Enables encrypted key backup (server-side storage).
    constexpr const char* MEGOLM_BACKUP = "SETTINGS_ENCRYPTION_USE_MEGOLM_BACKUP";

    // Original Kotlin: "SETTINGS_CRYPTO_USE_KEY_FORWARDING"
    // Enables automatic key sharing/forwarding to new devices.
    constexpr const char* KEY_FORWARDING = "SETTINGS_CRYPTO_USE_KEY_FORWARDING";

    // Original Kotlin: "SETTINGS_CRYPTO_USE_CROSS_SIGNING"
    // Enables cross-signing (SSSS-based identity).
    constexpr const char* CROSS_SIGNING = "SETTINGS_CRYPTO_USE_CROSS_SIGNING";

    // Original Kotlin: "SETTINGS_ENCRYPTION_USE_ENCRYPTION_TO_DEVICE_ONLY"
    // Encrypt to verified devices only (default false).
    constexpr const char* ENCRYPT_TO_DEVICE_ONLY = "SETTINGS_ENCRYPTION_USE_ENCRYPTION_TO_DEVICE_ONLY";

    // Original Kotlin: "SETTINGS_CRYPTO_UNSIGNED_PREKEY"
    // MSC3984: support unsigned pre-keys for device identity.
    constexpr const char* UNSIGNED_PREKEY = "SETTINGS_CRYPTO_UNSIGNED_PREKEY";

    // Original Kotlin: "DISPLAY_LIVE_LOCATION_LABS_FLAG_PROMOTION"
    // Live location sharing experimental feature.
    constexpr const char* LIVE_LOCATION = "DISPLAY_LIVE_LOCATION_LABS_FLAG_PROMOTION";

    // Original Kotlin: MSC3061 shared history (key export on invite)
    constexpr const char* SHARED_HISTORY = "SETTINGS_CRYPTO_MSC3061_SHARED_HISTORY";
}

// ==== Room Algorithm Detection ====
//
// Original Kotlin (PrepareToEncryptUseCase.kt:89-91):
//   private fun getEncryptionAlgorithm(roomId: String): String? {
//       return cryptoStore.getRoomAlgorithm(roomId)
//   }
//
// Also (CryptoConstants.kt):
//   const val MXCRYPTO_ALGORITHM_OLM = "m.olm.v1.curve25519-aes-sha2"
//   const val MXCRYPTO_ALGORITHM_MEGOLM = "m.megolm.v1.aes-sha2"
//
// This function checks the `m.room.encryption` state event content
// to determine the encryption algorithm set for a room.

// Get the encryption algorithm for a room from the m.room.encryption event content.
// The `algorithmJson` is the "content" object from m.room.encryption state event.
// Returns the algorithm string (e.g. "m.megolm.v1.aes-sha2"), or empty if none.
std::string getOlmAlgorithmForRoom(const std::string& algorithmJson);

// ============================================================================
// Megolm Session Models
// ============================================================================

// ==== MegolmSessionData ====
//
// Original Kotlin (MegolmSessionData.kt:26-81):
//   internal data class MegolmSessionData(
//       algorithm, sessionId, senderKey, roomId, sessionKey,
//       senderClaimedKeys, senderClaimedEd25519Key,
//       forwardingCurve25519KeyChain, sharedHistory
//   )
//
// Used for importing/exporting megolm session keys (e.g. from key backup).

struct MegolmSessionData {
    std::string algorithm;                              // "algorithm": "m.megolm.v1.aes-sha2"
    std::string sessionId;                              // "session_id": unique session identifier
    std::string senderKey;                              // "sender_key": Curve25519 key of sender
    std::string roomId;                                 // "room_id": room this session is for
    std::string sessionKey;                             // "session_key": base64 key data
    std::map<std::string, std::string> senderClaimedKeys; // "sender_claimed_keys": {"ed25519": "..."}
    std::string senderClaimedEd25519Key;                // "sender_claimed_ed25519_key": shortcut
    std::vector<std::string> forwardingCurve25519KeyChain; // "forwarding_curve25519_key_chain"
    bool sharedHistory = false;                         // "org.matrix.msc3061.shared_history"
};

// Parse MegolmSessionData from JSON.
MegolmSessionData parseMegolmSessionData(const std::string& json);

// Build MegolmSessionData to JSON.
std::string buildMegolmSessionData(const MegolmSessionData& data);

// ==== MegolmInboundSessionInfo ====
//
// Original Kotlin: Derived from MXMegolmSessionData, representing an active
// inbound Megolm session stored in the crypto store.
// Ported from:
//   org.matrix.android.sdk.internal.crypto.store (CryptoStore + RoomKey logic)
//   MegolmSessionData.kt (export/import session format)
//
// Tracks the state of an inbound Megolm ratchet session including sender,
// first known index, whether it's inbound, and backup status.

struct MegolmInboundSessionInfo {
    std::string sessionId;            // Unique session identifier
    std::string senderKey;            // Curve25519 key of the original sender
    std::string ed25519Key;           // Claimed Ed25519 key of the original sender
    std::string roomId;               // Room this session is for
    std::string algorithm;            // "m.megolm.v1.aes-sha2"
    int64_t firstKnownIndex = 0;      // First message index known for this session
    bool isInbound = true;            // Always true for inbound sessions
    bool hasBeenBackedUp = false;     // Whether this session was uploaded to key backup
    std::vector<std::string> forwardingCurve25519KeyChain; // Forwarded key chain
    bool sharedHistory = false;       // MSC3061 shared history flag
    int64_t lastReceivedTs = 0;       // Timestamp of last received message
    int64_t messageCount = 0;         // Number of messages decrypted with this session
};

// Parse MegolmInboundSessionInfo from JSON.
MegolmInboundSessionInfo parseMegolmInboundSessionInfo(const std::string& json);

// Build MegolmInboundSessionInfo to JSON.
std::string buildMegolmInboundSessionInfo(const MegolmInboundSessionInfo& info);

// ==== Key Sharing Logic ====
//
// Original Kotlin (RoomHistoryVisibility.kt:55-56):
//   internal fun RoomHistoryVisibility.shouldShareHistory() =
//       this == RoomHistoryVisibility.WORLD_READABLE || this == RoomHistoryVisibility.SHARED
//
// Determines whether a room's history should be shared (via key forwarding)
// based on the room's history visibility setting.

// Room history visibility values.
enum class RoomHistoryVisibility : int {
    WORLD_READABLE = 0,  // "world_readable"
    SHARED = 1,          // "shared"
    INVITED = 2,         // "invited"
    JOINED = 3           // "joined"
};

// Determine if history should be shared based on visibility.
// Original Kotlin (RoomHistoryVisibility.kt:55-56):
//   fun RoomHistoryVisibility.shouldShareHistory()
inline bool shouldShareHistory(RoomHistoryVisibility visibility) {
    return visibility == RoomHistoryVisibility::WORLD_READABLE
        || visibility == RoomHistoryVisibility::SHARED;
}

// Convert string to RoomHistoryVisibility.
RoomHistoryVisibility parseRoomHistoryVisibility(const std::string& value);

// Reverse: RoomHistoryVisibility to JSON string.
std::string roomHistoryVisibilityToString(RoomHistoryVisibility visibility);

// ==== Session ID Formatting ====
//
// Original Kotlin (MegolmSessionData.kt + room key event handling):
//   Megolm session IDs are base64 strings. When displayed to users,
//   they can be truncated to a human-readable form.

// Format a megolm session ID for human display (first + last N chars).
// Original Kotlin pattern: sessionId.take(12) + "..." + sessionId.takeLast(12)
// With default N=8 for compact display.
std::string formatMegolmSessionId(const std::string& sessionId, int prefixLen = 8);

} // namespace progressive
