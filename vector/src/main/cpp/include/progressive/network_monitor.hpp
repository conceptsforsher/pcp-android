#ifndef PROGRESSIVE_NETWORK_MONITOR_HPP
#define PROGRESSIVE_NETWORK_MONITOR_HPP

#include <string>
#include <cstdint>

namespace progressive {

// ---- Network Quality Monitor ----

enum class NetworkType { Wifi, Cellular, Ethernet, Vpn, Unknown, None };

struct NetworkQuality {
    NetworkType type = NetworkType::Unknown;
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
    NetworkType oldType = NetworkType::Unknown;
    NetworkType newType = NetworkType::Unknown;
    bool connectivityLost = false;
    bool connectivityRestored = false;
    bool becameMetered = false;
    bool becameUnmetered = false;
    int64_t timestampMs = 0;
};

// Compute network quality from raw metrics.
NetworkQuality computeNetworkQuality(
    NetworkType type, bool connected, bool metered, bool roaming,
    int signalStrength, double latencyMs, double lossRate
);

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

} // namespace progressive

#endif // PROGRESSIVE_NETWORK_MONITOR_HPP
