#ifndef PROGRESSIVE_CLIENT_INFO_HPP
#define PROGRESSIVE_CLIENT_INFO_HPP

#include <string>
#include <cstdint>

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

} // namespace progressive

#endif // PROGRESSIVE_CLIENT_INFO_HPP
