#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ExportPhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_export_stateToString(ExportPhase s);
ExportPhase session_export_stateFromString(const std::string& s);

struct SessionExportConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionExportRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionExportResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionExportStatus {
    ExportPhase state = ExportPhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionExportStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session export module with configuration
bool session_export_init(const SessionExportConfig& config);

// Handle a session export request, returning a response
SessionExportResponse session_export_handle(const SessionExportRequest& req);

// Get the current operational status
SessionExportStatus session_export_getStatus();

// Reset the module to initial state
void session_export_reset();

// Validate input before processing
bool session_export_validate(const std::string& input);

// Process input and return result
std::string session_export_process(const std::string& input);

// Serialize to JSON format
json session_export_toJson(const std::string& input);

// Get runtime statistics
SessionExportStats session_export_getStats();

// Build config from JSON
SessionExportConfig session_export_configFromJson(const json& j);

// Serialize config to JSON
json session_export_configToJson(const SessionExportConfig& config);

} // namespace pcp
