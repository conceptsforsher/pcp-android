#ifndef PROGRESSIVE_DEVICE_MANAGER_HPP
#define PROGRESSIVE_DEVICE_MANAGER_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <optional>

namespace progressive {

// ---- Device Info ----

struct ManagedDeviceInfo {
    std::string deviceId;
    std::string displayName;
    std::string lastSeenIp;
    int64_t lastSeenTs = 0;
    int64_t lastSeenAtMs = 0;
    bool isVerified = false;
    bool isCurrentDevice = false;
    bool isInactive = false;     // 90+ days since last activity
    std::string deviceType;      // "Mobile", "Web", "Desktop", "Unknown"
    std::string applicationName;
    std::string applicationVersion;
    std::string operatingSystem;
    std::string sessionName;
};

struct DeviceStats {
    int totalDevices = 0;
    int verifiedDevices = 0;
    int unverifiedDevices = 0;
    int inactiveDevices = 0;
    int currentDeviceIndex = -1;
    std::vector<ManagedDeviceInfo> devices;
};

// ---- Device Info (port of Kotlin DeviceInfo.kt:26-68) ----
// Original Kotlin:
//   data class DeviceInfo(
//       @Json(name = "user_id") val userId: String?,
//       @Json(name = "device_id") val deviceId: String?,
//       @Json(name = "display_name") val displayName: String?,
//       @Json(name = "last_seen_ts") val lastSeenTs: Long?,
//       @Json(name = "last_seen_ip") val lastSeenIp: String?,
//       @Json(name = "org.matrix.msc3852.last_seen_user_agent") val unstableLastSeenUserAgent: String?,
//       @Json(name = "last_seen_user_agent") val lastSeenUserAgent: String?
//   ) : DatedObject

struct DeviceInfo {
    std::string userId;
    std::string deviceId;
    std::string displayName;
    int64_t lastSeenTs = 0;
    std::string lastSeenIp;
    std::string unstableLastSeenUserAgent;
    std::string lastSeenUserAgent;

    // Original Kotlin: fun getBestLastSeenUserAgent() = lastSeenUserAgent ?: unstableLastSeenUserAgent
    std::string bestLastSeenUserAgent() const;
};

// ---- Device Trust Level (port of Kotlin DeviceTrustLevel.kt:18-25) ----
// Original Kotlin:
//   data class DeviceTrustLevel(
//       val crossSigningVerified: Boolean,
//       val locallyVerified: Boolean?
//   ) {
//       fun isVerified() = crossSigningVerified || locallyVerified == true
//       fun isCrossSigningVerified() = crossSigningVerified
//       fun isLocallyVerified() = locallyVerified
//   }

struct DeviceTrustLevel {
    bool crossSigningVerified = false;
    std::optional<bool> locallyVerified;

    // Original Kotlin: fun isVerified() = crossSigningVerified || locallyVerified == true
    bool isVerified() const;

    // Original Kotlin: fun isCrossSigningVerified() = crossSigningVerified
    bool isCrossSigningVerified() const;

    // Original Kotlin: fun isLocallyVerified() = locallyVerified
    bool isLocallyVerified() const;
};

// ---- Unsigned Device Info (port of Kotlin UnsignedDeviceInfo.kt:22-29) ----
// Original Kotlin:
//   data class UnsignedDeviceInfo(
//       @Json(name = "device_display_name") val deviceDisplayName: String?
//   )

struct UnsignedDeviceInfo {
    std::string deviceDisplayName;
};

// ---- Crypto Device Info (port of Kotlin CryptoDeviceInfo.kt:22-78) ----
// Original Kotlin:
//   data class CryptoDeviceInfo(
//       val deviceId: String,
//       override val userId: String,
//       val algorithms: List<String>?,
//       override val keys: Map<String, String>?,
//       override val signatures: Map<String, Map<String, String>>?,
//       val unsigned: UnsignedDeviceInfo?,
//       var trustLevel: DeviceTrustLevel?,
//       val isBlocked: Boolean,
//       val firstTimeSeenLocalTs: Long?
//   ) : CryptoInfo

struct CryptoDeviceInfo {
    std::string deviceId;
    std::string userId;
    std::vector<std::string> algorithms;
    std::unordered_map<std::string, std::string> keys;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> signatures;
    UnsignedDeviceInfo unsigned;
    std::optional<DeviceTrustLevel> trustLevel;
    bool isBlocked = false;
    int64_t firstTimeSeenLocalTs = 0;

    // Original Kotlin: val isVerified: Boolean get() = trustLevel?.isVerified() == true
    bool isDeviceVerified() const;

    // Original Kotlin: val isCrossSigningVerified: Boolean get() = trustLevel?.isCrossSigningVerified() == true
    bool isDeviceCrossSigningVerified() const;

    // Original Kotlin: val isUnknown: Boolean get() = trustLevel == null
    bool isDeviceUnknown() const;

    // Original Kotlin: fun fingerprint(): String? { keys?.takeIf {...}?.get("ed25519:$deviceId") }
    std::string fingerprint() const;

    // Original Kotlin: fun identityKey(): String? { keys?.get("curve25519:$deviceId") }
    std::string identityKey() const;

    // Original Kotlin: fun displayName(): String? { return unsigned?.deviceDisplayName }
    std::string displayName() const;

    // Original Kotlin: fun shortDebugString() = "$userId|$deviceId"
    std::string shortDebugString() const;
};

// Parse device list from Matrix /devices API response.
DeviceStats parseDeviceList(const std::string& apiResponseJson, const std::string& currentDeviceId);

// Classify device type from user agent string.
std::string classifyDeviceType(const std::string& userAgent, const std::string& clientName);

// Check if device is inactive (>90 days since last seen).
bool isDeviceInactive(int64_t lastSeenMs, int64_t nowMs = 0);

// Format device last seen as relative time.
std::string formatDeviceLastSeen(int64_t lastSeenMs);

// Format device stats as text for settings display.
std::string formatDeviceStats(const DeviceStats& stats);

// Format device info as JSON (defined in crypto_models.hpp)
std::string managedManagedDeviceInfoToJson(const ManagedDeviceInfo& device);

// A device with its resolution timestamp (millis since epoch).

// Format device list as JSON.
std::string deviceListToJson(const DeviceStats& stats);

// Get security recommendation for a device.
std::string getDeviceRecommendation(const ManagedDeviceInfo& device);

// Sort devices (by name, by last seen, by verification).
void sortDevices(std::vector<ManagedDeviceInfo>& devices, const std::string& sortBy);

// ---- Device Crypto (from CryptoDeviceInfo.kt 78L) ----
// Extract ed25519 fingerprint from device keys map.
std::string extractDeviceFingerprint(const std::string& deviceId, const std::string& keysJson);

// Extract curve25519 identity key from device keys map.
std::string extractDeviceIdentityKey(const std::string& deviceId, const std::string& keysJson);

// Format fingerprint for display: chunk into groups of 4.
std::string formatFingerprint(const std::string& fingerprint);

// ---- Session Rename ----

struct SessionRename {
    std::string sessionId;
    std::string newName;
    bool valid = false;
    std::string error;
};

// Validate a session rename request.
SessionRename validateSessionRename(const std::string& sessionId, const std::string& newName);

// Build session rename request body JSON.
std::string buildSessionRenameBody(const std::string& sessionId, const std::string& newName);

// ---- Expanded Device Manager API ----

// Compute device trust level from cross-signing and local verification status.
// Original Kotlin (Device.kt:164-166):
//   fun trustLevel(): DeviceTrustLevel {
//       return DeviceTrustLevel(crossSigningVerified = innerDevice.crossSigningTrusted, locallyVerified = innerDevice.locallyTrusted)
//   }
DeviceTrustLevel computeDeviceTrustLevel(bool crossSigningVerified, std::optional<bool> locallyVerified);

// Serialize DeviceInfo to JSON string.
std::string deviceInfoToJson(const DeviceInfo& info);

// Parse a single DeviceInfo object from JSON string.
DeviceInfo parseDeviceInfo(const std::string& json);

// Compare two DeviceTrustLevel values for UI sorting.
// Returns -1 if `a` is more trusted, 1 if `b` is more trusted, 0 if equal.
// Original Kotlin sorting logic: cross-signing verified > locally verified > unknown
int compareDeviceTrust(const DeviceTrustLevel& a, const DeviceTrustLevel& b);

// Format device display name with fallback to default.
// Original Kotlin: unsigned?.deviceDisplayName ?: defaultDisplayName
std::string formatDeviceDisplayName(const std::string& defaultName, const std::string& unsignedDisplayName);

// Serialize CryptoDeviceInfo to JSON.
std::string cryptoDeviceInfoToJson(const CryptoDeviceInfo& info);

// Parse CryptoDeviceInfo from JSON string.
CryptoDeviceInfo parseCryptoDeviceInfo(const std::string& json);

} // namespace progressive

#endif // PROGRESSIVE_DEVICE_MANAGER_HPP
