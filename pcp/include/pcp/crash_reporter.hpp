#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CrashReporterState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* crash_reporter_stateToString(CrashReporterState s);
CrashReporterState crash_reporter_stateFromString(const std::string& s);

struct CrashReporterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CrashReporterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CrashReporterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CrashReporterStatus {
    CrashReporterState state = CrashReporterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CrashReporterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the crash reporter module with configuration
bool crash_reporter_init(const CrashReporterConfig& config);

// Handle a crash reporter request, returning a response
CrashReporterResponse crash_reporter_handle(const CrashReporterRequest& req);

// Get the current operational status
CrashReporterStatus crash_reporter_getStatus();

// Reset the module to initial state
void crash_reporter_reset();

// Validate input before processing
bool crash_reporter_validate(const std::string& input);

// Process input and return result
std::string crash_reporter_process(const std::string& input);

// Serialize to JSON format
json crash_reporter_toJson(const std::string& input);

// Get runtime statistics
CrashReporterStats crash_reporter_getStats();

// Build config from JSON
CrashReporterConfig crash_reporter_configFromJson(const json& j);

// Serialize config to JSON
json crash_reporter_configToJson(const CrashReporterConfig& config);

} // namespace pcp
