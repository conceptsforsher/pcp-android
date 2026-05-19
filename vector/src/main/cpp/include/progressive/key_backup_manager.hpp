#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Key Backup Manager — full E2EE key backup pipeline
//
// Portable C++ implementation inspired by Element Android:
//   RustCryptoService.kt, DefaultKeysBackupService.kt,
//   RecoveryKey.kt, KeysBackup.kt, DefaultSecretStoringManager.kt
//
// Covers the complete backup lifecycle:
//   1. Create backup (createBackupVersion)
//   2. Export session keys for backup (exportRoomKeysForBackup)
//   3. Encrypt session data (encryptSessionData)
//   4. Upload keys to backup (buildUploadRoomsRequest)
//   5. Download keys from backup (parseBackupData/roomKeyBackupFromJson)
//   6. Decrypt backup key (decryptBackupKey with recovery key)
//   7. Decrypt session data (decryptSessionData)
//   8. Verify backup integrity (verifyBackup)
//   9. Delete backup (buildDeleteBackupRequest)
//   10. Trust management (markBackupAsTrusted)
// ================================================================

// ---- Backup Configuration ----

struct KeyBackupConfig {
    std::string algorithm = "m.megolm_backup.v1.curve25519-aes-sha2";
    std::string authData;                   // Signed JSON with public key
    int version = 0;                        // Backup version number
    int64_t createdAt = 0;                  // Unix millis
    int minKeyCount = 1;                    // Minimum keys for a valid backup
    bool forceCreate = false;               // Always create new version
    std::string recoveryKey;                // Recovery key for decrypt
    std::string curve25519Key;              // Curve25519 private key (for encrypt)
};

// ---- Megolm Session Data (for backup) ----

struct MegolmSessionData {
    std::string sessionId;
    std::string roomId;
    std::string senderKey;
    std::string sessionKey;              // Base64-encoded session key
    int64_t firstMessageIndex = 0;
    int64_t forwardedCount = 0;
    bool isForwardedKey = false;
    std::vector<uint8_t> rawData;        // Raw binary session data (decrypted)
};

// ---- Megolm Session Export (for backup) ----

struct MegolmSessionExport {
    std::string roomId;
    std::string senderKey;
    std::string sessionId;
    std::string senderClaimedKeys;       // Ed25519 key
    std::vector<uint8_t> sessionKey;     // Binary Megolm session key
    int64_t firstMessageIndex = 0;
    int64_t forwardedCount = 0;
    bool isForwardedKey = false;
    bool hasKnownIndex = false;
};

// ---- Room Key Backup Data ----

struct RoomKeyBackupData {
    std::string roomId;
    std::string sessionId;               // Base64 of firstMessageIndex + sessionKey
    std::unordered_map<std::string, std::vector<std::string>> sessions;
    // roomId → { session_id1, session_id2, ... }
};

// ---- Backup Version Details ----
// From GET /room_keys/version response

struct BackupVersion {
    std::string version;                 // "1", "2", etc.
    std::string algorithm;               // "m.megolm_backup.v1.curve25519-aes-sha2"
    std::string authData;               // Signed JSON with public key and signatures
    int count = 0;                      // Number of keys in backup
    std::string etag;                   // For conditional requests
    bool valid = false;
    std::string error;
    int64_t versionInt = 0;            // Parsed integer version
};

// ---- Decrypted Session Data ----

struct DecryptedSessionData {
    std::string sessionId;
    std::string senderKey;
    std::string sessionKeyBase64;        // Base64 Megolm session key
    int64_t firstMessageIndex = 0;
    int64_t forwardedCount = 0;
    bool isForwardedKey = false;
    bool decrypted = false;
    std::string error;
};

// ---- Backup Session Result (after decrypting one session) ----

struct BackupSessionResult {
    std::string sessionId;
    std::string roomId;
    std::string senderKey;
    std::string sessionKey;              // The actual Megolm session key
    bool success = false;
    std::string error;
};

// ---- Key Backup Trust Status ----

enum class BackupTrust {
    UNKNOWN = 0,           // Not yet checked
    TRUSTED = 1,           // User verified recovery key
    UNTRUSTED = 2,         // Not verified
    VERIFIED = 3,          // Verified via signature
};

// ---- Backup Progress ----

struct BackupProgress {
    int totalKeys = 0;
    int uploadedKeys = 0;
    int failedKeys = 0;
    int downloadedKeys = 0;
    int decryptedKeys = 0;
    int importedKeys = 0;
    int64_t startedAt = 0;
    int64_t lastUpdateAt = 0;
    bool isRunning = false;
    bool isComplete = false;
    std::string error;
};

// ---- Key Backup Manager ----

class KeyBackupManager {
public:
    KeyBackupManager();

    // ====== Configuration ======

    void setRecoveryKey(const std::string& key);
    void setCurve25519Key(const std::string& key);
    void setBackupVersion(const std::string& version);
    BackupVersion getCurrentVersion() const { return currentVersion_; }
    BackupProgress getProgress() const { return progress_; }

    // ====== Recovery Key Management ======

    // Validate and extract the private key from a recovery key.
    // Returns the Curve25519 private key if valid, empty string otherwise.
    std::string extractPrivateKeyFromRecoveryKey(const std::string& recoveryKey);

    // Generate a new recovery key from a Curve25519 private key.
    std::string generateRecoveryKey(const std::string& curve25519Key);

    // Validate passphrase for key derivation.
    bool validatePassphrase(const std::string& passphrase);

    // ====== Backup Version Management ======

    // Parse backup version info from server response.
    // From GET /room_keys/version → {"version":"1","algorithm":"...","auth_data":"...","count":5}
    BackupVersion parseBackupVersion(const std::string& json);

    // Build request body for creating a new backup version.
    // POST /room_keys/version → {"algorithm":"...","auth_data":"..."}
    std::string buildCreateBackupVersionRequest(const KeyBackupConfig& config);

    // Build request body for updating backup version info.
    // PUT /room_keys/version/{version} → {"algorithm":"...","auth_data":"...","version":"2"}
    std::string buildUpdateBackupVersionRequest(const std::string& version,
                                                 const std::string& authData);

    // Build request to delete a backup version.
    // DELETE /room_keys/version/{version}
    std::string buildDeleteBackupRequest(const std::string& version);

    // ====== Session Key Export ======

    // Export a Megolm session for backup storage.
    // Formats: sender_key, session_id, session_key, message_index, etc.
    MegolmSessionExport exportSessionForBackup(
        const std::string& roomId, const std::string& senderKey,
        const std::string& sessionId, const std::string& sessionKeyBase64,
        int64_t firstMessageIndex, bool isForwardedKey, int64_t forwardedCount);

    // ====== Session Key Encryption for Backup ======

    // Encrypt session data for backup upload.
    // Uses the backup key (Curve25519 public key from auth_data).
    // Returns the encrypted session data as JSON.
    std::string encryptSessionDataForBackup(const MegolmSessionExport& session,
                                             const std::string& authData);

    // Encrypt multiple sessions for batch backup upload.
    std::string encryptSessionsForBackup(const std::vector<MegolmSessionExport>& sessions,
                                          const std::string& authData);

    // ====== Backup Upload ======

    // Build the request body for uploading room keys to backup.
    // PUT /room_keys/keys/{roomId}/{sessionId} → {...encrypted session data...}
    std::string buildUploadRoomKeyRequest(const std::string& roomId,
                                           const std::string& sessionId,
                                           const std::string& encryptedData);

    // Build batch upload request for multiple rooms.
    // PUT /room_keys/keys → {"rooms": {"!room:id": {"sessions": {"sessionId":"...", ...}}}}
    std::string buildBatchUploadRequest(const std::vector<MegolmSessionExport>& sessions);

    // ====== Backup Download ======

    // Parse backup data response from server.
    // From GET /room_keys/keys → {"rooms": {"!room:id": {"sessions": {...}}}}
    // Returns map: roomId → list of encrypted session IDs
    std::vector<RoomKeyBackupData> parseBackupKeysResponse(const std::string& json);

    // Parse a single room's encrypted keys.
    // From GET /room_keys/keys/{roomId} → {"sessions": {...}}
    // Returns list of encrypted session data strings.
    std::vector<std::string> parseRoomBackupKeys(const std::string& json, const std::string& roomId);

    // Parse a single encrypted session.
    // From GET /room_keys/keys/{roomId}/{sessionId} → {encrypted session}
    std::string parseSessionBackupKey(const std::string& json, const std::string& sessionId);

    // ====== Backup Decryption ======

    // Decrypt a backup key (the key that protects the backup) using recovery key.
    // The backup key is encrypted in auth_data.
    // Returns the AES key used for session data encryption.
    std::string decryptBackupKey(const std::string& backupAuthData,
                                  const std::string& recoveryKey);

    // Decrypt a single encrypted session from backup.
    // Takes the encrypted session JSON and the decrypted backup AES key.
    DecryptedSessionData decryptSessionData(const std::string& encryptedSessionJson,
                                              const std::string& backupKey,
                                              const std::string& roomId);

    // Decrypt all sessions from a backup.
    // Returns list of successfully decrypted sessions.
    std::vector<BackupSessionResult> decryptAllSessions(
        const std::string& backupKeysJson,
        const std::string& backupAuthData,
        const std::string& recoveryKey);

    // ====== Backup Verification ======

    // Verify backup integrity: check that auth_data is valid, keys are decryptable.
    bool verifyBackupIntegrity(const std::string& backupAuthData);

    // Verify that a recovery key matches the backup auth_data.
    bool verifyRecoveryKeyMatchesBackup(const std::string& recoveryKey,
                                         const std::string& backupAuthData);

    // ====== Trust Management ======

    // Mark the current backup as trusted.
    void markBackupAsTrusted();

    // Mark the current backup as untrusted.
    void markBackupAsUntrusted();

    // Get backup trust level.
    BackupTrust getTrustLevel() const { return trust_; }

    // Build the request body for updating backup trust.
    // POST /room_keys/version/{version} → mark as verified
    std::string buildMarkBackupAsTrustedRequest();

    // ====== Progress Tracking ======

    void setTotalKeys(int count);
    void advanceUploaded(int count = 1);
    void advanceDownloaded(int count = 1);
    void advanceDecrypted(int count = 1);
    void advanceImported(int count = 1);
    void markComplete();
    void markError(const std::string& error);

    // ====== Serialization ======

    // Export progress as JSON for UI.
    std::string progressToJson() const;

    // Export backup version as JSON.
    std::string backupVersionToJson(const BackupVersion& ver) const;

    // Export all download results as JSON.
    std::string decryptResultsToJson(const std::vector<BackupSessionResult>& results) const;

private:
    BackupVersion currentVersion_;
    BackupProgress progress_;
    BackupTrust trust_ = BackupTrust::UNKNOWN;
    std::string recoveryKey_;
    std::string curve25519Key_;

    // Internal helpers
    std::string generateSessionId(const MegolmSessionExport& session) const;
    std::string extractAuthDataField(const std::string& authData, const std::string& field) const;
    bool validateEncryptionPayload(const std::string& encryptedJson) const;
};

} // namespace progressive
