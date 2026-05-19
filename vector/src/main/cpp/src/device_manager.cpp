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

} // namespace progressive
