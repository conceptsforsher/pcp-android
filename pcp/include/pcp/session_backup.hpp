#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BackupPhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_backup_stateToString(BackupPhase s);
BackupPhase session_backup_stateFromString(const std::string& s);

struct SessionBackupConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionBackupRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionBackupResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionBackupStatus {
    BackupPhase state = BackupPhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionBackupStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session backup module with configuration
bool session_backup_init(const SessionBackupConfig& config);

// Handle a session backup request, returning a response
SessionBackupResponse session_backup_handle(const SessionBackupRequest& req);

// Get the current operational status
SessionBackupStatus session_backup_getStatus();

// Reset the module to initial state
void session_backup_reset();

// Validate input before processing
bool session_backup_validate(const std::string& input);

// Process input and return result
std::string session_backup_process(const std::string& input);

// Serialize to JSON format
json session_backup_toJson(const std::string& input);

// Get runtime statistics
SessionBackupStats session_backup_getStats();

// Build config from JSON
SessionBackupConfig session_backup_configFromJson(const json& j);

// Serialize config to JSON
json session_backup_configToJson(const SessionBackupConfig& config);

} // namespace pcp
