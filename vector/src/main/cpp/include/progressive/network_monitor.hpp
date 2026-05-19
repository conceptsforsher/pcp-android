#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cstdint>

namespace progressive {

// ---- Network Quality Monitor ----

enum class NetworkType {
    WIFI = 0,
    CELLULAR = 1,
    ETHERNET = 2,
    VPN = 3,
    BLUETOOTH = 4,
    UNKNOWN = 5,
    NONE = 6
};

enum class NetworkState {
    CONNECTED = 0,
    CONNECTING = 1,
    DISCONNECTED = 2,
    SUSPENDED = 3
};

enum class NetworkQualityLevel {
    EXCELLENT = 0,
    GOOD = 1,
    FAIR = 2,
    POOR = 3,
    UNKNOWN = 4
};

enum class NetworkRestriction {
    NONE = 0,
    METERED_ONLY = 1,
    WIFI_ONLY = 2,
    UNMETERED_ONLY = 3
};

const char* networkTypeToString(NetworkType type);
const char* networkStateToString(NetworkState state);
const char* networkQualityLevelToString(NetworkQualityLevel level);

using NetworkChangeCallback = std::function<void(/* NetworkInfo */ const struct NetworkInfo&)>;

struct NetworkInfo {
    NetworkType type = NetworkType::UNKNOWN;
    NetworkState state = NetworkState::DISCONNECTED;
    bool isMetered = false;
    bool isRoaming = false;
    int signalStrength = 0;          // 0-100
    double linkSpeedMbps = 0.0;
    std::vector<std::string> dnsServers;
};

struct NetworkQuality {
    NetworkType type = NetworkType::WIFI;
    bool isConnected = false;
    bool isMetered = false;        // cellular or metered wifi
    bool isRoaming = false;
    int signalStrength = 0;        // 0-100
    double bandwidthEstimateKbps = 0.0; // estimated bandwidth
    double latencyMs = 0.0;        // last measured latency
    double packetLossRate = 0.0;    // 0.0-1.0
    int64_t lastCheckedMs = 0;

    // Computed
    bool isReliable = false;       // good enough for calls
    std::string qualityLabel;      // "Excellent", "Good", "Poor", "None"
};

struct NetworkChange {
    NetworkType oldType = NetworkType::WIFI;
    NetworkType newType = NetworkType::WIFI;
    bool connectivityLost = false;
    bool connectivityRestored = false;
    bool becameMetered = false;
    bool becameUnmetered = false;
    int64_t timestampMs = 0;
};

struct NetworkUsageTracker {
    int64_t bytesUp = 0;
    int64_t bytesDown = 0;
    int64_t lastResetTs = 0;
};

struct NetworkBandwidthEstimate {
    double downloadBps = 0.0;
    double uploadBps = 0.0;
    double latencyMs = 0.0;
    double jitterMs = 0.0;
};

struct NetworkStatsSummary {
    int64_t totalBytesUp = 0;
    int64_t totalBytesDown = 0;
    int64_t totalSamples = 0;
    double avgDownloadBps = 0.0;
    double avgUploadBps = 0.0;
    double avgLatencyMs = 0.0;
    double avgJitterMs = 0.0;
    int connectivityChangesCount = 0;
    int64_t totalDowntimeMs = 0;
    int64_t firstSeenTs = 0;
    int64_t lastSeenTs = 0;
};

// Compute network quality from raw metrics.
NetworkQuality computeNetworkQuality(
    NetworkType type, bool connected, bool metered, bool roaming,
    int signalStrength, double latencyMs, double lossRate
);

// Estimate network quality level from latency + signal strength + speed.
NetworkQualityLevel estimateNetworkQuality(
    double latencyMs, int signalStrength, double linkSpeedMbps);

// Classify quality into label.
std::string classifyQualityLabel(int signalStrength, double latencyMs, double lossRate);

// Check if network is good enough for voice calls.
bool isGoodForVoiceCall(const NetworkQuality& quality);

// Check if network is good enough for video calls.
bool isGoodForVideoCall(const NetworkQuality& quality);

// Detect network change between two states.
NetworkChange detectNetworkChange(const NetworkQuality& oldState, const NetworkQuality& newState);

// Format network change for notification.
std::string formatNetworkChange(const NetworkChange& change);

// Format network quality as JSON.
std::string networkQualityToJson(const NetworkQuality& quality);

// Get recommended media quality for current network.
std::string getRecommendedMediaQuality(const NetworkQuality& quality);
// Returns: "high", "medium", "low", "offline"

// ---- Metered Network Checks ----

// Check if the connection is metered (cellular or restricted WiFi).
bool isNetworkMetered(const NetworkInfo& info);

// Check if large downloads should be deferred.
bool shouldDeferLargeDownloads(const NetworkInfo& info, int64_t thresholdBytes = 10485760);

// Evaluate network restriction against current state.
// Returns true if the restriction is satisfied.
bool checkNetworkRestriction(const NetworkInfo& info, NetworkRestriction restriction);

// ---- Network Usage Tracking ----

void trackNetworkUsage(NetworkUsageTracker& tracker, int64_t bytesUp, int64_t bytesDown);
NetworkUsageTracker getNetworkUsage(const NetworkUsageTracker& tracker);
void resetNetworkUsage(NetworkUsageTracker& tracker);

// ---- Bandwidth Estimator ----

struct BandwidthSample {
    int64_t timestampMs = 0;
    int64_t bytesTransferred = 0;
    int64_t durationMs = 0;        // time to transfer
};

// Estimate bandwidth from recent transfer samples.
double estimateBandwidthKbps(const std::vector<BandwidthSample>& samples);

// Check if bandwidth is sufficient for a given bitrate.
bool isBandwidthSufficient(double bandwidthKbps, double requiredKbps, double margin = 0.2);

// Estimate full bandwidth metrics from transfer samples.
NetworkBandwidthEstimate estimateBandwidth(const std::vector<BandwidthSample>& samples);

// Compute aggregate stats summary from samples + usage tracker.
NetworkStatsSummary computeNetworkStatsSummary(
    const std::vector<BandwidthSample>& samples,
    const NetworkUsageTracker& usage,
    int connectivityChanges,
    int64_t totalDowntimeMs);

} // namespace progressive
