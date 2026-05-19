#include "progressive/auth_models.hpp"

namespace progressive {

const char* presenceEnumToString(PresenceEnum p) {
    switch (p) {
        case PresenceEnum::ONLINE: return "online";
        case PresenceEnum::OFFLINE: return "offline";
        case PresenceEnum::UNAVAILABLE: return "unavailable";
        case PresenceEnum::BUSY: return "busy";
    }
    return "offline";
}
PresenceEnum presenceEnumFromString(const std::string& s) {
    if (s == "online") return PresenceEnum::ONLINE;
    if (s == "unavailable") return PresenceEnum::UNAVAILABLE;
    if (s == "busy") return PresenceEnum::BUSY;
    return PresenceEnum::OFFLINE;
}

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

// ==== Parse Credentials ====
//
// Original Kotlin (Credentials.kt:33-63)
// JSON: {"user_id":"@...","access_token":"syt_...","device_id":"...","well_known":{...}}

Credentials parseCredentials(const std::string& json) {
    Credentials c;
    c.userId = extractJsonString(json, "user_id");
    c.accessToken = extractJsonString(json, "access_token");
    c.refreshToken = extractJsonString(json, "refresh_token");
    c.homeServer = extractJsonString(json, "home_server");
    c.deviceId = extractJsonString(json, "device_id");

    auto wkJson = extractJsonObject(json, "well_known");
    if (!wkJson.empty()) {
        auto hsJson = extractJsonObject(wkJson, "m.homeserver");
        if (!hsJson.empty()) c.discoveryInfo.homeServer.baseUrl = extractJsonString(hsJson, "base_url");
        auto isJson = extractJsonObject(wkJson, "m.identity_server");
        if (!isJson.empty()) c.discoveryInfo.identityServer.baseUrl = extractJsonString(isJson, "base_url");
    }

    return c;
}

// ==== Parse SessionParams ====

SessionParams parseSessionParams(const std::string& json) {
    SessionParams p;
    // Parse nested credentials
    auto credsJson = extractJsonObject(json, "credentials");
    if (!credsJson.empty()) p.credentials = parseCredentials(credsJson);

    p.isTokenValid = extractJsonBool(json, "isTokenValid");

    auto ltStr = extractJsonString(json, "loginType");
    if (ltStr == "PASSWORD") p.loginType = LoginType::PASSWORD;
    else if (ltStr == "SSO") p.loginType = LoginType::SSO;
    else if (ltStr == "TOKEN") p.loginType = LoginType::TOKEN;

    return p;
}

// ==== Parse UserPresence ====
//
// Original Kotlin (UserPresence.kt:21-24)

UserPresence parseUserPresence(const std::string& json) {
    UserPresence up;
    up.lastActiveAgo = extractJsonInt64(json, "last_active_ago");
    up.statusMessage = extractJsonString(json, "status_msg");
    up.isCurrentlyActive = extractJsonBool(json, "currently_active");
    up.presence = presenceEnumFromString(extractJsonString(json, "presence"));
    return up;
}

// ==== Serialize ====

std::string credentialsToJson(const Credentials& creds) {
    std::string json = "{";
    json += "\"user_id\":\"" + creds.userId + "\",";
    json += "\"access_token\":\"" + creds.accessToken + "\",";
    json += "\"device_id\":\"" + creds.deviceId + "\"";
    json += "}";
    return json;
}

std::string sessionParamsToJson(const SessionParams& params) {
    std::string json = "{";
    json += "\"userId\":\"" + params.userId() + "\",";
    json += "\"deviceId\":\"" + params.deviceId() + "\",";
    json += "\"homeServerUrl\":\"" + params.homeServerUrl() + "\"";
    json += "}";
    return json;
}

std::string userPresenceToJson(const UserPresence& presence) {
    std::string json = "{";
    json += "\"presence\":\"" + std::string(presenceEnumToString(presence.presence)) + "\",";
    json += "\"last_active_ago\":" + std::to_string(presence.lastActiveAgo) + ",";
    json += "\"currently_active\":" + std::string(presence.isCurrentlyActive ? "true" : "false");
    if (!presence.statusMessage.empty())
        json += ",\"status_msg\":\"" + presence.statusMessage + "\"";
    json += "}";
    return json;
}

// ==== Secure Storage Parsing ====

SecretStorageKeyContent parseSecretStorageKey(const std::string& json) {
    SecretStorageKeyContent k;
    k.algorithm = extractJsonString(json, "algorithm");
    k.name = extractJsonString(json, "name");
    k.publicKey = extractJsonString(json, "pubkey");
    auto ppJson = extractJsonObject(json, "passphrase");
    if (!ppJson.empty()) {
        k.passphrase.algorithm = extractJsonString(ppJson, "algorithm");
        k.passphrase.iterations = static_cast<int>(extractJsonInt64(ppJson, "iterations"));
        if (k.passphrase.iterations <= 0) k.passphrase.iterations = 500000;
        k.passphrase.salt = extractJsonString(ppJson, "salt");
    }
    return k;
}

EncryptedSecretContent parseEncryptedSecret(const std::string& json) {
    EncryptedSecretContent c;
    c.ciphertext = extractJsonString(json, "ciphertext");
    c.mac = extractJsonString(json, "mac");
    c.ephemeral = extractJsonString(json, "ephemeral");
    c.iv = extractJsonString(json, "iv");
    return c;
}

KeyInfoResult parseKeyInfoResult(const std::string& json) {
    KeyInfoResult r;
    r.success = extractJsonBool(json, "success");
    if (r.success) r.content = parseSecretStorageKey(json);
    return r;
}

// ==== HomeServer Capabilities ====

HomeServerCapabilities parseHomeServerCapabilities(const std::string& json) {
    HomeServerCapabilities c;
    c.canChangePassword = extractJsonBool(json, "canChangePassword");
    c.canChangeDisplayName = extractJsonBool(json, "canChangeDisplayName");
    c.canChangeAvatar = extractJsonBool(json, "canChangeAvatar");
    c.canChange3pid = extractJsonBool(json, "canChange3pid");
    c.maxUploadFileSize = extractJsonInt64(json, "maxUploadFileSize");
    c.canUseThreading = extractJsonBool(json, "canUseThreading");
    c.canUseAuthenticatedMedia = extractJsonBool(json, "canUseAuthenticatedMedia");
    c.authenticationIssuer = extractJsonString(json, "authenticationIssuer");
    c.delegatedOidcAuthEnabled = !c.authenticationIssuer.empty();
    return c;
}

RoomCapabilitySupport HomeServerCapabilities::isFeatureSupported(const std::string& feature) const {
    if (roomVersions.capabilities.empty()) return RoomCapabilitySupport::UNKNOWN;
    auto it = roomVersions.capabilities.find(feature);
    if (it == roomVersions.capabilities.end()) return RoomCapabilitySupport::UNSUPPORTED;
    const auto& pref = it->second.preferred.empty() ? (it->second.support.empty() ? "" : it->second.support.back()) : it->second.preferred;
    for (const auto& v : roomVersions.supportedVersion)
        if (v.version == pref)
            return v.status == RoomVersionStatus::STABLE ? RoomCapabilitySupport::SUPPORTED : RoomCapabilitySupport::SUPPORTED_UNSTABLE;
    return RoomCapabilitySupport::UNKNOWN;
}

std::string HomeServerCapabilities::versionOverrideForFeature(const std::string& feature) const {
    auto it = roomVersions.capabilities.find(feature);
    if (it == roomVersions.capabilities.end()) return "";
    if (!it->second.preferred.empty()) return it->second.preferred;
    return it->second.support.empty() ? "" : it->second.support.back();
}

// ==== Widget Models ====

WidgetContent parseWidgetContent(const std::string& json) {
    WidgetContent w;
    w.creatorUserId = extractJsonString(json, "creatorUserId");
    w.id = extractJsonString(json, "id");
    w.type = extractJsonString(json, "type");
    w.url = extractJsonString(json, "url");
    w.name = extractJsonString(json, "name");
    w.dataJson = extractJsonObject(json, "data");
    w.waitForIframeLoad = extractJsonBool(json, "waitForIframeLoad");
    return w;
}

} // namespace progressive
