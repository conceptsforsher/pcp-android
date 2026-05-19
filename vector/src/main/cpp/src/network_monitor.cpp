#include "progressive/network_monitor.hpp"
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>

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
            case NetworkType::Wifi:     return "wifi";
            case NetworkType::Cellular: return "cellular";
            case NetworkType::Ethernet: return "ethernet";
            case NetworkType::Vpn:      return "vpn";
            case NetworkType::None:     return "none";
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

} // namespace progressive
