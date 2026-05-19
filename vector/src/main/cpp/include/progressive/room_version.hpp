#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Room Version Info & Features
// ================================================================

// ==== RoomVersionStatus ====
//
// Original Kotlin: RoomVersionStatus.kt
enum class RoomVersionStatus {
    STABLE,
    UNSTABLE,
    DEPRECATED,
    DISCONTINUED
};

const char* roomVersionStatusToString(RoomVersionStatus status);
RoomVersionStatus roomVersionStatusFromString(const std::string& s);

// ==== VersionComparison ====
//
// Original Kotlin: VersionComparison.kt
enum class VersionComparison {
    SAME,
    NEWER,
    OLDER,
    INCOMPARABLE
};

const char* versionComparisonToString(VersionComparison cmp);

// ==== Matrix Room Version Constants ====
//
// Original Kotlin: RoomVersion.kt constants
namespace RoomVersions {
    constexpr auto V1  = "1";
    constexpr auto V2  = "2";
    constexpr auto V3  = "3";
    constexpr auto V4  = "4";
    constexpr auto V5  = "5";
    constexpr auto V6  = "6";
    constexpr auto V7  = "7";
    constexpr auto V8  = "8";
    constexpr auto V9  = "9";
    constexpr auto V10 = "10";
    constexpr auto V11 = "11";
}

// ==== Room Version Features ====
//
// Original Kotlin: RoomVersionFeatures.kt
namespace RoomVersionFeatures {
    constexpr auto STATE_RESOLUTION_V2    = "state_resolution_v2";
    constexpr auto EVENT_ID_HASHES        = "event_id_hashes";
    constexpr auto KNOCK                  = "knock";
    constexpr auto RESTRICTED             = "restricted";
    constexpr auto IMPLICIT_PL            = "implicit_power_levels";
    constexpr auto IGNORE_PL_INVITE       = "ignore_power_level_invite";
    constexpr auto KNOCK_REASON           = "knock_reason";
    constexpr auto IMPLICIT_PROFILES      = "implicit_profiles";
    constexpr auto REDACTION_RULES        = "redaction_rules";
    constexpr auto ROOM_VERSION_11        = "room_version_11";
}

// ==== RoomVersionInfo ====
//
// Original Kotlin: RoomVersionInfo.kt
struct RoomVersionInfo {
    std::string version;
    RoomVersionStatus status = RoomVersionStatus::STABLE;
    std::string releaseDate;
    std::vector<std::string> features;
};

// ==== RoomVersion (legacy struct) ====
//
// Original Kotlin: RoomVersion data class
struct RoomVersion {
    std::string id;          // e.g. "1", "9", "10"
    std::string description; // e.g. "v1 - Stable", "v10 - MSC... "
    bool isDefault = false;
    bool isStable = false;
};

// ==== RoomVersionCapability ====
//
// Original Kotlin: RoomVersionCapability (from /capabilities)
struct RoomVersionCapability {
    std::string defaultVersion;
    std::unordered_map<std::string, std::string> available;
    bool isDefaultStable = false;
};

// ================================================================
// Functions
// ================================================================

// ---- Version Info ----

// Get detailed info for a specific room version.
// Original Kotlin: getRoomVersionInfo()
RoomVersionInfo getRoomVersionInfo(const std::string& version);

// Get all known room versions with full metadata.
// Original Kotlin: getAllRoomVersions()
std::vector<RoomVersionInfo> getAllRoomVersions();

// Get the list of features available in a given room version.
// Original Kotlin: getRoomVersionFeatures()
std::vector<std::string> getRoomVersionFeatures(const std::string& version);

// Check whether a feature is supported in a given room version.
// Original Kotlin: isFeatureSupported()
bool isFeatureSupported(const std::string& version, const std::string& feature);

// Get the latest stable room version.
// Original Kotlin: getLatestRoomVersion()
std::string getLatestRoomVersion();

// Get the default recommended room version.
// Original Kotlin: getDefaultRoomVersion()
std::string getDefaultRoomVersion();

// ---- Legacy ----

// Get list of known Matrix room versions (legacy format).
std::vector<RoomVersion> getKnownRoomVersions();

// Check if a room version string is valid.
bool isValidRoomVersion(const std::string& version);

// Format room versions as JSON for settings UI.
std::string roomVersionsToJson();

// ---- Stability Checks ----

// Check if a room version is considered stable.
// Original Kotlin: isRoomVersionStable()
bool isRoomVersionStable(const std::string& version);

// Check if a room version is deprecated.
// Original Kotlin: isRoomVersionDeprecated()
bool isRoomVersionDeprecated(const std::string& version);

// ---- Version Comparison ----

// Compare two room version strings numerically.
// Returns SAME if equal, NEWER if v1 > v2, OLDER if v1 < v2,
// INCOMPARABLE if either version is not a recognized numeric version.
// Original Kotlin: compareVersions()
VersionComparison compareVersions(const std::string& v1, const std::string& v2);

// ---- Capabilities Parsing ----

// Parse the m.room_versions capability from a /capabilities response.
// Original Kotlin: parseRoomVersionsCapability()
RoomVersionCapability parseRoomVersionsCapability(const std::string& capabilitiesJson);

} // namespace progressive
