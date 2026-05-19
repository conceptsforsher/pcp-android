#include "progressive/device_manager.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <regex>

namespace progressive {

DeviceStats parseDeviceList(const std::string& apiResponseJson, const std::string& currentDeviceId) {
    DeviceStats stats;
    int currentIdx = -1;

    // Parse each device object from the array
    size_t pos = 0;
    int devIdx = 0;
    while (true) {
        pos = apiResponseJson.find("\"device_id\"", pos);
        if (pos == std::string::npos) break;

        auto objStart = apiResponseJson.rfind('{', pos);
        if (objStart == std::string::npos) break;

        int depth = 0;
        auto objEnd = objStart;
        while (objEnd < apiResponseJson.size()) {
            if (apiResponseJson[objEnd] == '{') ++depth;
            else if (apiResponseJson[objEnd] == '}') --depth;
            if (depth == 0) break;
            ++objEnd;
        }
        if (objEnd >= apiResponseJson.size()) break;

        std::string obj = apiResponseJson.substr(objStart, objEnd - objStart + 1);

        ManagedDeviceInfo d;
        d.deviceId          = parseJsonStringValue(obj, "device_id");
        d.displayName       = parseJsonStringValue(obj, "display_name");
        d.lastSeenIp        = parseJsonStringValue(obj, "last_seen_ip");

        auto ts = parseJsonStringValue(obj, "last_seen_ts");
        if (!ts.empty()) d.lastSeenTs = std::stoll(ts);

        d.isVerified = obj.find("\"verified\": true") != std::string::npos;
        d.isCurrentDevice = (d.deviceId == currentDeviceId);
        if (d.isCurrentDevice) currentIdx = devIdx;

        // Classify device type
        d.deviceType = classifyDeviceType("", d.displayName);

        d.isInactive = isDeviceInactive(d.lastSeenTs);

        if (!d.deviceId.empty()) {
            stats.devices.push_back(d);
            devIdx++;
        }

        pos = objEnd + 1;
    }

    stats.totalDevices = static_cast<int>(stats.devices.size());
    stats.currentDeviceIndex = currentIdx;

    for (const auto& d : stats.devices) {
        if (d.isVerified) stats.verifiedDevices++;
        else stats.unverifiedDevices++;
        if (d.isInactive) stats.inactiveDevices++;
    }

    return stats;
}

std::string classifyDeviceType(const std::string& userAgent, const std::string& clientName) {
    auto lowerAgent = userAgent;
    auto lowerName = clientName;
    std::transform(lowerAgent.begin(), lowerAgent.end(), lowerAgent.begin(), ::tolower);
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    if (lowerAgent.find("android") != std::string::npos || lowerName.find("android") != std::string::npos)
        return "Mobile";
    if (lowerAgent.find("iphone") != std::string::npos || lowerAgent.find("ios") != std::string::npos)
        return "Mobile";
    if (lowerAgent.find("electron") != std::string::npos || lowerName.find("desktop") != std::string::npos)
        return "Desktop";
    if (lowerAgent.find("mozilla") != std::string::npos || lowerName.find("web") != std::string::npos)
        return "Web";
    return "Unknown";
}

bool isDeviceInactive(int64_t lastSeenMs, int64_t nowMs) {
    if (lastSeenMs <= 0) return false;
    if (nowMs <= 0) {
        nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    int64_t diffMs = nowMs - lastSeenMs;
    return diffMs > 90LL * 24 * 3600 * 1000; // 90 days
}

std::string formatDeviceLastSeen(int64_t lastSeenMs) {
    if (lastSeenMs <= 0) return "Never";
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int64_t diffMs = now - lastSeenMs;
    int days = static_cast<int>(diffMs / (24 * 3600 * 1000));

    if (days < 1) return "Today";
    if (days == 1) return "Yesterday";
    if (days < 30) return std::to_string(days) + " days ago";
    if (days < 365) return std::to_string(days / 30) + " months ago";
    return std::to_string(days / 365) + " years ago";
}

std::string formatDeviceStats(const DeviceStats& stats) {
    std::ostringstream out;
    out << "Device Manager\n";
    out << "==============\n";
    out << "Total: " << stats.totalDevices << " ("
        << stats.verifiedDevices << " verified, "
        << stats.unverifiedDevices << " unverified, "
        << stats.inactiveDevices << " inactive)\n";
    return out.str();
}

std::string managedDeviceInfoToJson(const ManagedDeviceInfo& device) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"deviceId": ")" << esc(device.deviceId) << R"(")";
    json << R"(,"displayName": ")" << esc(device.displayName) << R"(")";
    json << R"(,"deviceType": ")" << esc(device.deviceType) << R"(")";
    json << R"(,"isVerified": )" << (device.isVerified ? "true" : "false");
    json << R"(,"isInactive": )" << (device.isInactive ? "true" : "false");
    json << R"(,"lastSeen": ")" << esc(formatDeviceLastSeen(device.lastSeenTs)) << R"(")";
    json << "}";
    return json.str();
}

std::string deviceListToJson(const DeviceStats& stats) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < stats.devices.size(); ++i) {
        if (i > 0) json << ",";
        json << managedDeviceInfoToJson(stats.devices[i]);
    }
    json << "]";
    return json.str();
}

std::string getDeviceRecommendation(const ManagedDeviceInfo& device) {
    if (device.isCurrentDevice) return "This is your current device.";
    if (!device.isVerified && !device.isInactive) return "Verify this device for secure messaging.";
    if (device.isInactive) return "Sign out from inactive devices you don't use.";
    if (device.isVerified) return "This device is ready for secure messaging.";
    return "No action needed.";
}

void sortDevices(std::vector<ManagedDeviceInfo>& devices, const std::string& sortBy) {
    if (sortBy == "name") {
        std::sort(devices.begin(), devices.end(), [](const auto& a, const auto& b) {
            return a.displayName < b.displayName;
        });
    } else if (sortBy == "lastSeen") {
        std::sort(devices.begin(), devices.end(), [](const auto& a, const auto& b) {
            return a.lastSeenTs > b.lastSeenTs;
        });
    } else { // verification
        std::sort(devices.begin(), devices.end(), [](const auto& a, const auto& b) {
            if (a.isVerified != b.isVerified) return a.isVerified;
            return a.lastSeenTs > b.lastSeenTs;
        });
    }
}

SessionRename validateSessionRename(const std::string& sessionId, const std::string& newName) {
    SessionRename rename;
    rename.sessionId = sessionId;
    rename.newName = newName;

    if (sessionId.empty()) {
        rename.error = "Session ID is required.";
        return rename;
    }
    if (newName.empty() || newName.size() > 100) {
        rename.error = "Name must be 1-100 characters.";
        return rename;
    }
    // Check for valid characters (no control chars)
    for (char c : newName) {
        if (static_cast<unsigned char>(c) < 32) {
            rename.error = "Name contains invalid characters.";
            return rename;
        }
    }

    rename.valid = true;
    return rename;
}

std::string buildSessionRenameBody(const std::string& sessionId, const std::string& newName) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"session_id": ")" + esc(sessionId) + R"(", "display_name": ")" + esc(newName) + R"("})";
}

// ==== Device Crypto (from CryptoManagedDeviceInfo.kt:46-59) ====
std::string extractDeviceFingerprint(const std::string& deviceId, const std::string& keysJson) {
    // Original: keys?.takeIf { deviceId.isNotBlank() }?.get("ed25519:$deviceId")
    if (deviceId.empty()) return "";
    std::string key = "ed25519:" + deviceId;
    auto search = "\"" + key + "\":\"";
    auto pos = keysJson.find(search);
    if (pos == std::string::npos) {
        search = "\"" + key + "\": \"";
        pos = keysJson.find(search);
    }
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = keysJson.find('"', pos);
    return (end != std::string::npos) ? keysJson.substr(pos, end - pos) : "";
}

std::string extractDeviceIdentityKey(const std::string& deviceId, const std::string& keysJson) {
    // Original: keys?.takeIf { deviceId.isNotBlank() }?.get("curve25519:$deviceId")
    if (deviceId.empty()) return "";
    std::string key = "curve25519:" + deviceId;
    auto search = "\"" + key + "\":\"";
    auto pos = keysJson.find(search);
    if (pos == std::string::npos) {
        search = "\"" + key + "\": \"";
        pos = keysJson.find(search);
    }
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = keysJson.find('"', pos);
    return (end != std::string::npos) ? keysJson.substr(pos, end - pos) : "";
}

std::string formatFingerprint(const std::string& fingerprint) {
    // Chunk into groups of 4 for readability
    std::ostringstream out;
    for (size_t i = 0; i < fingerprint.size(); ++i) {
        if (i > 0 && i % 4 == 0) out << ' ';
        out << static_cast<char>(std::toupper(static_cast<unsigned char>(fingerprint[i])));
    }
    return out.str();
}

// ========================================================================
// Expanded: DeviceInfo, DeviceTrustLevel, CryptoDeviceInfo implementations
// Ported from Kotlin models
// ========================================================================

// ---- DeviceInfo (port of Kotlin DeviceInfo.kt) ----

std::string DeviceInfo::bestLastSeenUserAgent() const {
    // Original Kotlin: fun getBestLastSeenUserAgent() = lastSeenUserAgent ?: unstableLastSeenUserAgent
    return !lastSeenUserAgent.empty() ? lastSeenUserAgent : unstableLastSeenUserAgent;
}

// ---- DeviceTrustLevel (port of Kotlin DeviceTrustLevel.kt) ----

bool DeviceTrustLevel::isVerified() const {
    // Original Kotlin: fun isVerified() = crossSigningVerified || locallyVerified == true
    return crossSigningVerified || locallyVerified.value_or(false);
}

bool DeviceTrustLevel::isCrossSigningVerified() const {
    // Original Kotlin: fun isCrossSigningVerified() = crossSigningVerified
    return crossSigningVerified;
}

bool DeviceTrustLevel::isLocallyVerified() const {
    // Original Kotlin: fun isLocallyVerified() = locallyVerified
    return locallyVerified.value_or(false);
}

// ---- CryptoDeviceInfo (port of Kotlin CryptoDeviceInfo.kt) ----

bool CryptoDeviceInfo::isDeviceVerified() const {
    // Original Kotlin: val isVerified: Boolean get() = trustLevel?.isVerified() == true
    return trustLevel.has_value() && trustLevel->isVerified();
}

bool CryptoDeviceInfo::isDeviceCrossSigningVerified() const {
    // Original Kotlin: val isCrossSigningVerified: Boolean get() = trustLevel?.isCrossSigningVerified() == true
    return trustLevel.has_value() && trustLevel->isCrossSigningVerified();
}

bool CryptoDeviceInfo::isDeviceUnknown() const {
    // Original Kotlin: val isUnknown: Boolean get() = trustLevel == null
    return !trustLevel.has_value();
}

std::string CryptoDeviceInfo::fingerprint() const {
    // Original Kotlin: keys?.takeIf { deviceId.isNotBlank() }?.get("ed25519:$deviceId")
    if (deviceId.empty()) return "";
    auto it = keys.find("ed25519:" + deviceId);
    return (it != keys.end()) ? it->second : "";
}

std::string CryptoDeviceInfo::identityKey() const {
    // Original Kotlin: keys?.takeIf { deviceId.isNotBlank() }?.get("curve25519:$deviceId")
    if (deviceId.empty()) return "";
    auto it = keys.find("curve25519:" + deviceId);
    return (it != keys.end()) ? it->second : "";
}

std::string CryptoDeviceInfo::displayName() const {
    // Original Kotlin: fun displayName(): String? { return unsigned?.deviceDisplayName }
    return unsigned.deviceDisplayName;
}

std::string CryptoDeviceInfo::shortDebugString() const {
    // Original Kotlin: fun shortDebugString() = "$userId|$deviceId"
    return userId + "|" + deviceId;
}

// ---- Compute Device Trust Level ----
// Original Kotlin (Device.kt:164-166):
//   suspend fun trustLevel(): DeviceTrustLevel {
//       return DeviceTrustLevel(
//           crossSigningVerified = innerDevice.crossSigningTrusted,
//           locallyVerified = innerDevice.locallyTrusted
//       )
//   }

DeviceTrustLevel computeDeviceTrustLevel(bool crossSigningVerified, std::optional<bool> locallyVerified) {
    DeviceTrustLevel level;
    level.crossSigningVerified = crossSigningVerified;
    level.locallyVerified = locallyVerified;
    return level;
}

// ---- DeviceInfo <-> JSON serialization ----

std::string deviceInfoToJson(const DeviceInfo& info) {
    // Original Kotlin: Moshi JSON serialization of DeviceInfo data class
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << "\"user_id\":\"" << esc(info.userId) << "\"";
    json << ",\"device_id\":\"" << esc(info.deviceId) << "\"";
    json << ",\"display_name\":\"" << esc(info.displayName) << "\"";
    json << ",\"last_seen_ts\":" << info.lastSeenTs;
    if (!info.lastSeenIp.empty())
        json << ",\"last_seen_ip\":\"" << esc(info.lastSeenIp) << "\"";
    if (!info.lastSeenUserAgent.empty())
        json << ",\"last_seen_user_agent\":\"" << esc(info.lastSeenUserAgent) << "\"";
    if (!info.unstableLastSeenUserAgent.empty())
        json << ",\"org.matrix.msc3852.last_seen_user_agent\":\"" << esc(info.unstableLastSeenUserAgent) << "\"";
    json << "}";
    return json.str();
}

DeviceInfo parseDeviceInfo(const std::string& json) {
    // Original Kotlin: Moshi JSON deserialization of DeviceInfo data class
    DeviceInfo info;
    info.userId = parseJsonStringValue(json, "user_id");
    info.deviceId = parseJsonStringValue(json, "device_id");
    info.displayName = parseJsonStringValue(json, "display_name");
    auto ts = parseJsonStringValue(json, "last_seen_ts");
    if (!ts.empty()) info.lastSeenTs = std::stoll(ts);
    info.lastSeenIp = parseJsonStringValue(json, "last_seen_ip");
    info.lastSeenUserAgent = parseJsonStringValue(json, "last_seen_user_agent");
    info.unstableLastSeenUserAgent = parseJsonStringValue(json, "org.matrix.msc3852.last_seen_user_agent");
    return info;
}

// ---- Device Trust Comparison ----
// Original Kotlin sorting logic: cross-signing verified > locally verified > unknown

int compareDeviceTrust(const DeviceTrustLevel& a, const DeviceTrustLevel& b) {
    // Verified takes priority over unverified
    bool aVer = a.isVerified();
    bool bVer = b.isVerified();
    if (aVer != bVer) return aVer ? -1 : 1;

    // Among verified, cross-signing is stronger than local-only
    if (a.crossSigningVerified != b.crossSigningVerified)
        return a.crossSigningVerified ? -1 : 1;

    // If cross-signing equal, compare local verification
    bool aLocal = a.locallyVerified.value_or(false);
    bool bLocal = b.locallyVerified.value_or(false);
    if (aLocal != bLocal) return aLocal ? -1 : 1;

    return 0;
}

// ---- Device Display Name Formatting ----
// Original Kotlin: unsigned?.deviceDisplayName ?: defaultDisplayName

std::string formatDeviceDisplayName(const std::string& defaultName, const std::string& unsignedDisplayName) {
    if (!unsignedDisplayName.empty()) return unsignedDisplayName;
    return defaultName;
}

// ---- CryptoDeviceInfo <-> JSON serialization ----

std::string cryptoDeviceInfoToJson(const CryptoDeviceInfo& info) {
    // Original Kotlin: Moshi JSON serialization of CryptoDeviceInfo
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };
    std::ostringstream json;
    json << "{";
    json << "\"device_id\":\"" << esc(info.deviceId) << "\"";
    json << ",\"user_id\":\"" << esc(info.userId) << "\"";

    // algorithms
    json << ",\"algorithms\":[";
    for (size_t i = 0; i < info.algorithms.size(); ++i) {
        if (i > 0) json << ",";
        json << "\"" << esc(info.algorithms[i]) << "\"";
    }
    json << "]";

    // keys
    json << ",\"keys\":{";
    { bool first = true;
    for (const auto& [k, v] : info.keys) {
        if (!first) json << ",";
        first = false;
        json << "\"" << esc(k) << "\":\"" << esc(v) << "\"";
    }}
    json << "}";

    // signatures
    json << ",\"signatures\":{";
    { bool first = true;
    for (const auto& [userId, sigMap] : info.signatures) {
        if (!first) json << ",";
        first = false;
        json << "\"" << esc(userId) << "\":{";
        bool firstSig = true;
        for (const auto& [k, v] : sigMap) {
            if (!firstSig) json << ",";
            firstSig = false;
            json << "\"" << esc(k) << "\":\"" << esc(v) << "\"";
        }
        json << "}";
    }}
    json << "}";

    // unsigned
    json << ",\"unsigned\":{";
    json << "\"device_display_name\":\"" << esc(info.unsigned.deviceDisplayName) << "\"";
    json << "}";

    // trustLevel
    if (info.trustLevel.has_value()) {
        json << ",\"trust_level\":{";
        json << "\"cross_signing_verified\":" << (info.trustLevel->crossSigningVerified ? "true" : "false");
        if (info.trustLevel->locallyVerified.has_value()) {
            json << ",\"locally_verified\":" << (info.trustLevel->locallyVerified.value() ? "true" : "false");
        }
        json << "}";
    }

    json << ",\"is_blocked\":" << (info.isBlocked ? "true" : "false");
    json << ",\"first_time_seen_local_ts\":" << info.firstTimeSeenLocalTs;
    json << "}";
    return json.str();
}

CryptoDeviceInfo parseCryptoDeviceInfo(const std::string& json) {
    // Original Kotlin: Moshi JSON deserialization of CryptoDeviceInfo
    CryptoDeviceInfo info;
    info.deviceId = parseJsonStringValue(json, "device_id");
    info.userId = parseJsonStringValue(json, "user_id");

    // Parse algorithms array
    auto algPos = json.find("\"algorithms\"");
    if (algPos != std::string::npos) {
        auto arrStart = json.find('[', algPos);
        auto arrEnd = json.find(']', algPos);
        if (arrStart != std::string::npos && arrEnd != std::string::npos && arrStart < arrEnd) {
            std::string arr = json.substr(arrStart + 1, arrEnd - arrStart - 1);
            size_t s = 0;
            while (s < arr.size()) {
                auto quote1 = arr.find('"', s);
                if (quote1 == std::string::npos) break;
                auto quote2 = arr.find('"', quote1 + 1);
                if (quote2 == std::string::npos) break;
                std::string alg = arr.substr(quote1 + 1, quote2 - quote1 - 1);
                if (!alg.empty()) info.algorithms.push_back(alg);
                s = quote2 + 1;
            }
        }
    }

    // Parse display_name from unsigned
    info.unsigned.deviceDisplayName = parseJsonStringValue(json, "device_display_name");

    // Parse is_blocked
    info.isBlocked = json.find("\"is_blocked\":true") != std::string::npos ||
                     json.find("\"is_blocked\": true") != std::string::npos;

    // Parse first_time_seen_local_ts
    auto fts = parseJsonStringValue(json, "first_time_seen_local_ts");
    if (!fts.empty()) info.firstTimeSeenLocalTs = std::stoll(fts);

    return info;
}

} // namespace progressive
