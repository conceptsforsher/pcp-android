#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace progressive {

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

struct SessionConfig {
    std::string homeserverUrl;
    std::string accessToken;
    std::string userId;
    std::string deviceId;
    std::string sessionId;           // Unique session identifier
    int64_t loginTimestamp = 0;      // When session was created
    bool pushEnabled = false;
    std::string pushEndpoint;        // UnifiedPush endpoint URL
    bool nativeHttpEnabled = false;  // Use C++ HTTP client
    bool nativeTimelineEnabled = false;
    bool webSearchEnabled = false;
    bool agentWebEnabled = false;
    bool draftEnabled = false;
    int draftThreshold = 20;
    int draftIntervalMs = 3000;

    bool isValid() const {
        return !homeserverUrl.empty() && !accessToken.empty() && !userId.empty();
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
