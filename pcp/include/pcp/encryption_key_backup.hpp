#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class KeyBackupMode {
    NONE = 0,
    SETUP = 1,
    ACTIVE = 2,
    ROTATING = 3,
    ERROR = 4
};

const char* encryption_key_backup_stateToString(KeyBackupMode s);
KeyBackupMode encryption_key_backup_stateFromString(const std::string& s);

struct EncryptionKeyBackupConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EncryptionKeyBackupRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EncryptionKeyBackupResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EncryptionKeyBackupStatus {
    KeyBackupMode state = KeyBackupMode::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EncryptionKeyBackupStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the encryption key backup module with configuration
bool encryption_key_backup_init(const EncryptionKeyBackupConfig& config);

// Handle a encryption key backup request, returning a response
EncryptionKeyBackupResponse encryption_key_backup_handle(const EncryptionKeyBackupRequest& req);

// Get the current operational status
EncryptionKeyBackupStatus encryption_key_backup_getStatus();

// Reset the module to initial state
void encryption_key_backup_reset();

// Validate input before processing
bool encryption_key_backup_validate(const std::string& input);

// Process input and return result
std::string encryption_key_backup_process(const std::string& input);

// Serialize to JSON format
json encryption_key_backup_toJson(const std::string& input);

// Get runtime statistics
EncryptionKeyBackupStats encryption_key_backup_getStats();

// Build config from JSON
EncryptionKeyBackupConfig encryption_key_backup_configFromJson(const json& j);

// Serialize config to JSON
json encryption_key_backup_configToJson(const EncryptionKeyBackupConfig& config);

} // namespace pcp
