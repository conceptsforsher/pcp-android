#include "progressive/network_monitor.hpp"
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <cmath>

namespace progressive {

NetworkQuality computeNetworkQuality(
    NetworkType type, bool connected, bool metered, bool roaming,
    int signalStrength, double latencyMs, double lossRate
) {
    NetworkQuality quality;
    quality.type = type;
    quality.isConnected = connected;
    quality.isMetered = metered;
    quality.isRoaming = roaming;
    quality.signalStrength = signalStrength;
    quality.latencyMs = latencyMs;
    quality.packetLossRate = lossRate;
    quality.lastCheckedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    quality.qualityLabel = classifyQualityLabel(signalStrength, latencyMs, lossRate);
    quality.isReliable = quality.qualityLabel != "None" && quality.qualityLabel != "Poor";

    return quality;
}

std::string classifyQualityLabel(int signalStrength, double latencyMs, double lossRate) {
    if (signalStrength == 0 && latencyMs == 0) return "None";

    // Scoring
    int score = 0;

    if (signalStrength >= 75) score += 3;
    else if (signalStrength >= 50) score += 2;
    else if (signalStrength > 0) score += 1;

    if (latencyMs > 0) {
        if (latencyMs < 50) score += 3;
        else if (latencyMs < 150) score += 2;
        else if (latencyMs < 500) score += 1;
    }

    if (lossRate > 0) {
        if (lossRate < 0.01) score += 3;
        else if (lossRate < 0.05) score += 2;
        else if (lossRate < 0.1) score += 1;
    } else {
        score += 3; // no loss measured = assume good
    }

    if (score >= 7) return "Excellent";
    if (score >= 5) return "Good";
    if (score >= 3) return "Fair";
    if (score >= 1) return "Poor";
    return "None";
}

bool isGoodForVoiceCall(const NetworkQuality& quality) {
    if (!quality.isConnected) return false;
    return quality.latencyMs < 300 && quality.packetLossRate < 0.05;
}

bool isGoodForVideoCall(const NetworkQuality& quality) {
    if (!quality.isConnected) return false;
    return quality.latencyMs < 200 && quality.packetLossRate < 0.02 &&
           quality.bandwidthEstimateKbps >= 500;
}

NetworkChange detectNetworkChange(const NetworkQuality& oldState, const NetworkQuality& newState) {
    NetworkChange change;
    change.oldType = oldState.type;
    change.newType = newState.type;
    change.timestampMs = newState.lastCheckedMs;

    if (oldState.isConnected && !newState.isConnected) change.connectivityLost = true;
    if (!oldState.isConnected && newState.isConnected) change.connectivityRestored = true;
    if (!oldState.isMetered && newState.isMetered) change.becameMetered = true;
    if (oldState.isMetered && !newState.isMetered) change.becameUnmetered = true;

    return change;
}

std::string formatNetworkChange(const NetworkChange& change) {
    if (change.connectivityLost) return "Connection lost";
    if (change.connectivityRestored) return "Connection restored";
    if (change.becameMetered) return "Switched to metered network";
    if (change.becameUnmetered) return "Switched to unmetered network";

    std::ostringstream out;
    out << "Network changed";
    return out.str();
}

std::string networkQualityToJson(const NetworkQuality& quality) {
    auto typeStr = [](NetworkType t) -> std::string {
        switch (t) {
            case NetworkType::WIFI:     return "wifi";
            case NetworkType::CELLULAR: return "cellular";
            case NetworkType::ETHERNET: return "ethernet";
            case NetworkType::VPN:      return "vpn";
            case NetworkType::NONE:     return "none";
            default:                    return "unknown";
        }
    };

    std::ostringstream json;
    json << "{";
    json << R"("type": ")" << typeStr(quality.type) << R"(",)";
    json << R"("connected": )" << (quality.isConnected ? "true" : "false") << ",";
    json << R"("metered": )" << (quality.isMetered ? "true" : "false") << ",";
    json << R"("signalStrength": )" << quality.signalStrength << ",";
    json << R"("latencyMs": )" << quality.latencyMs << ",";
    json << R"("qualityLabel": ")" << quality.qualityLabel << R"(")";
    json << "}";
    return json.str();
}

std::string getRecommendedMediaQuality(const NetworkQuality& quality) {
    if (!quality.isConnected) return "offline";
    auto label = quality.qualityLabel;
    if (label == "Excellent" || label == "Good") return "high";
    if (label == "Fair") return "medium";
    return "low";
}

double estimateBandwidthKbps(const std::vector<BandwidthSample>& samples) {
    if (samples.empty()) return 0.0;

    double totalBytes = 0.0;
    int64_t totalMs = 0;

    for (const auto& s : samples) {
        totalBytes += s.bytesTransferred;
        totalMs += s.durationMs;
    }

    if (totalMs <= 0) return 0.0;
    // bytes/ms * 8 bits/byte * 1000 ms/s / 1000 = kbps
    return (totalBytes * 8.0) / (totalMs / 1000.0) / 1000.0;
}

bool isBandwidthSufficient(double bandwidthKbps, double requiredKbps, double margin) {
    return bandwidthKbps >= requiredKbps * (1.0 + margin);
}

// ================================================================
// Network Type/State/String conversions
// ================================================================

const char* networkTypeToString(NetworkType type) {
    switch (type) {
        case NetworkType::WIFI:      return "WIFI";
        case NetworkType::CELLULAR:  return "CELLULAR";
        case NetworkType::ETHERNET:  return "ETHERNET";
        case NetworkType::VPN:       return "VPN";
        case NetworkType::BLUETOOTH: return "BLUETOOTH";
        case NetworkType::NONE:      return "NONE";
        default:                     return "UNKNOWN";
    }
}

const char* networkStateToString(NetworkState state) {
    switch (state) {
        case NetworkState::CONNECTED:    return "CONNECTED";
        case NetworkState::CONNECTING:   return "CONNECTING";
        case NetworkState::DISCONNECTED: return "DISCONNECTED";
        case NetworkState::SUSPENDED:    return "SUSPENDED";
    }
    return "DISCONNECTED";
}

const char* networkQualityLevelToString(NetworkQualityLevel level) {
    switch (level) {
        case NetworkQualityLevel::EXCELLENT: return "EXCELLENT";
        case NetworkQualityLevel::GOOD:      return "GOOD";
        case NetworkQualityLevel::FAIR:      return "FAIR";
        case NetworkQualityLevel::POOR:      return "POOR";
        default:                             return "UNKNOWN";
    }
}

// ================================================================
// Network Quality Estimation
// ================================================================

NetworkQualityLevel estimateNetworkQuality(
    double latencyMs, int signalStrength, double linkSpeedMbps)
{
    if (signalStrength == 0 && latencyMs == 0.0 && linkSpeedMbps == 0.0)
        return NetworkQualityLevel::UNKNOWN;

    int score = 0;

    // Signal strength score
    if (signalStrength >= 75) score += 4;
    else if (signalStrength >= 50) score += 3;
    else if (signalStrength > 0) score += 1;

    // Latency score
    if (latencyMs > 0) {
        if (latencyMs < 30) score += 4;
        else if (latencyMs < 100) score += 3;
        else if (latencyMs < 300) score += 2;
        else if (latencyMs < 600) score += 1;
    }

    // Link speed score
    if (linkSpeedMbps > 0) {
        if (linkSpeedMbps >= 100) score += 4;
        else if (linkSpeedMbps >= 20) score += 3;
        else if (linkSpeedMbps >= 5) score += 2;
        else if (linkSpeedMbps >= 1) score += 1;
    }

    if (score >= 10) return NetworkQualityLevel::EXCELLENT;
    if (score >= 7)  return NetworkQualityLevel::GOOD;
    if (score >= 4)  return NetworkQualityLevel::FAIR;
    if (score >= 1)  return NetworkQualityLevel::POOR;
    return NetworkQualityLevel::UNKNOWN;
}

// ================================================================
// Metered Network Checks
// ================================================================

bool isNetworkMetered(const NetworkInfo& info) {
    if (info.type == NetworkType::CELLULAR) return true;
    // On Android, VPNDatatagramSocket may have metered info
    // VPN connections may be over metered links
    return info.isMetered;
}

bool shouldDeferLargeDownloads(const NetworkInfo& info, int64_t thresholdBytes) {
    if (info.state != NetworkState::CONNECTED) return true;
    if (isNetworkMetered(info)) return true;
    // Poor quality = defer
    auto quality = estimateNetworkQuality(/* latency unknown */ 0.0,
        info.signalStrength, info.linkSpeedMbps);
    if (quality == NetworkQualityLevel::POOR ||
        quality == NetworkQualityLevel::UNKNOWN)
        return true;
    return false;
}

bool checkNetworkRestriction(const NetworkInfo& info, NetworkRestriction restriction) {
    switch (restriction) {
        case NetworkRestriction::NONE:
            return true;
        case NetworkRestriction::METERED_ONLY:
            return isNetworkMetered(info);
        case NetworkRestriction::WIFI_ONLY:
            return info.type == NetworkType::WIFI ||
                   info.type == NetworkType::ETHERNET;
        case NetworkRestriction::UNMETERED_ONLY:
            return !isNetworkMetered(info);
    }
    return true;
}

// ================================================================
// Network Usage Tracking
// ================================================================

void trackNetworkUsage(NetworkUsageTracker& tracker, int64_t bytesUp, int64_t bytesDown) {
    tracker.bytesUp += bytesUp;
    tracker.bytesDown += bytesDown;
    if (tracker.lastResetTs == 0) {
        tracker.lastResetTs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

NetworkUsageTracker getNetworkUsage(const NetworkUsageTracker& tracker) {
    return tracker;
}

void resetNetworkUsage(NetworkUsageTracker& tracker) {
    tracker.bytesUp = 0;
    tracker.bytesDown = 0;
    tracker.lastResetTs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

// ================================================================
// Bandwidth Estimation (full)
// ================================================================

NetworkBandwidthEstimate estimateBandwidth(const std::vector<BandwidthSample>& samples) {
    NetworkBandwidthEstimate est;
    if (samples.empty()) return est;

    double totalBytes = 0.0;
    int64_t totalMs = 0;

    for (const auto& s : samples) {
        totalBytes += s.bytesTransferred;
        totalMs += s.durationMs;
    }

    if (totalMs > 0) {
        double bytesPerSecond = (totalBytes / static_cast<double>(totalMs)) * 1000.0;
        est.downloadBps = bytesPerSecond;
        est.uploadBps = bytesPerSecond * 0.6; // rough estimate
    }

    // Estimate latency from sample timestamps
    if (samples.size() >= 2) {
        int64_t tsFirst = samples.front().timestampMs;
        int64_t tsLast = samples.back().timestampMs;
        if (tsLast > tsFirst && samples.size() > 1) {
            est.latencyMs = static_cast<double>(tsLast - tsFirst) / samples.size();
        }
    }

    return est;
}

// ================================================================
// Network Stats Summary
// ================================================================

NetworkStatsSummary computeNetworkStatsSummary(
    const std::vector<BandwidthSample>& samples,
    const NetworkUsageTracker& usage,
    int connectivityChanges,
    int64_t totalDowntimeMs)
{
    NetworkStatsSummary summary;
    summary.totalBytesUp = usage.bytesUp;
    summary.totalBytesDown = usage.bytesDown;
    summary.totalSamples = static_cast<int64_t>(samples.size());
    summary.connectivityChangesCount = connectivityChanges;
    summary.totalDowntimeMs = totalDowntimeMs;

    if (!samples.empty()) {
        summary.firstSeenTs = samples.front().timestampMs;
        summary.lastSeenTs = samples.back().timestampMs;
    }

    auto est = estimateBandwidth(samples);
    summary.avgDownloadBps = est.downloadBps;
    summary.avgUploadBps = est.uploadBps;
    summary.avgLatencyMs = est.latencyMs;
    summary.avgJitterMs = est.jitterMs;

    return summary;
}

} // namespace progressive
