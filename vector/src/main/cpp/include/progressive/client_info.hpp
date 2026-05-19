#pragma once

#include <string>
#include <cstdint>
#include <sstream>

namespace progressive {

// ---- Client Info (for /versions and User-Agent) ----

struct ClientVersionInfo {
    std::string appName;           // "Progressive Chat"
    std::string appVersion;        // "1.0.0"
    std::string sdkVersion;        // Matrix SDK version
    std::string olmVersion;        // libolm version
    std::string platform;          // "Android 14"
    std::string deviceModel;       // "Pixel 8"
    std::string architecture;      // "arm64-v8a"
    int64_t buildTimestamp = 0;
    std::string gitHash;           // commit SHA
};

// Build User-Agent string for HTTP requests.
std::string buildUserAgent(const ClientVersionInfo& info);

// Build Client-Version query string for /versions API.
std::string buildClientVersionQuery(const ClientVersionInfo& info);

// Parse server version from /versions API response.
std::string parseServerVersion(const std::string& apiResponseJson);

// Check if client version is compatible with server version.
bool isServerCompatible(const std::string& serverVersion, const std::string& minRequiredVersion);

// Format version info for About screen.
std::string formatVersionInfo(const ClientVersionInfo& info);

// Compare two semantic version strings.
// Returns: -1 if a < b, 0 if equal, 1 if a > b.
int compareSemver(const std::string& a, const std::string& b);

// Check if a version satisfies a minimum requirement.
bool satisfiesMinVersion(const std::string& current, const std::string& minimum);

// ---- Build Constants ----

struct BuildInfo {
    std::string flavor;            // "gplay" or "fdroid"
    std::string buildType;         // "debug" or "release"
    bool isDebug = false;
    bool isNightly = false;
    std::string applicationId;     // "im.vector.app"
    int versionCode = 1;
    std::string versionName;       // "1.0.0"
};

// Check if this is a development build.
bool isDevelopmentBuild(const BuildInfo& info);

// Format build info for bug reports.
std::string formatBuildInfo(const BuildInfo& info);

// Get the appropriate app name based on build type.
std::string getAppDisplayName(const BuildInfo& info);

// ---- Matrix Client Info (for PUT /account/whoami) ----
// Original Kotlin: client info sent to server for device identification
// Mirrors the structure sent to PUT /_matrix/client/r0/account/whoami

struct MatrixClientInfo {
    std::string clientName;               // Human-readable client name
    std::string clientVersion;            // Client version string
    std::string clientUrl;                // Client homepage / repo URL
    std::string deviceId;                 // Device ID
    std::string initialDeviceDisplayName; // Initial display name for the device
    std::string stableClientName;         // Stable client identifier (e.g. "io.element.android")
    std::string deviceType;               // e.g. "android", "ios", "web"
    std::string osName;                   // Operating system name
    std::string osVersion;                // Operating system version
    std::string appPackage;               // App package / bundle ID

    bool isValid() const {
        return !clientName.empty() && !deviceId.empty();
    }

    bool hasClientUrl() const {
        return !clientUrl.empty();
    }
};

// Manual JSON value extractor (local helper).
namespace {
    inline std::string extractValue(const std::string& json, const std::string& key) {
        std::string search = '"' + key + '"';
        auto pos = json.find(search);
        if (pos == std::string::npos) return {};
        pos += search.size();
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
            ++pos;
        if (pos >= json.size() || json[pos] != ':') return {};
        ++pos;
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
            ++pos;
        if (pos >= json.size()) return {};
        if (json[pos] == '"') {
            ++pos;
            auto end = json.find('"', pos);
            if (end == std::string::npos) return {};
            return json.substr(pos, end - pos);
        }
        auto end = pos;
        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ' &&
               json[end] != '\t' && json[end] != '\n' && json[end] != '\r')
            ++end;
        return json.substr(pos, end - pos);
    }
} // anonymous namespace

// Original Kotlin: Build MatrixClientInfo JSON for PUT /_matrix/client/r0/account/whoami
// Request body format:
// {
//   "client_name": "Progressive Chat",
//   "client_version": "1.0.0",
//   "client_url": "https://github.com/...",
//   "device_id": "ABC123",
//   "initial_device_display_name": "Pixel 8"
// }
inline std::string buildMatrixClientInfo(const MatrixClientInfo& info) {
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
    json << R"("client_name": ")" << esc(info.clientName) << R"(")";
    json << R"(,"client_version": ")" << esc(info.clientVersion) << R"(")";
    if (!info.clientUrl.empty()) {
        json << R"(,"client_url": ")" << esc(info.clientUrl) << R"(")";
    }
    json << R"(,"device_id": ")" << esc(info.deviceId) << R"(")";
    if (!info.initialDeviceDisplayName.empty()) {
        json << R"(,"initial_device_display_name": ")" << esc(info.initialDeviceDisplayName) << R"(")";
    }
    if (!info.deviceType.empty()) {
        json << R"(,"device_type": ")" << esc(info.deviceType) << R"(")";
    }
    if (!info.osName.empty()) {
        json << R"(,"os_name": ")" << esc(info.osName) << R"(")";
    }
    if (!info.osVersion.empty()) {
        json << R"(,"os_version": ")" << esc(info.osVersion) << R"(")";
    }
    if (!info.appPackage.empty()) {
        json << R"(,"app_package": ")" << esc(info.appPackage) << R"(")";
    }
    if (!info.stableClientName.empty()) {
        json << R"(,"stable_client_name": ")" << esc(info.stableClientName) << R"(")";
    }
    json << "}";
    return json.str();
}

// Original Kotlin: Parse MatrixClientInfo from JSON (server response or storage).
inline MatrixClientInfo parseMatrixClientInfo(const std::string& json) {
    MatrixClientInfo info;
    info.clientName               = extractValue(json, "client_name");
    info.clientVersion            = extractValue(json, "client_version");
    info.clientUrl                = extractValue(json, "client_url");
    info.deviceId                 = extractValue(json, "device_id");
    info.initialDeviceDisplayName = extractValue(json, "initial_device_display_name");
    info.deviceType               = extractValue(json, "device_type");
    info.osName                   = extractValue(json, "os_name");
    info.osVersion                = extractValue(json, "os_version");
    info.appPackage               = extractValue(json, "app_package");
    info.stableClientName         = extractValue(json, "stable_client_name");
    return info;
}

} // namespace progressive
