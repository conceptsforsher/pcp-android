#include "progressive/crypto_models.hpp"

namespace progressive {

// ==== JSON Helpers ====

static std::string extractJsonString(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t extractJsonInt64(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return val;
}

static bool extractJsonBool(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    return json.compare(pos, 4, "true") == 0;
}

static std::string extractJsonObject(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

// ==== Parse DeviceInfo ====
//
// Original Kotlin (DeviceInfo.kt:26-62)

DeviceInfo parseDeviceInfo(const std::string& json) {
    DeviceInfo d;
    d.userId = extractJsonString(json, "user_id");
    d.deviceId = extractJsonString(json, "device_id");
    d.displayName = extractJsonString(json, "display_name");
    d.lastSeenTs = extractJsonInt64(json, "last_seen_ts");
    d.lastSeenIp = extractJsonString(json, "last_seen_ip");
    d.lastSeenUserAgent = extractJsonString(json, "last_seen_user_agent");
    return d;
}

// Original Kotlin (DevicesListResponse.kt:26-29)
DevicesListResponse parseDevicesList(const std::string& json) {
    DevicesListResponse r;
    auto arrPos = json.find("\"devices\"");
    if (arrPos != std::string::npos) {
        arrPos = json.find('[', arrPos);
        if (arrPos != std::string::npos) {
            size_t pos = arrPos + 1;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                if (json[pos] == '{') {
                    int d = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < json.size() && d > 0) {
                        if (json[pos] == '{') d++;
                        else if (json[pos] == '}') d--;
                        pos++;
                    }
                    r.devices.push_back(parseDeviceInfo(json.substr(start, pos - start)));
                }
            }
        }
    }
    return r;
}

// ==== Parse CryptoDeviceInfo ====
//
// Original Kotlin (CryptoDeviceInfo.kt:25-53)

CryptoDeviceInfo parseCryptoDeviceInfo(const std::string& json) {
    CryptoDeviceInfo d;
    d.userId = extractJsonString(json, "user_id");
    d.deviceId = extractJsonString(json, "device_id");
    d.isBlocked = extractJsonBool(json, "isBlocked");
    d.firstTimeSeenLocalTs = extractJsonInt64(json, "firstTimeSeenLocalTs");

    auto unsignedJson = extractJsonObject(json, "unsigned");
    if (!unsignedJson.empty()) {
        d.unsignedInfo.deviceDisplayName = extractJsonString(unsignedJson, "device_display_name");
    }

    return d;
}

// ==== Parse ForwardedRoomKeyContent ====
//
// Original Kotlin (ForwardedRoomKeyContent.kt:27-62)

ForwardedRoomKeyContent parseForwardedRoomKey(const std::string& json) {
    ForwardedRoomKeyContent k;
    k.algorithm = extractJsonString(json, "algorithm");
    k.roomId = extractJsonString(json, "room_id");
    k.senderKey = extractJsonString(json, "sender_key");
    k.sessionId = extractJsonString(json, "session_id");
    k.sessionKey = extractJsonString(json, "session_key");
    k.senderClaimedEd25519Key = extractJsonString(json, "sender_claimed_ed25519_key");
    k.sharedHistory = extractJsonBool(json, "org.matrix.msc3061.shared_history");
    return k;
}

// ==== Parse RoomKeyShareRequest ====
//
// Original Kotlin (RoomKeyShareRequest.kt:25-39)

RoomKeyShareRequest parseRoomKeyShareRequest(const std::string& json) {
    RoomKeyShareRequest r;
    r.action = extractJsonString(json, "action");
    r.requestingDeviceId = extractJsonString(json, "requesting_device_id");
    r.requestId = extractJsonString(json, "request_id");

    auto bodyJson = extractJsonObject(json, "body");
    if (!bodyJson.empty()) {
        r.body = parseRoomKeyContent(bodyJson);
    }

    return r;
}

// ==== Parse ImportRoomKeysResult ====
//
// Original Kotlin (ImportRoomKeysResult.kt:22-26)

ImportRoomKeysResult parseImportRoomKeysResult(const std::string& json) {
    ImportRoomKeysResult r;
    r.totalNumberOfKeys = static_cast<int>(extractJsonInt64(json, "totalNumberOfKeys"));
    r.successfullyNumberOfImportedKeys = static_cast<int>(extractJsonInt64(json, "successfullyNumberOfImportedKeys"));
    return r;
}

// ==== Serialize ====

std::string deviceInfoToJson(const DeviceInfo& info) {
    std::string json = "{";
    if (!info.deviceId.empty()) json += "\"device_id\":\"" + info.deviceId + "\",";
    if (!info.userId.empty()) json += "\"user_id\":\"" + info.userId + "\",";
    if (!info.displayName.empty()) json += "\"display_name\":\"" + info.displayName + "\",";
    json += "\"last_seen_ts\":" + std::to_string(info.lastSeenTs);
    json += "}";
    return json;
}

std::string cryptoDeviceInfoToJson(const CryptoDeviceInfo& info) {
    std::string json = "{";
    json += "\"device_id\":\"" + info.deviceId + "\",";
    json += "\"user_id\":\"" + info.userId + "\"";
    if (!info.displayName().empty()) json += ",\"display_name\":\"" + info.displayName() + "\"";
    json += "}";
    return json;
}

} // namespace progressive
