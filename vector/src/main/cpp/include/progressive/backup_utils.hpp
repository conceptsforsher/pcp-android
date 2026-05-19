#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Key Backup Utilities ----

struct BackupInfo {
    std::string version;       // backup version ID
    std::string algorithm;     // "m.megolm_backup.v1.curve25519-aes-sha2"
    std::string authData;      // JSON with public_key, signatures
    int64_t createdAtMs = 0;
    int totalKeys = 0;         // total keys in backup
    int backedUpKeys = 0;      // keys already backed up
    bool verified = false;     // signature verified
    bool trusted = false;      // user trusts this backup
};

struct BackupKeyResult {
    bool success = false;
    std::string sessionId;
    std::string encryptedData;
    std::string error;
};

// Parse backup info from Matrix key backup API response.
BackupInfo parseBackupInfo(const std::string& apiResponseJson);

// Format backup stats for display.
std::string formatBackupStats(const BackupInfo& info);

// Compute backup progress percentage.
double computeBackupProgress(const BackupInfo& info);

// Check if a backup version needs attention (not verified, keys missing).
bool needsBackupAttention(const BackupInfo& info, double minProgress = 100.0);

// Build the request body for creating a new key backup.
std::string buildCreateBackupBody(const std::string& algorithm, const std::string& authData);

// Validate a backup recovery key format (base58, starts with "Es").
bool isValidRecoveryKey(const std::string& key);

// ---- Secret Storage Utilities ----

struct SecretInfo {
    std::string secretId;      // e.g. "m.cross_signing.master"
    std::string encryptedContent; // base64-encoded encrypted secret
    bool found = false;
};

// Parse secret storage default key from account data.
std::string extractDefaultSecretKey(const std::string& accountDataJson);

// Extract a specific secret from secret storage event.
SecretInfo extractSecret(const std::string& secretEventJson, const std::string& secretId);

// Build a secret storage request body for storing a secret.
std::string buildStoreSecretBody(const std::string& secretId, const std::string& encryptedContent);

// Check if cross-signing secrets are stored.
bool hasCrossSigningSecrets(const std::string& accountDataJson);

// Validate and format a backup recovery key.
// Returns JSON: {"valid":bool, "formatted":"EsTj 4fGz...", "error":"..."}
std::string validateAndFormatRecoveryKey(const std::string& rawKey);

// ---- Backup Recovery Key (higher-level, structured) ----
// Original Kotlin (RecoveryKey.kt + BackupRecoveryKey.kt + IBackupRecoveryKey.kt):
//   interface IBackupRecoveryKey { toBase58(), toBase64(), decryptV1(...), megolmV1PublicKey() }
//   class BackupRecoveryKey : IBackupRecoveryKey { fromBase58, fromBase64, fromPassphrase, newFromPassphrase }

struct BackupRecoveryKey {
    std::string keyBase58;               // Base58-encoded recovery key (e.g., "EsTc 2FZd ...")
    std::string keyBytes;                // Raw binary key material (Curve25519 private key, 32 bytes)
    std::string algorithm;               // "m.megolm_backup.v1.curve25519-aes-sha2"
    bool fromPassphrase = false;         // true if derived from passphrase, not random
    std::string passphraseSalt;          // salt used for PBKDF2 (if fromPassphrase)
    int passphraseIterations = 0;        // PBKDF2 iterations (if fromPassphrase)
};

// ---- Recovery Key Info (validation result) ----
// Original Kotlin (RecoveryKey.kt + UI validation in KeysBackupSetupSharedViewModel.kt):
//   Result of validating/parsing a user-entered recovery key.
//   Tracks whether the key is complete, which chars are missing, and its formatted form.

struct RecoveryKeyInfo {
    std::string keyBase58;               // Cleaned-up base58 key (no spaces, uppercase)
    bool isComplete = false;             // true if the key is fully entered and valid
    std::string missingChars;            // placeholder chars for partially-entered keys (e.g., "EsTc ???")
    std::string format;                  // formatted for display: groups of 4 (e.g., "EsTc 2FZd ...")
    bool hasValidChecksum = false;       // true if the parity/checksum check passes
    std::string errorMessage;            // human-readable error if validation fails
};

// Generate a new random recovery key (Curve25519 private key → base58 recovery key).
// Uses a secure random number generator to create the 32-byte key.
// Original Kotlin (RecoveryKey.kt:computeRecoveryKey + random key generation):
//   fun computeRecoveryKey(curve25519Key: ByteArray): String
BackupRecoveryKey generateRecoveryKey();

// Validate a recovery key for format, header bytes, and checksum.
// Returns RecoveryKeyInfo with detailed validation results.
// Works on the parsed BackupRecoveryKey struct.
// Original Kotlin (RecoveryKey.kt:isValidRecoveryKey + extractCurveKeyFromRecoveryKey):
//   fun isValidRecoveryKey(recoveryKey: String?): Boolean
RecoveryKeyInfo validateRecoveryKeyInfo(const BackupRecoveryKey& key);

// Format a recovery key for display: groups of 4 characters separated by spaces.
// e.g., "EsTc2FZdJsdf4Gt7..." → "EsTc 2FZd Jsdf 4Gt7 ..."
// Works on the structured BackupRecoveryKey.
// Original Kotlin (KeysBackupSetupSharedViewModel.kt):
//   fun formatRecoveryKey(raw: String): String { return raw.chunked(4).joinToString(" ") }
std::string formatRecoveryKeyDisplay(const BackupRecoveryKey& key);

// Parse a recovery key from raw user input.
// Handles: removing spaces, converting to uppercase, validating base58 characters,
// extracting the key bytes, checking header and parity.
// Returns a BackupRecoveryKey struct with keyBytes populated if valid.
// Original Kotlin (RecoveryKey.kt:extractCurveKeyFromRecoveryKey):
//   fun extractCurveKeyFromRecoveryKey(recoveryKey: String?): ByteArray?
BackupRecoveryKey parseRecoveryKey(const std::string& input);

} // namespace progressive
