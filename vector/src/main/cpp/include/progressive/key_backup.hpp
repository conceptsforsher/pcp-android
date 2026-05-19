#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

#include "progressive/crypto_algorithms.hpp"

namespace progressive {

// ---- Key Backup / Recovery Key Formatter ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.crypto.keysbackup.RecoveryKey.kt (121 lines)
//   org.matrix.android.sdk.internal.crypto.keysbackup.KeysBackup.kt
//   im.vector.app.features.crypto.keysbackup.setup.KeysBackupSetupSharedViewModel.kt
//
// Recovery key format (from MSC1219):
//   [header 0x8B] [header 0x01] [curve25519 key 32B] [parity 1B]
//   → 35 bytes total → base58 encoded → ~58 chars
//   Parity = XOR of all 35 bytes, must equal 0 for valid key

// Recovery key status after parsing
enum class RecoveryKeyStatus {
    Valid,
    Invalid_TooShort,
    Invalid_TooLong,
    Invalid_BadCharacters,     // invalid base58 characters
    Invalid_Checksum,          // checksum mismatch
    Invalid_Format,            // wrong format (not space-separated)
};

struct RecoveryKey {
    std::string raw;                    // base58-encoded key without spaces
    bool valid = false;
    RecoveryKeyStatus status = RecoveryKeyStatus::Valid;
};

// ---- Recovery Key Formatting ----
// Original Kotlin (KeysBackupSetupSharedViewModel.kt):
//   fun formatRecoveryKey(raw: String): String {
//       return raw.chunked(4).joinToString(" ")
//   }
// "EsTc2FZdJsdf4Gt7HqX9bKpLmNvRwQzYx3A5B6C7D8E" → "EsTc 2FZd Jsdf 4Gt7 HqX9 bKpL mNvR wQzY x3A5 B6C7 D8E"

// Format a raw recovery key into 4-character groups separated by spaces.
std::string formatRecoveryKey(const std::string& raw);

// Unformat: remove spaces and uppercase to get raw key.
// "EsTc 2FZd Jsdf" → "EsTc2FZdJsdf"
std::string unformatRecoveryKey(const std::string& formatted);

// Validate a recovery key format.
RecoveryKey validateRecoveryKey(const std::string& key);

// Check if a character is valid in base58 (Bitcoin alphabet).
// Base58 chars: 123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz
bool isValidBase58Char(char c);

// Decode a recovery key to extract the Curve25519 private key.
// Recovery key format: [0x8B][0x01][key 32B][parity 1B] → base58
// Validates header bytes and XOR parity check.
// Returns empty string if invalid.
// Original Kotlin (RecoveryKey.kt:extractCurveKeyFromRecoveryKey):
//   fun extractCurveKeyFromRecoveryKey(recoveryKey: String?): ByteArray?
std::string extractCurveKeyFromRecoveryKey(const std::string& recoveryKey);

// Compute a recovery key from a Curve25519 key (32 bytes).
// Appends header (0x8B, 0x01) and parity byte, then base58 encodes.
// Original Kotlin (RecoveryKey.kt:computeRecoveryKey):
//   fun computeRecoveryKey(curve25519Key: ByteArray): String
std::string computeRecoveryKey(const std::string& curve25519Key);

// Encode binary data to base58 string (via crypto_algorithms.hpp).
// Decode base58 string to binary data (via crypto_algorithms.hpp).

// Validate the checksum of a recovery key.
// The last 4 bytes of the decoded data are a SHA-256 checksum.
// We do a simple length check since actual SHA-256 requires OpenSSL.
bool validateRecoveryKeyChecksum(const std::string& rawKey);

// ---- Backup Version Info ----
// Original Kotlin (KeysVersionResult.kt:25-50):
//   data class KeysVersionResult(
//       @Json(name = "algorithm") override val algorithm: String,
//       @Json(name = "auth_data") override val authData: JsonDict,
//       @Json(name = "version") val version: String,
//       @Json(name = "etag") val hash: String,
//       @Json(name = "count") val count: Int
//   ) : KeysAlgorithmAndData

struct KeyBackupVersion {
    std::string version;          // backup version identifier
    std::string algorithm;        // "m.megolm_backup.v1.curve25519-aes-sha2"
    std::string authData;         // signed JSON with public key and signatures
    std::string etag;             // opaque hash representing stored keys (for conditional requests)
    int count = 0;                // number of keys in backup
    bool valid = false;
    std::string error;
};

// ---- Megolm Backup Session ----
// Original Kotlin (KeyBackupData.kt:28-61):
//   data class KeyBackupData(
//       @Json(name = "first_message_index") val firstMessageIndex: Long,
//       @Json(name = "forwarded_count") val forwardedCount: Int,
//       @ForceToBoolean @Json(name = "is_verified") val isVerified: Boolean,
//       @Json(name = "session_data") val sessionData: JsonDict,
//       @Json(name = "org.matrix.msc3061.shared_history") val sharedHistory: Boolean
//   )

struct KeyBackupSession {
    std::string sessionId;           // backup session identifier (base64 of index+senderKey)
    std::string roomId;              // room where the megolm session is used
    std::string senderKey;           // curve25519 key of the sender
    std::string sessionKey;          // base64-encoded megolm session key
    int64_t firstMessageIndex = 0;   // first message index the session can decrypt
    int64_t forwardedCount = 0;      // number of times key has been forwarded
    bool isVerified = false;         // true if sender was verified at backup time
    std::string algorithm;           // "m.megolm.v1.aes-sha2"
};

// ---- Backup Auth Data ----
// Original Kotlin (MegolmBackupAuthData.kt:30-79):
//   data class MegolmBackupAuthData(
//       @Json(name = "public_key") val publicKey: String,
//       @Json(name = "private_key_salt") val privateKeySalt: String?,
//       @Json(name = "private_key_iterations") val privateKeyIterations: Int?,
//       @Json(name = "signatures") val signatures: Map<String, Map<String, String>>?
//   )

struct KeyBackupAuthData {
    std::string publicKey;           // curve25519 public key for backup encryption
    std::string privateKeySalt;      // salt for passphrase-derived key (optional)
    int privateKeyIterations = 0;    // PBKDF2 iterations (optional, 0 if recovery key)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> signatures;
                                     // userId → (deviceSignKeyId → signature)
    bool fromPassphrase = false;     // true if derived from passphrase not recovery key
};

// ---- Key Backup Room Sessions ----
// Original Kotlin (KeysBackupData.kt:26-30):
//   data class KeysBackupData(
//       @Json(name = "rooms") val roomIdToRoomKeysBackupData: Map<String, RoomKeysBackupData>
//   )

struct KeyBackupRoomSessions {
    std::string roomId;
    std::unordered_map<std::string, KeyBackupSession> sessions; // sessionId → KeyBackupSession
};

// Parse key backup version info from JSON (from GET /room_keys/version).
// Original Kotlin (KeysBackup.kt):
//   data class KeysBackupVersion(
//       @Json(name = "version") val version: String,
//       @Json(name = "algorithm") val algorithm: String,
//       @Json(name = "auth_data") val authData: JsonObject,
//       @Json(name = "count") val count: Int
//   )
KeyBackupVersion parseKeyBackupVersion(const std::string& json);

// Check if the backup algorithm is supported.
// Currently supports: m.megolm_backup.v1.curve25519-aes-sha2
bool isSupportedBackupAlgorithm(const std::string& algorithm);

// Format key backup info as JSON for the Kotlin UI.
std::string keyBackupVersionToJson(const KeyBackupVersion& backup);

// Get a human-readable description of the backup algorithm.
std::string getBackupAlgorithmDescription(const std::string& algorithm);

// Generate a recovery key suggestion (for display to user).
// NOT a real key — just a formatted example.
std::string getRecoveryKeyExample();

// Validate a passphrase: must be non-empty, min length recommended.
bool isValidPassphrase(const std::string& passphrase);

// Get the minimum recommended passphrase length.
int getMinPassphraseLength();

// ---- Key Backup HTTP API Builders / Parsers ----

// Build request body for creating a new key backup version.
// POST /room_keys/version
// Original Kotlin (CreateKeysBackupVersionBody.kt:24-37):
//   data class CreateKeysBackupVersionBody(
//       @Json(name = "algorithm") override val algorithm: String,
//       @Json(name = "auth_data") override val authData: JsonDict
//   )
std::string buildKeyBackupCreateRequest(const std::string& algorithm, const std::string& authDataJson);

// Build request body for uploading room keys to a backup version.
// PUT /room_keys/keys/{roomId}/{sessionId}
// Original Kotlin (KeyBackupData.kt + session_data):
//   { "rooms": { "!room:id": { "sessions": { "sessionId": { sessionData } } } } }
std::string buildKeyBackupUploadRequest(const std::string& roomId, const std::string& sessionId,
                                         const std::string& sessionDataJson);

// Parse download response from GET /room_keys/keys.
// Returns a vector of KeyBackupRoomSessions.
// Original Kotlin (KeysBackupData.kt + RoomKeysBackupData):
//   { "rooms": { "!room:id": { "sessions": { "sessionId": { ... } } } } }
std::vector<KeyBackupRoomSessions> parseKeyBackupDownloadResponse(const std::string& responseJson);

// Compute a version string for a key backup.
// For new backups starting at version 0: "0"
// Increment on each version change.
// Original Kotlin (KeysBackup.kt):
//   fun computeBackupVersion(currentVersion: String?): String
std::string computeKeyBackupVersion(const std::string& currentVersion);

// Validate that a key backup is usable.
// Checks: version non-empty, algorithm supported, auth_data present and well-formed.
// Original Kotlin (KeysBackupVersionTrust.kt + KeysBackup.kt):
//   fun isKeyBackupValid(keysVersionResult: KeysVersionResult): Boolean
bool isKeyBackupValid(const KeyBackupVersion& backup);

// ---- Key Backup Auth Data Builders / Parsers ----

// Build a KeyBackupAuthData JSON string from a struct.
// Result: {"public_key":"...","signatures":{...}} (plus optional passphrase fields)
std::string buildKeyBackupAuthData(const KeyBackupAuthData& authData);

// Parse auth_data JSON string into a KeyBackupAuthData struct.
// Handles both recovery-key and passphrase-based auth data.
KeyBackupAuthData parseKeyBackupAuthData(const std::string& authDataJson);

// ---- Secure Storage Key (4S/SSSS) from SecretStorageKeyContent.kt (103L) ----
struct KeyBackupPassphrase {
    std::string algorithm;
    int iterations = 500000;
    std::string salt;
};

struct SecretStorageKey {
    std::string keyId;
    std::string algorithm;
    std::string name;
    std::string publicKey;
    KeyBackupPassphrase passphrase;
    bool valid = false;
    bool hasPassphrase() const { return !passphrase.salt.empty(); }
};

SecretStorageKey parseSecretStorageKey(const std::string& keyId, const std::string& json);
std::string secretStorageKeyToJson(const SecretStorageKey& key);

} // namespace progressive
