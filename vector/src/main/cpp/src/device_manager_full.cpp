#include "progressive/device_manager_full.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

namespace progressive {

// ====== JSON helpers ======

std::string DeviceManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int64_t DeviceManager::extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

bool DeviceManager::extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

DeviceManager::DeviceManager() {}

// ====== Device List Parsing ======
// Original: DevicesListResponse.kt — {"devices":[{"device_id":"...","display_name":"...",...}]}

DevicesListResponse DeviceManager::parseDevicesList(const std::string& json) {
    DevicesListResponse resp;

    size_t pos = json.find("\"devices\"");
    if (pos == std::string::npos) return resp;

    pos = json.find('[', pos);
    if (pos == std::string::npos) return resp;
    pos++;

    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < json.size()) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            if (depth == 0 && json[pos] == '}') { pos++; break; }
            pos++;
        }
        std::string devJson = json.substr(objStart, pos - objStart);

        DeviceInfo dev;
        dev.userId = extractStr(devJson, "user_id");
        dev.deviceId = extractStr(devJson, "device_id");
        dev.displayName = extractStr(devJson, "display_name");
        dev.lastSeenTs = extractInt(devJson, "last_seen_ts");
        dev.lastSeenIp = extractStr(devJson, "last_seen_ip");
        dev.lastSeenUserAgent = extractStr(devJson, "last_seen_user_agent");
        // Also try unstable MSC3852 field
        if (dev.lastSeenUserAgent.empty()) {
            dev.lastSeenUserAgent = extractStr(devJson, "org.matrix.msc3852.last_seen_user_agent");
        }
        dev.valid = !dev.deviceId.empty();

        if (dev.valid) resp.devices.push_back(dev);
    }

    resp.totalCount = static_cast<int>(resp.devices.size());
    return resp;
}

// Original: fetchDeviceInfo(deviceId) — single device
DeviceInfo DeviceManager::parseDeviceInfo(const std::string& deviceId, const std::string& json) {
    DeviceInfo dev;
    dev.deviceId = deviceId;
    dev.userId = extractStr(json, "user_id");
    dev.displayName = extractStr(json, "display_name");
    if (dev.displayName.empty()) dev.displayName = extractStr(json, "displayName");
    dev.lastSeenTs = extractInt(json, "last_seen_ts");
    dev.lastSeenIp = extractStr(json, "last_seen_ip");
    dev.lastSeenUserAgent = extractStr(json, "last_seen_user_agent");
    dev.valid = !deviceId.empty();
    return dev;
}

// Original: getCryptoDeviceInfo(userId, deviceId) → CryptoDeviceInfo
CryptoDeviceInfo DeviceManager::parseCryptoDeviceInfo(const std::string& deviceId,
                                                       const std::string& userId,
                                                       const std::string& json) {
    CryptoDeviceInfo dev;
    dev.deviceId = deviceId;
    dev.userId = userId;
    dev.isBlocked = extractBool(json, "blocked");
    dev.firstTimeSeenLocalTs = extractInt(json, "first_time_seen_ts");

    // Parse algorithms
    size_t pos = json.find("\"algorithms\"");
    if (pos != std::string::npos) {
        pos = json.find('[', pos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') { pos++; size_t e = pos;
                    while (e < json.size() && json[e] != '"') e++;
                    dev.algorithms.push_back(json.substr(pos, e - pos));
                    pos = e; }
                pos++;
            }
        }
    }

    // Parse keys map: {"ed25519:DEV":"key...", "curve25519:DEV":"key..."}
    auto keysObj = json;
    auto keysPos = keysObj.find("\"keys\"");
    if (keysPos != std::string::npos) {
        keysPos = keysObj.find('{', keysPos);
        if (keysPos != std::string::npos) {
            int depth = 0; size_t s = keysPos; keysPos++;
            while (keysPos < keysObj.size()) {
                if (keysObj[keysPos] == '{') depth++;
                else if (keysObj[keysPos] == '}') depth--;
                if (depth == -1) break;
                keysPos++;
            }
            auto keysJson = keysObj.substr(s, keysPos - s);
            size_t kp = 0;
            while ((kp = keysJson.find("\"ed25519:", kp)) != std::string::npos ||
                   (kp = keysJson.find("\"curve25519:", kp)) != std::string::npos) {
                kp++;
                size_t ke = kp;
                while (ke < keysJson.size() && keysJson[ke] != '"') ke++;
                std::string keyName = keysJson.substr(kp, ke - kp);
                std::string keyVal = extractStr(keysJson.substr(ke), "");
                if (!keyVal.empty()) dev.keys[keyName] = keyVal;
                kp = ke + 2;
            }
        }
    }

    // Parse trust level
    auto tlJson = extractStr(json, "trust_level");
    if (!tlJson.empty()) {
        dev.trustLevel.crossSigningVerified = tlJson.find("cross_signing_verified") != std::string::npos;
        dev.trustLevel.locallyVerified = tlJson.find("locally_verified") != std::string::npos;
    }

    // Parse unsigned info for display name
    auto unsignedJson = json;
    auto unsignedPos = unsignedJson.find("\"unsigned\"");
    if (unsignedPos != std::string::npos) {
        unsignedPos = unsignedJson.find('{', unsignedPos);
        if (unsignedPos != std::string::npos) {
            int depth = 0; size_t s = unsignedPos; unsignedPos++;
            while (unsignedPos < unsignedJson.size()) {
                if (unsignedJson[unsignedPos] == '{') depth++;
                else if (unsignedJson[unsignedPos] == '}') depth--;
                if (depth == -1) break;
                unsignedPos++;
            }
            auto uJson = unsignedJson.substr(s, unsignedPos - s);
            dev.unsignedDisplayName = extractStr(uJson, "device_display_name");
            if (dev.unsignedDisplayName.empty()) {
                dev.unsignedDisplayName = extractStr(uJson, "deviceDisplayName");
            }
        }
    }

    dev.valid = !deviceId.empty();
    return dev;
}

// ====== Device Rename ======
// Original: setDeviceName(deviceId, deviceName) — PUT /devices/{deviceId}
std::string DeviceManager::buildRenameRequest(const DeviceRenameRequest& req) const {
    std::ostringstream os;
    os << R"({"display_name":")" << req.newDisplayName << R"("})";
    return os.str();
}

// ====== Device Deletion ======
// Original: deleteDevice(deviceId, uiaInterceptor)
std::string DeviceManager::buildDeleteRequest(const DeviceDeletionRequest& req) const {
    std::ostringstream os;
    os << R"({"auth":{)" << R"("type":")" << req.authType
       << R"(","session":")" << req.authSession << R"(")";
    if (!req.password.empty()) {
        os << R"(,"password":")" << req.password << R"(")";
    }
    os << R"(,"identifier":{"type":"m.id.user","user":"@"}})";
    os << "}}";
    return os.str();
}

std::string DeviceManager::buildBatchDeleteRequest(const std::vector<DeviceDeletionRequest>& requests) const {
    std::ostringstream os;
    os << R"({"devices":[)";
    for (size_t i = 0; i < requests.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << requests[i].deviceId << "\"";
    }
    os << R"(],"auth":{)" << R"("type":")" << (requests.empty() ? "" : requests[0].authType)
       << R"(","session":")" << (requests.empty() ? "" : requests[0].authSession) << R"(")";
    os << "}}";
    return os.str();
}

bool DeviceManager::requiresUia(const std::string& deleteResponseJson) const {
    return deleteResponseJson.find("\"flows\"") != std::string::npos ||
           deleteResponseJson.find("M_UNAUTHORIZED") != std::string::npos;
}

// ====== Device Verification ======

std::string DeviceManager::formatTrustLevel(const DeviceTrustLevel& level) const {
    if (level.crossSigningVerified) return "Cross-signing verified";
    if (level.locallyVerified) return "Locally verified";
    return "Not verified";
}

std::string DeviceManager::getTrustLabel(const DeviceTrustLevel& level) const {
    if (level.crossSigningVerified) return "Verified";
    if (level.locallyVerified) return "Verified (local)";
    return "Not verified";
}

// ====== Device Fingerprint ======
// Original: getFingerprintHumanReadable() — 4-char groups uppercase

std::string DeviceManager::formatFingerprint(const std::string& rawKey) const {
    std::string upper;
    for (char c : rawKey) upper += (c >= 'a' && c <= 'z') ? c - 32 : c;

    std::string result;
    for (size_t i = 0; i < upper.size(); i++) {
        result += upper[i];
        if ((i + 1) % 4 == 0 && i + 1 < upper.size()) result += ' ';
    }
    return result;
}

std::string DeviceManager::formatShortKey(const std::string& rawKey) const {
    if (rawKey.size() <= 8) return rawKey;
    return rawKey.substr(0, 8);
}

// ====== Device Inactivity ======

bool DeviceManager::isDeviceInactive(int64_t lastSeenTs, int inactivityDays) const {
    if (lastSeenTs <= 0) return true;
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    int64_t threshold = inactivityDays * 24LL * 3600LL * 1000LL;
    return (now - lastSeenTs) > threshold;
}

std::string DeviceManager::formatLastSeen(int64_t lastSeenTs) const {
    if (lastSeenTs <= 0) return "Never";

    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    int64_t diffMs = now - lastSeenTs;

    if (diffMs < 0) return "Just now";

    int64_t secs = diffMs / 1000;
    if (secs < 60) return "Just now";
    int64_t mins = secs / 60;
    if (mins < 60) return std::to_string(mins) + "m ago";
    int64_t hours = mins / 60;
    if (hours < 24) return std::to_string(hours) + "h ago";
    int64_t days = hours / 24;
    if (days < 30) return std::to_string(days) + "d ago";
    int64_t months = days / 30;
    if (months < 12) return std::to_string(months) + "mo ago";
    return std::to_string(days / 365) + "y ago";
}

bool DeviceManager::satisfiesMinVersion(const std::string& clientVersion, const std::string& minRequired) const {
    if (minRequired.empty()) return true;
    if (clientVersion.empty()) return false;
    return clientVersion.compare(minRequired) >= 0;
}

// ====== Sorting & Filtering ======

void DeviceManager::sortDevices(std::vector<DeviceInfo>& devices, DeviceSortMode mode) const {
    switch (mode) {
        case DeviceSortMode::BY_NAME:
            std::sort(devices.begin(), devices.end(), [](const DeviceInfo& a, const DeviceInfo& b) {
                return a.displayName < b.displayName;
            });
            break;
        case DeviceSortMode::BY_LAST_SEEN:
            std::sort(devices.begin(), devices.end(), [](const DeviceInfo& a, const DeviceInfo& b) {
                return a.lastSeenTs > b.lastSeenTs;
            });
            break;
        case DeviceSortMode::BY_TRUST_LEVEL:
            std::sort(devices.begin(), devices.end(), [](const DeviceInfo& a, const DeviceInfo& b) {
                return a.displayName < b.displayName; // Just alpha for basic devices
            });
            break;
    }
}

void DeviceManager::sortCryptoDevices(std::vector<CryptoDeviceInfo>& devices, DeviceSortMode mode) const {
    switch (mode) {
        case DeviceSortMode::BY_NAME:
            std::sort(devices.begin(), devices.end(), [](const CryptoDeviceInfo& a, const CryptoDeviceInfo& b) {
                return a.displayName() < b.displayName();
            });
            break;
        case DeviceSortMode::BY_TRUST_LEVEL:
            std::sort(devices.begin(), devices.end(), [](const CryptoDeviceInfo& a, const CryptoDeviceInfo& b) {
                // Verified first
                if (a.isVerified() != b.isVerified()) return a.isVerified();
                return a.displayName() < b.displayName();
            });
            break;
        case DeviceSortMode::BY_LAST_SEEN:
            std::sort(devices.begin(), devices.end(), [](const CryptoDeviceInfo& a, const CryptoDeviceInfo& b) {
                return a.firstTimeSeenLocalTs > b.firstTimeSeenLocalTs;
            });
            break;
    }
}

std::vector<DeviceInfo> DeviceManager::filterDevices(const std::vector<DeviceInfo>& devices,
                                                      const DeviceFilter& filter) const {
    std::vector<DeviceInfo> result;
    for (const auto& dev : devices) {
        if (filter.inactiveOnly && !isDeviceInactive(dev.lastSeenTs, filter.inactivityDays)) continue;
        result.push_back(dev);
    }

    // Current device first
    if (filter.currentDeviceFirst && !filter.currentDeviceId.empty()) {
        std::sort(result.begin(), result.end(), [&](const DeviceInfo& a, const DeviceInfo& b) {
            if (a.deviceId == filter.currentDeviceId) return true;
            if (b.deviceId == filter.currentDeviceId) return false;
            return a.deviceId < b.deviceId;
        });
    }

    return result;
}

// ====== Serialization ======

std::string DeviceManager::deviceToJson(const DeviceInfo& device) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"device_id":")" << esc(device.deviceId)
       << R"(","user_id":")" << esc(device.userId)
       << R"(","display_name":")" << esc(device.displayName)
       << R"(,"last_seen_ts":)" << device.lastSeenTs
       << R"(,"last_seen":")" << formatLastSeen(device.lastSeenTs)
       << R"(","last_seen_ip":")" << esc(device.lastSeenIp)
       << R"(","user_agent":")" << esc(device.getBestLastSeenUserAgent())
       << R"(,"is_inactive":)" << (isDeviceInactive(device.lastSeenTs) ? "true" : "false")
       << "}";
    return os.str();
}

std::string DeviceManager::cryptoDeviceToJson(const CryptoDeviceInfo& device) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"device_id":")" << esc(device.deviceId)
       << R"(","user_id":")" << esc(device.userId)
       << R"(","display_name":")" << esc(device.displayName())
       << R"(,"fingerprint":")" << esc(formatFingerprint(device.fingerprint()))
       << R"(,"identity_key":")" << esc(device.identityKey())
       << R"(,"trust_label":")" << esc(getTrustLabel(device.trustLevel))
       << R"(,"is_verified":)" << (device.isVerified() ? "true" : "false")
       << R"(,"is_blocked":)" << (device.isBlocked ? "true" : "false")
       << R"(,"algorithms":[)";
    for (size_t i = 0; i < device.algorithms.size(); i++) {
        if (i > 0) os << ","; os << "\"" << device.algorithms[i] << "\"";
    }
    os << "]}";
    return os.str();
}

std::string DeviceManager::devicesToJson(const std::vector<DeviceInfo>& devices) const {
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < devices.size(); i++) {
        if (i > 0) os << ","; os << deviceToJson(devices[i]);
    }
    os << "]";
    return os.str();
}

std::string DeviceManager::cryptoDevicesToJson(const std::vector<CryptoDeviceInfo>& devices) const {
    std::ostringstream os; os << "[";
    for (size_t i = 0; i < devices.size(); i++) {
        if (i > 0) os << ","; os << cryptoDeviceToJson(devices[i]);
    }
    os << "]";
    return os.str();
}

std::string DeviceManager::trustLevelToJson(const DeviceTrustLevel& level) const {
    std::ostringstream os;
    os << R"({"cross_signing_verified":)" << (level.crossSigningVerified ? "true" : "false")
       << R"(,"locally_verified":)" << (level.locallyVerified ? "true" : "false")
       << R"(,"is_verified":)" << (level.isVerified() ? "true" : "false")
       << R"(,"label":")" << getTrustLabel(level) << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
