#ifndef PROGRESSIVE_KEY_BACKUP_HPP
#define PROGRESSIVE_KEY_BACKUP_HPP

#include <string>
#include <vector>
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

struct KeyBackupVersion {
    std::string version;          // backup version identifier
    std::string algorithm;        // "m.megolm_backup.v1.curve25519-aes-sha2"
    std::string authData;         // signed JSON with public key and signatures
    int count = 0;                // number of keys in backup
    bool valid = false;
    std::string error;
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

#endif // PROGRESSIVE_KEY_BACKUP_HPP
