#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EncryptedFileState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* encrypted_file_stateToString(EncryptedFileState s);
EncryptedFileState encrypted_file_stateFromString(const std::string& s);

struct EncryptedFileConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EncryptedFileRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EncryptedFileResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EncryptedFileStatus {
    EncryptedFileState state = EncryptedFileState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EncryptedFileStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the encrypted file module with configuration
bool encrypted_file_init(const EncryptedFileConfig& config);

// Handle a encrypted file request, returning a response
EncryptedFileResponse encrypted_file_handle(const EncryptedFileRequest& req);

// Get the current operational status
EncryptedFileStatus encrypted_file_getStatus();

// Reset the module to initial state
void encrypted_file_reset();

// Validate input before processing
bool encrypted_file_validate(const std::string& input);

// Process input and return result
std::string encrypted_file_process(const std::string& input);

// Serialize to JSON format
json encrypted_file_toJson(const std::string& input);

// Get runtime statistics
EncryptedFileStats encrypted_file_getStats();

// Build config from JSON
EncryptedFileConfig encrypted_file_configFromJson(const json& j);

// Serialize config to JSON
json encrypted_file_configToJson(const EncryptedFileConfig& config);

} // namespace pcp
