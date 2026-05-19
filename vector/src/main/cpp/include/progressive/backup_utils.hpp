#ifndef PROGRESSIVE_BACKUP_UTILS_HPP
#define PROGRESSIVE_BACKUP_UTILS_HPP

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

} // namespace progressive

#endif // PROGRESSIVE_BACKUP_UTILS_HPP
