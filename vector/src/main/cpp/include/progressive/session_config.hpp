#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <sstream>

namespace progressive {

// ==== Room Version Capabilities ====
// Original Kotlin: HomeServerCapabilities.RoomCapabilitySupport

enum class RoomCapabilitySupport {
    SUPPORTED,
    SUPPORTED_UNSTABLE,
    UNSUPPORTED,
    UNKNOWN
};

inline std::string roomCapabilitySupportToString(RoomCapabilitySupport support) {
    switch (support) {
        case RoomCapabilitySupport::SUPPORTED:          return "SUPPORTED";
        case RoomCapabilitySupport::SUPPORTED_UNSTABLE: return "SUPPORTED_UNSTABLE";
        case RoomCapabilitySupport::UNSUPPORTED:        return "UNSUPPORTED";
        case RoomCapabilitySupport::UNKNOWN:            return "UNKNOWN";
        default:                                        return "UNKNOWN";
    }
}

inline RoomCapabilitySupport roomCapabilitySupportFromString(const std::string& name) {
    if (name == "SUPPORTED")          return RoomCapabilitySupport::SUPPORTED;
    if (name == "SUPPORTED_UNSTABLE") return RoomCapabilitySupport::SUPPORTED_UNSTABLE;
    if (name == "UNSUPPORTED")        return RoomCapabilitySupport::UNSUPPORTED;
    return RoomCapabilitySupport::UNKNOWN;
}

// ==== HomeServer Capabilities ====
// Original Kotlin: org.matrix.android.sdk.api.session.homeserver.HomeServerCapabilities
// Represents capabilities discovered via GET /_matrix/client/r0/capabilities

struct RoomVersionMeta {
    std::string preferred;
    std::vector<std::string> support;
};

struct RoomVersionEntry {
    std::string version;
    std::string status; // "stable" or "unstable"
};

struct HomeServerCapabilities {
    // Max size of file which can be uploaded to the homeserver, in bytes.
    // Original Kotlin: MAX_UPLOAD_FILE_SIZE_UNKNOWN = -1L
    int64_t maxUploadFileSize = -1;

    // Whether the homeserver allows changing the account password.
    bool canChangePassword = true;

    // Whether the homeserver allows changing the account display name.
    bool canChangeDisplayName = true;

    // Whether the homeserver allows changing the account avatar.
    bool canChangeAvatar = true;

    // Whether the homeserver allows changing the 3pid associations.
    bool canChange3pid = true;

    // Whether the homeserver supports the last version of the identity server
    // and binding protocol.
    bool lastVersionIdentityServerSupported = false;

    // Default identity server URL, provided in WellKnown.
    std::string defaultIdentityServerUrl;

    // Whether the homeserver supports threading.
    // Original Kotlin: canUseThreading
    bool canUseThreading = false;

    // Whether the homeserver supports threaded read receipts and unread
    // notifications (MSC3771).
    // Original Kotlin: canUseThreadReadReceiptsAndNotifications
    bool canUseThreadReadReceiptsAndNotifications = false;

    // Whether the homeserver supports controlling logout of all devices
    // when changing password.
    bool canControlLogoutDevices = false;

    // Whether the homeserver supports login via QR code.
    bool canLoginWithQrCode = false;

    // Whether the homeserver supports redaction of related events (MSC3912).
    // Original Kotlin: canRedactRelatedEvents (var)
    bool canRedactRelatedEvents = false;

    // Whether the homeserver supports remote toggle of pushers for a device.
    bool canRemotelyTogglePushNotificationsOfDevices = false;

    // External account management URL for OAuth API (MSC4191).
    std::string externalAccountManagementUrl;

    // External account management supported actions (MSC4191).
    std::vector<std::string> externalAccountManagementSupportedActions;

    // Authentication issuer for delegated OIDC (MSC3824).
    std::string authenticationIssuer;

    // Whether the homeserver supports authenticated media (MSC3916).
    bool canUseAuthenticatedMedia = false;

    // If set to true, SDK will not use network constraint for WorkManager.
    // Provided in WellKnown.
    bool disableNetworkConstraint = false;

    // Room version capabilities (feature -> version metadata).
    std::unordered_map<std::string, RoomVersionMeta> roomCapabilities;

    // Overall room versions supported by the server.
    std::vector<RoomVersionEntry> supportedRoomVersions;

    static constexpr int64_t MAX_UPLOAD_FILE_SIZE_UNKNOWN = -1;
    static inline const char* ROOM_CAP_KNOCK = "knock";
    static inline const char* ROOM_CAP_RESTRICTED = "restricted";

    // Convenience: derived from authenticationIssuer not being empty.
    bool delegatedOidcAuthEnabled() const {
        return !authenticationIssuer.empty();
    }

    // Check if all defaults are in place (server capabilities not yet fetched).
    bool isDefault() const {
        return maxUploadFileSize == MAX_UPLOAD_FILE_SIZE_UNKNOWN
            && canChangePassword && canChangeDisplayName && canChangeAvatar
            && canChange3pid && !lastVersionIdentityServerSupported
            && !canUseThreading && !canUseThreadReadReceiptsAndNotifications
            && !canControlLogoutDevices && !canLoginWithQrCode
            && !canRedactRelatedEvents && !canRemotelyTogglePushNotificationsOfDevices
            && !canUseAuthenticatedMedia && !disableNetworkConstraint
            && defaultIdentityServerUrl.empty()
            && externalAccountManagementUrl.empty()
            && authenticationIssuer.empty()
            && roomCapabilities.empty()
            && supportedRoomVersions.empty();
    }

    // Check if a capability feature is supported.
    // Returns UNKNOWN if the server does not implement room capabilities.
    RoomCapabilitySupport isFeatureSupported(const std::string& feature) const {
        auto it = roomCapabilities.find(feature);
        if (it == roomCapabilities.end()) return RoomCapabilitySupport::UNSUPPORTED;

        const auto& preferred = it->second.preferred;
        auto ver = preferred.empty()
            ? (!it->second.support.empty() ? it->second.support.back() : "")
            : preferred;

        for (const auto& entry : supportedRoomVersions) {
            if (entry.version == ver) {
                return entry.status == "stable"
                    ? RoomCapabilitySupport::SUPPORTED
                    : RoomCapabilitySupport::SUPPORTED_UNSTABLE;
            }
        }
        return RoomCapabilitySupport::UNKNOWN;
    }

    // Version override for a feature when creating a room that requires it.
    std::string versionOverrideForFeature(const std::string& feature) const {
        auto it = roomCapabilities.find(feature);
        if (it == roomCapabilities.end()) return {};
        return it->second.preferred.empty()
            ? (!it->second.support.empty() ? it->second.support.back() : "")
            : it->second.preferred;
    }
};

// Manual JSON parser helper — extracts a string value between quotes.
// Used internally by the capabilities parser.
namespace {
    inline std::string extractJsonString(const std::string& json, size_t& pos) {
        while (pos < json.size() && json[pos] != '"') ++pos;
        if (pos >= json.size()) return {};
        ++pos; // skip opening quote
        size_t start = pos;
        while (pos < json.size() && json[pos] != '"') {
            if (json[pos] == '\\' && pos + 1 < json.size()) ++pos; // skip escaped
            ++pos;
        }
        std::string result = json.substr(start, pos - start);
        if (pos < json.size()) ++pos; // skip closing quote
        return result;
    }

    inline std::string findJsonValue(const std::string& json, const std::string& key) {
        std::string search = '"' + key + '"';
        auto p = json.find(search);
        if (p == std::string::npos) return {};
        p += search.size();
        while (p < json.size() && (json[p] == ' ' || json[p] == '\t' || json[p] == '\n' || json[p] == '\r')) ++p;
        if (p >= json.size() || json[p] != ':') return {};
        ++p;
        while (p < json.size() && (json[p] == ' ' || json[p] == '\t' || json[p] == '\n' || json[p] == '\r')) ++p;
        if (p >= json.size()) return {};

        if (json[p] == '"') {
            return extractJsonString(json, p);
        }
        // number or literal
        auto end = p;
        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ' &&
               json[end] != '\t' && json[end] != '\n' && json[end] != '\r') ++end;
        return json.substr(p, end - p);
    }

    inline bool findJsonBool(const std::string& json, const std::string& key, bool defVal = false) {
        auto v = findJsonValue(json, key);
        if (v.empty()) return defVal;
        return v == "true";
    }

    inline int64_t findJsonInt64(const std::string& json, const std::string& key, int64_t defVal = 0) {
        auto v = findJsonValue(json, key);
        if (v.empty()) return defVal;
        return std::strtoll(v.c_str(), nullptr, 10);
    }
} // anonymous namespace

// Original Kotlin: HomeServerCapabilities parsed from GET /_matrix/client/r0/capabilities
inline HomeServerCapabilities parseHomeServerCapabilities(const std::string& json) {
    HomeServerCapabilities caps;

    // Top-level: {"capabilities": {...}}
    auto capObj = findJsonValue(json, "capabilities");

    // Parse individual capability fields from the capabilities object
    auto parseCap = [&](const std::string& key) -> std::string {
        return findJsonValue(capObj.empty() ? json : capObj, key);
    };

    // m.change_password
    caps.canChangePassword = findJsonBool(parseCap("m.change_password"), "enabled", true);

    // m.room_versions
    auto roomVers = parseCap("m.room_versions");
    if (!roomVers.empty()) {
        // default
        caps.supportedRoomVersions.push_back(
            {findJsonValue(roomVers, "default"), "stable"});
        // available
        auto avail = findJsonValue(roomVers, "available");
        // Simplified: extract version names from JSON object keys
        if (!avail.empty()) {
            size_t pos = 0;
            while (pos < avail.size()) {
                while (pos < avail.size() && avail[pos] != '"' && avail[pos] != '{') ++pos;
                if (pos >= avail.size() || avail[pos] == '{') break;
                auto ver = extractJsonString(avail, pos);
                if (!ver.empty() && ver != "default" && ver != "available") {
                    // Check stability status
                    auto statusStr = findJsonValue(avail, ver);
                    auto stability = findJsonValue(statusStr.empty() ? avail : statusStr, "stable");
                    bool isStable = stability == "stable" || stability == "true";
                    caps.supportedRoomVersions.push_back({ver, isStable ? "stable" : "unstable"});
                }
            }
        }
    }

    // m.change_password enabled
    caps.canChangePassword = findJsonBool(parseCap("m.change_password"), "enabled", true);

    // m.set_displayname (no server capability, always true)
    caps.canChangeDisplayName = true;

    // m.set_avatar_url (no server capability, always true)
    caps.canChangeAvatar = true;

    // m.3pid_changes
    caps.canChange3pid = findJsonBool(parseCap("m.3pid_changes"), "enabled", true);

    // m.change_password (re-read for maxUploadFileSize... actually not)

    // Threading: m.thread (MSC3440)
    auto threadCap = parseCap("m.thread");
    caps.canUseThreading = !threadCap.empty() && findJsonBool(threadCap, "enabled", true);

    // Threaded read receipts (MSC3771)
    auto threadRR = parseCap("org.matrix.msc3771");
    caps.canUseThreadReadReceiptsAndNotifications = !threadRR.empty() && findJsonBool(threadRR, "enabled", true);
    if (!caps.canUseThreadReadReceiptsAndNotifications) {
        auto threadRR2 = parseCap("org.matrix.msc3771.threaded_read_receipts");
        caps.canUseThreadReadReceiptsAndNotifications = !threadRR2.empty();
    }

    // Control logout devices
    auto logoutCtrl = parseCap("m.change_password");
    caps.canControlLogoutDevices = !logoutCtrl.empty();

    // QR code login
    auto qrLogin = parseCap("org.matrix.msc3885");
    caps.canLoginWithQrCode = !qrLogin.empty();

    // Redact related events (MSC3912)
    auto rre = parseCap("org.matrix.msc3912");
    caps.canRedactRelatedEvents = !rre.empty();

    // Remote push toggle
    auto rpt = parseCap("org.matrix.msc3881");
    caps.canRemotelyTogglePushNotificationsOfDevices = !rpt.empty();

    // Authenticated media (MSC3916)
    auto authMedia = parseCap("org.matrix.msc3916");
    caps.canUseAuthenticatedMedia = !authMedia.empty();

    // Last version identity server
    auto isVer = parseCap("m.identity_server");
    caps.lastVersionIdentityServerSupported = !isVer.empty();

    // Default identity server URL
    caps.defaultIdentityServerUrl = findJsonValue(parseCap("m.identity_server"), "base_url");

    // External account management (MSC4191)
    auto extAcc = parseCap("m.external_account_management");
    if (!extAcc.empty()) {
        caps.externalAccountManagementUrl = findJsonValue(extAcc, "account_management_url");
        auto actions = findJsonValue(extAcc, "supported_actions");
        if (!actions.empty()) {
            size_t pos = 0;
            while (pos < actions.size()) {
                while (pos < actions.size() && actions[pos] != '"') ++pos;
                auto action = extractJsonString(actions, pos);
                if (!action.empty()) {
                    caps.externalAccountManagementSupportedActions.push_back(action);
                }
            }
        }
    }

    // Authentication issuer (MSC3824 delegated OIDC)
    caps.authenticationIssuer = findJsonValue(parseCap("m.authentication"), "issuer");

    // Room capabilities (feature -> version mapping)
    auto roomCapObj = parseCap("m.room_capabilities");
    if (!roomCapObj.empty()) {
        size_t pos = 0;
        while (pos < roomCapObj.size()) {
            while (pos < roomCapObj.size() && roomCapObj[pos] != '"') ++pos;
            auto feature = extractJsonString(roomCapObj, pos);
            if (feature.empty()) break;
            RoomVersionMeta meta;
            meta.preferred = findJsonValue(roomCapObj, "preferred");
            auto supportList = findJsonValue(roomCapObj, "support");
            if (!supportList.empty()) {
                size_t sp = 0;
                while (sp < supportList.size()) {
                    while (sp < supportList.size() && supportList[sp] != '"') ++sp;
                    auto sv = extractJsonString(supportList, sp);
                    if (!sv.empty()) meta.support.push_back(sv);
                    else break;
                }
            }
            caps.roomCapabilities[feature] = meta;
        }
    }

    // Disable network constraint (from WellKnown)
    caps.disableNetworkConstraint = findJsonBool(json, "disableNetworkConstraint", false);

    return caps;
}

// Original Kotlin: HomeServerCapabilities JSON serialization for storage / JNI
inline std::string buildHomeServerCapabilitiesJson(const HomeServerCapabilities& caps) {
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
    json << R"("maxUploadFileSize": )" << caps.maxUploadFileSize << ",";
    json << R"("canChangePassword": )" << (caps.canChangePassword ? "true" : "false") << ",";
    json << R"("canChangeDisplayName": )" << (caps.canChangeDisplayName ? "true" : "false") << ",";
    json << R"("canChangeAvatar": )" << (caps.canChangeAvatar ? "true" : "false") << ",";
    json << R"("canChange3pid": )" << (caps.canChange3pid ? "true" : "false") << ",";
    json << R"("lastVersionIdentityServerSupported": )" << (caps.lastVersionIdentityServerSupported ? "true" : "false") << ",";
    json << R"("defaultIdentityServerUrl": ")" << esc(caps.defaultIdentityServerUrl) << R"(",)";
    json << R"("canUseThreading": )" << (caps.canUseThreading ? "true" : "false") << ",";
    json << R"("canUseThreadReadReceiptsAndNotifications": )" << (caps.canUseThreadReadReceiptsAndNotifications ? "true" : "false") << ",";
    json << R"("canControlLogoutDevices": )" << (caps.canControlLogoutDevices ? "true" : "false") << ",";
    json << R"("canLoginWithQrCode": )" << (caps.canLoginWithQrCode ? "true" : "false") << ",";
    json << R"("canRedactRelatedEvents": )" << (caps.canRedactRelatedEvents ? "true" : "false") << ",";
    json << R"("canRemotelyTogglePushNotificationsOfDevices": )" << (caps.canRemotelyTogglePushNotificationsOfDevices ? "true" : "false") << ",";
    json << R"("externalAccountManagementUrl": ")" << esc(caps.externalAccountManagementUrl) << R"(",)";
    json << R"("authenticationIssuer": ")" << esc(caps.authenticationIssuer) << R"(",)";
    json << R"("canUseAuthenticatedMedia": )" << (caps.canUseAuthenticatedMedia ? "true" : "false") << ",";
    json << R"("disableNetworkConstraint": )" << (caps.disableNetworkConstraint ? "true" : "false") << ",";
    json << R"("delegatedOidcAuthEnabled": )" << (caps.delegatedOidcAuthEnabled() ? "true" : "false") << ",";
    json << R"("externalAccountManagementSupportedActions": [)";
    for (size_t i = 0; i < caps.externalAccountManagementSupportedActions.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(caps.externalAccountManagementSupportedActions[i]) << R"(")";
    }
    json << "]";
    // roomCapabilities omitted for brevity in serialization
    json << "}";
    return json.str();
}

// ==== Feature Flags & Labs Configuration ====
//
// Central registry for all Progressive Chat feature flags.
// Each feature maps to a Labs setting and can be queried at runtime.
// Used by the C++ layer to check if native modules should be used.

struct FeatureFlag {
    std::string key;           // SharedPreferences key
    std::string name;          // Human-readable name
    std::string description;   // What the feature does
    bool defaultValue = false; // Default state (usually OFF)
    bool requiresRestart = false; // Needs app restart to take effect
};

// All registered Progressive Chat feature flags.
inline std::vector<FeatureFlag> getAllFeatureFlags() {
    return {
        {"SETTINGS_LABS_NATIVE_HTTP_KEY", "Native HTTP", "C++ HTTP client via JNI TLS bridge", false, true},
        {"SETTINGS_LABS_NATIVE_TIMELINE_KEY", "Native Timeline", "C++ pagination engine", false, true},
        {"SETTINGS_LABS_WEB_SEARCH_KEY", "Web Search", "/web command: SearXNG/DDG/Google", false, false},
        {"SETTINGS_LABS_AGENT_WEB_ACCESS_KEY", "Agent Web", "Allow /agent to search web", false, false},
        {"SETTINGS_LABS_ROOM_COUNT_KEY", "Room Count", "Show room count in header", false, false},
        {"SETTINGS_LABS_ROOM_COUNT_SPLIT_KEY", "Room Count Split", "Per-account count display", false, false},
        {"SETTINGS_LABS_ROOM_COUNT_UNIQUE_KEY", "Room Count Unique", "Count unique rooms only", false, false},
        {"SETTINGS_LABS_ROOM_NUMBERING_KEY", "Room Numbering", "Assign join-order numbers", false, false},
        {"SETTINGS_LABS_LIVE_DRAFT_KEY", "Live Draft", "Auto-save drafts while typing", false, false},
        {"SETTINGS_LABS_FULL_AVATAR_KEY", "Full Avatars", "Square/rect avatars not circles", false, false},
        {"SETTINGS_LABS_AVATAR_ORIGINAL_RATIO", "Avatar Ratio", "Preserve original aspect ratio", true, false},
        {"SETTINGS_NOTIFICATION_ICON_STYLE", "Notif Icon", "Notification icon style selector", false, false},
        {"SETTINGS_LABS_MULTI_SERVER_EXPORT_KEY", "Multi-Server Export", "Export from multiple servers", false, false},
        {"SETTINGS_LABS_PROFILE_SWIPER_KEY", "Profile Swiper", "Swipe between user profiles", false, false},
        {"SETTINGS_LABS_DESYNC_DETECTOR_KEY", "Desync Detector", "Detect timeline desync", false, false},
    };
}

// ==== Session Configuration ====
//
// Per-session configuration that the C++ layer needs.
// Original Kotlin: derived from SessionParams + SecureStorage preferences

struct SessionConfig {
    // Core connection fields
    std::string homeserverUrl;
    std::string accessToken;
    std::string userId;
    std::string deviceId;
    std::string sessionId;           // Unique session identifier
    int64_t loginTimestamp = 0;      // When session was created

    // Push notification configuration
    bool pushEnabled = false;
    std::string pushEndpoint;        // UnifiedPush endpoint URL

    // Network / timeout configuration
    int64_t timeoutMs = 30000;
    int64_t maxTimeoutMs = 120000;
    int retryCount = 3;

    // Security configuration
    bool enableEncryption = false;
    bool pinCodeEnabled = false;
    std::string pinCode;             // Hashed/stored securely
    bool biometricAuthEnabled = false;

    // Feature flags (native module toggles)
    bool nativeHttpEnabled = false;      // Use C++ HTTP client
    bool nativeTimelineEnabled = false;
    bool webSearchEnabled = false;
    bool agentWebEnabled = false;

    // Draft configuration
    bool draftEnabled = false;
    int draftThreshold = 20;
    int draftIntervalMs = 3000;

    // Additional config flags
    bool useOnlineKeyBackup = false;
    bool useOnlyHttp1 = false;       // Force HTTP/1.1 only
    bool allowIpv6 = true;

    bool isValid() const {
        return !homeserverUrl.empty() && !accessToken.empty() && !userId.empty();
    }

    bool hasEncryption() const {
        return enableEncryption;
    }

    bool hasBiometricAuth() const {
        return biometricAuthEnabled;
    }

    bool hasPinCode() const {
        return pinCodeEnabled && !pinCode.empty();
    }
};

// Serialize session config to JSON for passing to C++ layer via JNI.
inline std::string sessionConfigToJson(const SessionConfig& config) {
    return "{"
        R"("homeserverUrl":")" + config.homeserverUrl + R"(")"
        R"(,"accessToken":")" + config.accessToken + R"(")"
        R"(,"userId":")" + config.userId + R"(")"
        R"(,"deviceId":")" + config.deviceId + R"(")"
        R"(,"nativeHttp":)" + (config.nativeHttpEnabled ? "true" : "false") +
        R"(,"nativeTimeline":)" + (config.nativeTimelineEnabled ? "true" : "false") +
        R"(,"webSearch":)" + (config.webSearchEnabled ? "true" : "false") +
        R"(,"agentWeb":)" + (config.agentWebEnabled ? "true" : "false") +
        "}";
}

// Build full session config JSON including all expanded fields.
inline std::string buildSessionConfigJson(const SessionConfig& config) {
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
    json << R"("homeserverUrl": ")" << esc(config.homeserverUrl) << R"(")";
    json << R"(,"accessToken": ")" << esc(config.accessToken) << R"(")";
    json << R"(,"userId": ")" << esc(config.userId) << R"(")";
    json << R"(,"deviceId": ")" << esc(config.deviceId) << R"(")";
    json << R"(,"sessionId": ")" << esc(config.sessionId) << R"(")";
    json << R"(,"loginTimestamp": )" << config.loginTimestamp << ",";
    json << R"("pushEnabled": )" << (config.pushEnabled ? "true" : "false") << ",";
    json << R"("pushEndpoint": ")" << esc(config.pushEndpoint) << R"(",)";
    json << R"("timeoutMs": )" << config.timeoutMs << ",";
    json << R"("maxTimeoutMs": )" << config.maxTimeoutMs << ",";
    json << R"("retryCount": )" << config.retryCount << ",";
    json << R"("enableEncryption": )" << (config.enableEncryption ? "true" : "false") << ",";
    json << R"("pinCodeEnabled": )" << (config.pinCodeEnabled ? "true" : "false") << ",";
    json << R"("biometricAuthEnabled": )" << (config.biometricAuthEnabled ? "true" : "false") << ",";
    json << R"("nativeHttpEnabled": )" << (config.nativeHttpEnabled ? "true" : "false") << ",";
    json << R"("nativeTimelineEnabled": )" << (config.nativeTimelineEnabled ? "true" : "false") << ",";
    json << R"("webSearchEnabled": )" << (config.webSearchEnabled ? "true" : "false") << ",";
    json << R"("agentWebEnabled": )" << (config.agentWebEnabled ? "true" : "false") << ",";
    json << R"("draftEnabled": )" << (config.draftEnabled ? "true" : "false") << ",";
    json << R"("draftThreshold": )" << config.draftThreshold << ",";
    json << R"("draftIntervalMs": )" << config.draftIntervalMs << ",";
    json << R"("useOnlineKeyBackup": )" << (config.useOnlineKeyBackup ? "true" : "false") << ",";
    json << R"("useOnlyHttp1": )" << (config.useOnlyHttp1 ? "true" : "false") << ",";
    json << R"("allowIpv6": )" << (config.allowIpv6 ? "true" : "false");
    json << "}";
    return json.str();
}

// Parse session config from JSON (compatible with buildSessionConfigJson output).
// Minimal parser using string searches.
inline SessionConfig parseSessionConfigJson(const std::string& json) {
    SessionConfig config;

    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else out += c;
        }
        return out;
    };
    auto findStr = [&](const std::string& key) -> std::string {
        auto v = findJsonValue(json, key);
        // don't double-escape
        return v;
    };
    auto findB = [&](const std::string& key, bool defVal = false) -> bool {
        auto v = findJsonValue(json, key);
        if (v.empty()) return defVal;
        return v == "true";
    };
    auto findI64 = [&](const std::string& key, int64_t defVal = 0) -> int64_t {
        auto v = findJsonValue(json, key);
        if (v.empty()) return defVal;
        return std::strtoll(v.c_str(), nullptr, 10);
    };
    auto findI = [&](const std::string& key, int defVal = 0) -> int {
        auto v = findJsonValue(json, key);
        if (v.empty()) return defVal;
        return static_cast<int>(std::strtoll(v.c_str(), nullptr, 10));
    };

    config.homeserverUrl = findStr("homeserverUrl");
    config.accessToken   = findStr("accessToken");
    config.userId        = findStr("userId");
    config.deviceId      = findStr("deviceId");
    config.sessionId     = findStr("sessionId");
    config.loginTimestamp   = findI64("loginTimestamp", 0);
    config.pushEnabled      = findB("pushEnabled", false);
    config.pushEndpoint     = findStr("pushEndpoint");
    config.timeoutMs        = findI64("timeoutMs", 30000);
    config.maxTimeoutMs     = findI64("maxTimeoutMs", 120000);
    config.retryCount       = findI("retryCount", 3);
    config.enableEncryption     = findB("enableEncryption", false);
    config.pinCodeEnabled       = findB("pinCodeEnabled", false);
    config.pinCode              = findStr("pinCode");
    config.biometricAuthEnabled = findB("biometricAuthEnabled", false);
    config.nativeHttpEnabled    = findB("nativeHttpEnabled", false);
    config.nativeTimelineEnabled = findB("nativeTimelineEnabled", false);
    config.webSearchEnabled     = findB("webSearchEnabled", false);
    config.agentWebEnabled      = findB("agentWebEnabled", false);
    config.draftEnabled         = findB("draftEnabled", false);
    config.draftThreshold       = findI("draftThreshold", 20);
    config.draftIntervalMs      = findI("draftIntervalMs", 3000);
    config.useOnlineKeyBackup   = findB("useOnlineKeyBackup", false);
    config.useOnlyHttp1         = findB("useOnlyHttp1", false);
    config.allowIpv6            = findB("allowIpv6", true);

    return config;
}

// ==== Build Info ====

struct BuildInfo {
    std::string versionName;     // "0.1"
    int versionCode = 1;
    std::string gitHash;         // Short commit hash
    std::string buildType;       // "debug" or "release"
    std::string flavor;          // "fdroid" or "gplay"
    int64_t buildTimestamp = 0;  // Unix timestamp
    std::string sdkVersion;      // Matrix SDK version used
    int nativeModuleCount = 0;   // Number of active C++ modules
    std::string olmVersion;      // libolm version
};

inline std::string buildInfoToJson(const BuildInfo& info) {
    return "{"
        R"("version":")" + info.versionName + R"(")"
        R"(,"code":)" + std::to_string(info.versionCode) +
        R"(,"git":")" + info.gitHash + R"(")"
        R"(,"build":")" + info.buildType + R"(")"
        R"(,"flavor":")" + info.flavor + R"(")"
        R"(,"timestamp":)" + std::to_string(info.buildTimestamp) +
        R"(,"sdk":")" + info.sdkVersion + R"(")"
        R"(,"nativeModules":)" + std::to_string(info.nativeModuleCount) +
        R"(,"olm":")" + info.olmVersion + R"(")"
        "}";
}

} // namespace progressive
